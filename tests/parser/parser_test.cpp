#include "http/HttpMessage.hpp"
#include "http/HttpParser.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

// Test basic GET request
TEST(HttpParserTest, ParseSimpleGetRequest) {
  HttpParser parser;
  std::string raw = "GET /index.html HTTP/1.1\r\n\r\n";

  HttpRequest req = parser.parse(raw);

  EXPECT_EQ(req.getMethod(), Method::GET);
  EXPECT_EQ(req.getRequestURI(), "/index.html");
  EXPECT_EQ(req.getHttpVersion(), HTTPVersion::HTTP1x1);
  EXPECT_TRUE(req.getHeaders().empty());
  EXPECT_FALSE(req.hasBody());
}

// Test GET request with headers
TEST(HttpParserTest, ParseGetRequestWithHeaders) {
  HttpParser parser;
  std::string raw = "GET /api/users HTTP/1.1\r\n"
                    "Host: example.com\r\n"
                    "User-Agent: Mozilla/5.0\r\n"
                    "Accept: application/json\r\n"
                    "\r\n";

  HttpRequest req = parser.parse(raw);

  EXPECT_EQ(req.getMethod(), Method::GET);
  EXPECT_EQ(req.getRequestURI(), "/api/users");
  EXPECT_EQ(req.getHttpVersion(), HTTPVersion::HTTP1x1);

  ASSERT_EQ(req.getHeaders().size(), 3);
  EXPECT_EQ((req.getHeaders()[0].k_), "Host");
  EXPECT_EQ(req.getHeaders()[0].v_, "example.com");
  EXPECT_EQ((req.getHeaders()[1].k_), "User-Agent");
  EXPECT_EQ(req.getHeaders()[1].v_, "Mozilla/5.0");
  EXPECT_EQ((req.getHeaders()[2].v_), "Accept");
  EXPECT_EQ(req.getHeaders()[2].v_, "application/json");
}

// Test POST request with body
TEST(HttpParserTest, ParsePostRequestWithBody) {
  HttpParser parser;
  std::string raw = "POST /api/login HTTP/1.1\r\n"
                    "Host: example.com\r\n"
                    "Content-Type: application/json\r\n"
                    "Content-Length: 27\r\n"
                    "\r\n"
                    "{\"user\":\"test\",\"pass\":\"123\"}";

  HttpRequest req = parser.parse(raw);

  EXPECT_EQ(req.getMethod(), Method::POST);
  EXPECT_EQ(req.getRequestURI(), "/api/login");

  ASSERT_EQ(req.getHeaders().size(), 3);

  ASSERT_TRUE(req.hasBody());
  EXPECT_EQ(req.getBody().value(), "{\"user\":\"test\",\"pass\":\"123\"}");
}

// Test all HTTP methods
TEST(HttpParserTest, ParseAllHttpMethods) {
  std::vector<std::pair<std::string, Method>> methods = {
      {"GET", Method::GET},     {"POST", Method::POST},
      {"PUT", Method::PUT},     {"DELETE", Method::DELETE},
      {"HEAD", Method::HEAD},   {"OPTIONS", Method::OPTIONS},
      {"TRACE", Method::TRACE}, {"CONNECT", Method::CONNECT}};

  for (const auto &[method_str, expected_method] : methods) {
    HttpParser parser;
    std::string raw = method_str + " / HTTP/1.1\r\n\r\n";

    HttpRequest req = parser.parse(raw);

    EXPECT_EQ(req.getMethod(), expected_method)
        << "Failed for method: " << method_str;
  }
}

// Test header with leading/trailing whitespace
TEST(HttpParserTest, ParseHeaderWithWhitespace) {
  HttpParser parser;
  std::string raw = "GET / HTTP/1.1\r\n"
                    "Host:   example.com   \r\n"
                    "Accept:application/json\r\n"
                    "\r\n";

  HttpRequest req = parser.parse(raw);

  ASSERT_EQ(req.getHeaders().size(), 2);
  EXPECT_EQ(req.getHeaders()[0].k_, "Host");
  EXPECT_EQ(req.getHeaders()[0].v_, "example.com");
  EXPECT_EQ((req.getHeaders()[1].v_), "Accept");
  EXPECT_EQ(req.getHeaders()[1].v_, "application/json");
}

// Test duplicate headers (e.g., Cookie)
TEST(HttpParserTest, ParseDuplicateHeaders) {
  HttpParser parser;
  std::string raw = "GET / HTTP/1.1\r\n"
                    "Cookie: session=abc\r\n"
                    "Cookie: user=xyz\r\n"
                    "Cookie: token=123\r\n"
                    "\r\n";

  HttpRequest req = parser.parse(raw);

  ASSERT_EQ(req.getHeaders().size(), 3);
  EXPECT_EQ((req.getHeaders()[0].v_), "Cookie");
  EXPECT_EQ(req.getHeaders()[0].v_, "session=abc");
  EXPECT_EQ((req.getHeaders()[1].v_), "Cookie");
  EXPECT_EQ(req.getHeaders()[1].v_, "user=xyz");
  EXPECT_EQ((req.getHeaders()[2].v_), "Cookie");
  EXPECT_EQ(req.getHeaders()[2].v_, "token=123");
}

// Test different URI formats
TEST(HttpParserTest, ParseDifferentUriFormats) {
  std::vector<std::string> uris = {
      "/",
      "/index.html",
      "/api/v1/users",
      "/path/to/resource?query=value",
      "/page?foo=bar&baz=qux",
      "http://example.com/absolute",
      "*" // For OPTIONS requests
  };

  for (const auto &uri : uris) {
    HttpParser parser;
    std::string raw = "GET " + uri + " HTTP/1.1\r\n\r\n";

    HttpRequest req = parser.parse(raw);

    EXPECT_EQ(req.getRequestURI(), uri) << "Failed for URI: " << uri;
  }
}

// Test error: invalid method
TEST(HttpParserTest, InvalidMethodThrowsError) {
  HttpParser parser;
  std::string raw = "INVALID /index.html HTTP/1.1\r\n\r\n";

  EXPECT_THROW(parser.parse(raw), std::runtime_error);
}

// Test error: invalid HTTP version
TEST(HttpParserTest, InvalidVersionThrowsError) {
  HttpParser parser;
  std::string raw = "GET /index.html HTTP/2.0\r\n\r\n";

  EXPECT_THROW(parser.parse(raw), std::runtime_error);
}

// Test error: malformed request line (missing spaces)
TEST(HttpParserTest, MalformedRequestLineThrowsError) {
  HttpParser parser;
  std::string raw = "GET/index.html\r\n\r\n";

  EXPECT_THROW(parser.parse(raw), std::runtime_error);
}

// Test error: malformed header (no colon)
TEST(HttpParserTest, MalformedHeaderThrowsError) {
  HttpParser parser;
  std::string raw = "GET / HTTP/1.1\r\n"
                    "InvalidHeader\r\n"
                    "\r\n";

  EXPECT_THROW(parser.parse(raw), std::runtime_error);
}

// Test empty body
TEST(HttpParserTest, RequestWithNoBody) {
  HttpParser parser;
  std::string raw = "DELETE /api/user/123 HTTP/1.1\r\n"
                    "Host: example.com\r\n"
                    "\r\n";

  HttpRequest req = parser.parse(raw);

  EXPECT_FALSE(req.hasBody());
}

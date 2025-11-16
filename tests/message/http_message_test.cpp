#include "http/HttpMessage.hpp"
#include <gtest/gtest.h>
#include <string>

TEST(HttpMessageTest, GetStartLineWorksForRequest) {
  HttpRequest req(Method::GET, "/test", HTTPVersion::HTTP1x1);

  std::string start_line = req.getStartLine();

  EXPECT_EQ(start_line, "GET /test HTTP/1.1");
};

TEST(HttpMessageTest, GetStartLineWorksForResponse) {
  HttpResponse res(HTTPVersion::HTTP1x1, 200, "OK");

  std::string start_line = res.getStartLine();

  EXPECT_EQ(start_line, "HTTP/1.1 200 OK");
};

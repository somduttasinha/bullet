#include "HttpMessage.hpp"
#include "Utils.hpp"

HttpResponse act(const HttpRequest &request) {
  std::string body = "<h1>Hello World!<h>";
  auto resp = HttpResponse(HTTPVersion::HTTP1x1, 200, "OK");
  resp.setBody(body);
  resp.addHeader(Header("Content-Length", std::to_string(body.size())));
  resp.addHeader(Header("Content-Type", "text/html"));
  resp.addHeader(Header("Connection", "close"));
  resp.addHeader(Header("Content-Language", "en/gb"));
  resp.addHeader(Header("Date", date()));
  return resp;
};

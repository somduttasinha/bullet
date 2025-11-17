#include "HttpMessage.hpp"
#include "Utils.hpp"
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/syscall.h>
#include <unistd.h>

HttpResponse _handle_get(const HttpRequest &request) {
  std::ifstream istrm("../dist/index.html", std::ios::binary);
  if (!istrm.is_open())
    std::cout << "failed to open " << "../dist/index.html" << "\n";
  else {
    auto resp = HttpResponse(HTTPVersion::HTTP1x1, 200, "OK");

    std::cout << "yeboi we in" << std::endl;
    std::string body;
    std::string line;
    while (std::getline(istrm, line)) {
      std::cout << line << std::endl;
      body += line;
    }
    resp.setBody(body);
    resp.addHeader(Header("Content-Length", std::to_string(body.size())));
    resp.addHeader(Header("Content-Type", "text/html"));
    resp.addHeader(Header("Connection", "close"));
    resp.addHeader(Header("Content-Language", "en/gb"));
    resp.addHeader(Header("Date", date()));
    return resp;
  }
  return HttpResponse();
};

HttpResponse act(const HttpRequest &request) {
  switch (request.getMethod()) {
  case Method::OPTIONS:
  case Method::GET: {
    return _handle_get(request);
  }
  case Method::HEAD:
  case Method::POST:
  case Method::PUT:
  case Method::DELETE:
  case Method::TRACE:
  case Method::CONNECT:
    break;
  }
  return HttpResponse();
};

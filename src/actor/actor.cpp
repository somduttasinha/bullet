#include "HttpMessage.hpp"
#include "Utils.hpp"
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <sys/syscall.h>
#include <unistd.h>

std::string _parse_path(const std::string &uri) {
  // if it doesn't end in a .xyz, append .html
  if (uri.ends_with("/")) {
    return uri + "index.html";
  } else {
    return uri;
  }
}

HttpResponse _handle_get(std::string web_root, const HttpRequest &request) {
  auto path = _parse_path(request.getRequestURI());
  fmt::print("\n\npath: {}\n\n", path);
  auto relative_path = web_root + path;
  std::filesystem::path file_path = relative_path;

  auto resp = HttpResponse(HTTPVersion::HTTP1x1, 200, "OK");

  if (std::filesystem::is_directory(relative_path)) {
    file_path = relative_path + "/index.html";
  } else {
    auto ext = file_path.extension();
    fmt::println("Ext: {}", ext.string());
    if (ext.string().ends_with("html")) {
      resp.addHeader(Header("Content-Type", "text/html"));
      resp.addHeader(Header("Content-Language", "en/gb"));
    } else if (ext.string().ends_with("css")) {
      resp.addHeader(Header("Content-Type", "text/css"));
    } else if (ext.string().ends_with("woff")) {
      resp.addHeader(Header("Content-Type", "font/woff"));
    } else if (ext.string().ends_with("woff2")) {
      resp.addHeader(Header("Content-Type", "font/woff2"));
    } else if (ext.string().ends_with("collection")) {
      resp.addHeader(Header("Content-Type", "font/collection"));
    } else if (ext.string().ends_with("otf")) {
      resp.addHeader(Header("Content-Type", "font/otf"));
    } else if (ext.string().ends_with("sfnt")) {
      resp.addHeader(Header("Content-Type", "font/sfnt"));
    } else if (ext.string().ends_with("ttf")) {
      resp.addHeader(Header("Content-Type", "font/ttf"));
    } else if (ext.string().ends_with("jpeg")) {
      resp.addHeader(Header("Content-Type", "image/jpeg"));
    } else if (ext.string().ends_with("jpg")) {
      resp.addHeader(Header("Content-Type", "image/jpg"));
    } else if (ext.string().ends_with("ico")) {
      resp.addHeader(Header("Content-Type", "image/ico"));
    } else if (ext.string().ends_with("js")) {
      resp.addHeader(Header("Content-Type", "text/javascript"));
    } else if (ext.string().ends_with("svg")) {
      resp.addHeader(Header("Content-Type", "image/svg+xml"));
    } else {
      fmt::println("No extension yet for {}", ext.string());
    }
  }
  fmt::println("file path: {}", file_path.string());
  std::ifstream istrm(file_path,
                      std::ios::in | std::ios::binary | std::ios::ate);
  if (!istrm.is_open())
    fmt::println("Failed to open {}", file_path.string());
  else {
    auto size = istrm.tellg();
    istrm.seekg(0, std::ios::beg);
    std::vector<uint8_t> body(size);
    istrm.read(reinterpret_cast<char *>(body.data()), size);
    istrm.close();

    resp.setBody(body);

    resp.addHeader(Header("Content-Length", std::to_string(body.size())));
    resp.addHeader(Header("Connection", "close"));
    resp.addHeader(Header("Date", date()));
    return resp;
  }
  return HttpResponse();
};

HttpResponse act(const HttpRequest &request) {
  switch (request.getMethod()) {
  case Method::OPTIONS:
  case Method::GET: {
    return _handle_get("../dist/", request);
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

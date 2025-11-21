#pragma once

#include "HttpMessage.hpp"
#include <cstdint>
#include <string>
#include <vector>

const std::string CRLF = "\r\n";

class HttpParser {
public:
  HttpParser();
  HttpParser(HttpParser &&) = default;
  HttpParser(const HttpParser &) = default;
  HttpParser &operator=(HttpParser &&) = default;
  HttpParser &operator=(const HttpParser &) = default;
  HttpRequest parse(const std::string &x);
  ~HttpParser();

private:
  enum class State {
    START,
    PARSING_START_LINE,
    PARSING_HEADERS,
    PARSING_BODY,
    DONE,
    ERROR
  };
  State state_;
  size_t pos_;

  Method parseMethod(const std::string &raw);
  HTTPVersion parseVersion(const std::string &version_str);
  void parseRequestLine(const std::string &start_line_str,
                        HttpRequest &request);
  Header parseHeaderLine(const std::string &line);
  std::optional<std::vector<uint8_t>> parseBody(const std::string &raw);
};

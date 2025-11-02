#pragma once

#include "HttpMessage.hpp"
#include <string>
#include <utility>

const std::string CRLF = "\r\n";

class HttpParser {
public:
  HttpParser();
  HttpParser(HttpParser &&) = default;
  HttpParser(const HttpParser &) = default;
  HttpParser &operator=(HttpParser &&) = default;
  HttpParser &operator=(const HttpParser &) = default;
  HttpMessage parse(const std::string &x);
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
  HttpMessage message_;
  size_t pos_;

  Method parseMethod(const std::string &raw);
  HTTPVersion parseVersion(const std::string &version_str);
  StartLine parseStartLine(const std::string &start_line_str);
  std::pair<std::string, std::string> parseHeaderLine(const std::string &line);
  std::optional<std::string> parseBody(const std::string &raw);
};

#pragma once

#include <string>
#include <unordered_map>
class HttpParser {
public:
  HttpParser();
  HttpParser(HttpParser &&) = default;
  HttpParser(const HttpParser &) = default;
  HttpParser &operator=(HttpParser &&) = default;
  HttpParser &operator=(const HttpParser &) = default;
  const std::unordered_map<std::string, std::string>
  parse(const std::string &x);
  ~HttpParser();

private:
};

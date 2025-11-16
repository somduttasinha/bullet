#include "HttpMessage.hpp"
#include "HttpParser.hpp"
#include "Utils.hpp"

std::string HttpRequest::getStartLineImpl() const {
  return methodToString(method_) + " " + request_uri_ + " " +
         versionToString(http_version_);
}

std::string HttpResponse::getStartLineImpl() const {
  return versionToString(http_version_) + " " + std::to_string(status_code_) +
         " " + reason_phrase_;
}

std::string HttpResponse::toString() const {
  auto raw = this->getStartLineImpl() + CRLF + join(this->headers_, CRLF) +
             CRLF + this->getBody().value_or("<h1>Hello World<h1>");
  return raw;
}

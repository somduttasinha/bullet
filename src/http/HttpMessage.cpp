#include "HttpMessage.hpp"
#include "HttpParser.hpp"
#include "Utils.hpp"
#include <cstdint>
#include <vector>

std::string HttpRequest::getStartLineImpl() const {
  return methodToString(method_) + " " + request_uri_ + " " +
         versionToString(http_version_);
}

std::string HttpResponse::getStartLineImpl() const {
  return versionToString(http_version_) + " " + std::to_string(status_code_) +
         " " + reason_phrase_;
}

std::vector<uint8_t> HttpResponse::get() const {
  auto raw =
      this->getStartLineImpl() + CRLF + join(this->headers_, CRLF) + CRLF;
  std::vector<uint8_t> response(raw.begin(), raw.end());

  auto b = this->getBody().value_or(std::vector<uint8_t>());
  response.insert(response.end(), b.begin(), b.end());
  return response;
}

#include "HttpMessage.hpp"

std::string HttpRequest::getStartLineImpl() const {
  return methodToString(method_) + " " + request_uri_ + " " + versionToString(http_version_);
}

std::string HttpResponse::getStartLineImpl() const {
  return versionToString(http_version_) + " " + std::to_string(status_code_) + " " + reason_phrase_;
}

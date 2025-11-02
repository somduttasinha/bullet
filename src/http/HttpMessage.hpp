#pragma once

#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

enum class Method { OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT };
enum class GeneralHeader {
  CacheControl,
  Connection,
  Date,
  Pragma,
  Trailer,
  TransferEncoding,
  Upgrade,
  Via,
  Warning
}; /// RFC2616, sec 4.5

/// TODO fill this
enum class RequestHeader {

};

/// TODO fill this
enum class ResponseHeader {

};

/// Recognised HTTP versions, for now we will only recognise HTTP1x1
enum class HTTPVersion { HTTP1x1 };

/// TODO: include all 4 possible variants given in RFC2616, sec 5.1.2
using RequestURI = std::string;

struct RequestLine {
  Method method;
  RequestURI request_uri;
  HTTPVersion http_version;
};

struct StatusLine {
  HTTPVersion http_version;
  int status_code;
  std::string reason_phrase;
};

using StartLine = std::variant<RequestLine, StatusLine>;

struct HttpMessage {
  StartLine start_line;
  std::vector<std::pair<std::string, std::string>> headers; // RFC2616, sec 4.2
  std::optional<std::string> body;
};

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

enum class ContentCoding {
  GZip,
  Compress,
  Deflate,
  Identity,
};

enum class RequestHeader {
  Accept,
  AcceptCharset,
  AcceptEncoding,
  AcceptLanguage,
  Authorization,
  Expect,
  From,
  Host,
  IfMatch,
  IfModifiedSince,
  IfNoneMatch,
  IfRange,
  IfUnmodifiedSince,
  MaxForwards,
  ProxyAuthorization,
  Range,
  Referer,
  TE,
  UserAgen
};

enum class ResponseHeader {
  AcceptRanges,
  Age,
  ETag,
  Location,
  ProxyAuthenticate,
  RetryAfter,
  Server,
  Vary,
  WWWAuthenticate,
};

enum class EntityHeader {
  Allow,
  ContentEncoding,
  ContentLanguage,
  ContentLength,
  ContentLocation,
  ContentMD5,
  ContentRange,
  ContentType,
  Expires,
  LastModified,
};

using Header =
    std::variant<GeneralHeader, RequestHeader, ResponseHeader, EntityHeader, std::string>;

enum class HTTPVersion { HTTP1x1 };

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
  std::vector<std::pair<Header, std::string>> headers; // RFC2616, sec 4.2
  std::optional<std::string> body;
};

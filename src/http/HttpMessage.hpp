#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

enum class Method { OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT };

enum class ContentCoding {
  GZip,
  Compress,
  Deflate,
  Identity,
};

struct Header {
  const std::string k_;
  const std::string v_;

  Header(std::string k, std::string v) : k_(k), v_(v) {};

  std::string toString() const { return k_ + ": " + v_; }
};

enum class HTTPVersion { HTTP1x1 };

using RequestURI = std::string;

// Helper functions for enum to string conversion
inline std::string methodToString(Method m) {
  switch (m) {
  case Method::GET:
    return "GET";
  case Method::POST:
    return "POST";
  case Method::PUT:
    return "PUT";
  case Method::DELETE:
    return "DELETE";
  case Method::HEAD:
    return "HEAD";
  case Method::OPTIONS:
    return "OPTIONS";
  case Method::TRACE:
    return "TRACE";
  case Method::CONNECT:
    return "CONNECT";
  }
  return "UNKNOWN";
}

inline std::string versionToString(HTTPVersion v) {
  switch (v) {
  case HTTPVersion::HTTP1x1:
    return "HTTP/1.1";
  }
  return "HTTP/1.1";
}

template <typename Derived> class HttpMessage {
public:
  std::string getStartLine() const {
    return static_cast<const Derived *>(this)->getStartLineImpl();
  }

  void addHeader(const std::string &name, const std::string &value) {
    headers_.push_back(Header(name, value));
  }
  void addHeader(const Header header) { headers_.push_back(header); }

  const std::vector<Header> &getHeaders() const { return headers_; }

  void setBody(const std::string &body) { body_ = body; }

  const std::optional<std::string> &getBody() const { return body_; }

  bool hasBody() const { return body_.has_value(); }

protected:
  std::vector<Header> headers_;
  std::optional<std::string> body_;
};

class HttpRequest : public HttpMessage<HttpRequest> {
public:
  HttpRequest() = default;
  HttpRequest(Method method, RequestURI uri, HTTPVersion version)
      : method_(method), request_uri_(uri), http_version_(version) {}

  std::string getStartLineImpl() const;

  Method getMethod() const { return method_; }

  void setMethod(Method method) { method_ = method; }

  const RequestURI &getRequestURI() const { return request_uri_; }
  void setRequestURI(const RequestURI &uri) { request_uri_ = uri; }

  HTTPVersion getHttpVersion() const { return http_version_; }
  void setHTTPVersion(HTTPVersion version) { http_version_ = version; }

private:
  Method method_;
  RequestURI request_uri_;
  HTTPVersion http_version_;
};

class HttpResponse : public HttpMessage<HttpResponse> {
public:
  HttpResponse() = default;

  HttpResponse(HTTPVersion version, int status_code, std::string reason_phrase)
      : http_version_(version), status_code_(status_code),
        reason_phrase_(std::move(reason_phrase)) {}

  std::string getStartLineImpl() const;

  HTTPVersion getHTTPVersion() const { return http_version_; }
  void setHTTPVersion(HTTPVersion version) { http_version_ = version; }

  int getStatusCode() const { return status_code_; }
  void setStatusCode(int code) { status_code_ = code; }

  const std::string &getReasonPhrase() const { return reason_phrase_; }
  void setReasonPhrase(const std::string &phrase) { reason_phrase_ = phrase; }

  std::string toString() const;

private:
  HTTPVersion http_version_;
  int status_code_;
  std::string reason_phrase_;
};

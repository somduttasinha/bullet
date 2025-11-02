#include "HttpParser.hpp"
#include "HttpMessage.hpp"
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

HttpParser::HttpParser() {}

HttpParser::~HttpParser() {}

HttpMessage HttpParser::parse(const std::string &raw) {
  state_ = State::START;
  pos_ = 0;
  message_ = HttpMessage{};

  std::string method_str, uri_str, version_str;

  state_ = State::PARSING_START_LINE;
  while (state_ != State::DONE && state_ != State::ERROR) {
    switch (state_) {
    case State::START: {
      state_ = State::PARSING_START_LINE;
      break;
    }
    case State::PARSING_START_LINE: {
      size_t start_line_size = raw.find(CRLF, pos_);
      if (start_line_size == std::string::npos) {
        state_ = State::ERROR;
        break;
      }
      auto start_line_raw = raw.substr(pos_, start_line_size - pos_);
      message_.start_line = parseStartLine(start_line_raw);
      pos_ = start_line_size + 2; // because CR LF is two characters
      state_ = State::PARSING_HEADERS;
      break;
    }
    case State::PARSING_HEADERS: {
      auto headers = std::vector<std::pair<std::string, std::string>>();
      // we need to keep going until we find two consecutive CRLFs
      while (true) {
        size_t header_line_size = raw.find(CRLF, pos_);
        std::string header_line_raw = raw.substr(pos_, header_line_size - pos_);
        if (header_line_raw.empty()) {
          // Skip past the empty line (final \r\n before body)
          pos_ = header_line_size + 2;
          break;
        }

        headers.push_back(parseHeaderLine(header_line_raw));
        pos_ = header_line_size + 2;
      }
      message_.headers = headers;
      state_ = State::PARSING_BODY;
      break;
    }
    case State::PARSING_BODY: {
      // The body is everything from pos_ to the end
      // Body format: <everything-after-headers>\r\n
      if (pos_ < raw.size()) {
        auto body = parseBody(raw.substr(pos_));
        message_.body = body;
      }
      state_ = State::DONE;
      break;
    }
    case State::DONE: {
      break;
    }
    case State::ERROR:
      throw std::runtime_error("Error occurred while parsing message");
    }
  }
  return message_;
}

/// Return either a Request-Line or a Status-Line
/// Request-Line for requests and Status-Line for responses
///
/// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
/// Request-Line  = Method SP Request-URI SP HTTP-Version CRLF
StartLine HttpParser::parseStartLine(const std::string &start_line_str) {

  std::string line = start_line_str.substr(0, start_line_str.size());

  size_t first_space = line.find(' ');
  size_t second_space = line.find(' ', first_space + 1);

  if (first_space == std::string::npos || second_space == std::string::npos) {
    throw std::runtime_error("Malformed start line");
  }

  std::string first_token = line.substr(0, first_space);
  std::string second_token =
      line.substr(first_space + 1, second_space - first_space - 1);
  std::string third_token = line.substr(second_space + 1);

  if (first_token.find("HTTP/") == 0) {
    HTTPVersion version = parseVersion(first_token);
    int status_code = std::stoi(second_token);
    std::string reason_phrase = third_token;

    StatusLine status_line{version, status_code, reason_phrase};
    return status_line;
  } else {
    Method method = parseMethod(first_token);
    std::string uri = second_token;
    HTTPVersion version = parseVersion(third_token);

    RequestLine request_line{method, uri, version};
    return request_line;
  }
};

Method HttpParser::parseMethod(const std::string &method_str) {
  if (method_str == "GET")
    return Method::GET;
  if (method_str == "POST")
    return Method::POST;
  if (method_str == "OPTIONS")
    return Method::OPTIONS;
  if (method_str == "HEAD")
    return Method::HEAD;
  if (method_str == "PUT")
    return Method::PUT;
  if (method_str == "DELETE")
    return Method::DELETE;
  if (method_str == "TRACE")
    return Method::TRACE;
  if (method_str == "CONNECT")
    return Method::CONNECT;
  throw std::runtime_error("Unknown method");
}

HTTPVersion HttpParser::parseVersion(const std::string &version_str) {
  if (version_str == "HTTP/1.1")
    return HTTPVersion::HTTP1x1;
  throw std::runtime_error("Unknown version");
}

/// message-header = field-name ":" [ field-value ]
/// field-name     = token
/// field-value    = *( field-content | LWS )
/// field-content  = <the OCTETs making up the field-value
///                 and consisting of either *TEXT or combinations
///                 of token, separators, and quoted-string>
/// LWS            = Linear White Space
/// LWS            = [CRLF] 1*( SP | HT )
/// SP             = <US-ASCII SP, space (32)>
/// HT             = <US-ASCII HT, horizontal-tab (9)>
std::pair<std::string, std::string>
HttpParser::parseHeaderLine(const std::string &line) {
  size_t colon_pos = line.find(':');

  if (colon_pos == std::string::npos) {
    throw std::runtime_error("Malformed header: no colon found");
  }

  std::string field_name = line.substr(0, colon_pos);
  size_t value_start = colon_pos + 1;

  while (value_start < line.size() &&
         (line[value_start] == ' ' || line[value_start] == '\t')) {
    value_start++;
  }

  std::string field_value = line.substr(value_start);

  while (!field_value.empty() &&
         (field_value.back() == ' ' || field_value.back() == '\t' ||
          field_value.back() == '\r')) {
    field_value.pop_back();
  }
  return {field_name, field_value};
}

std::optional<std::string> HttpParser::parseBody(const std::string &raw) {
  if (raw.empty())
    return std::nullopt;
  return raw;
}

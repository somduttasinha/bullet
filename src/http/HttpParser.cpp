#include "HttpParser.hpp"
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

HttpParser::HttpParser() {}

HttpParser::~HttpParser() {}

const std::unordered_map<std::string, std::string>
HttpParser::parse(const std::string &x) {
  std::vector<std::string> lines;

  std::unordered_map<std::string, std::string> headers;
  char delimeter = '\n';

  int start, end = -1;
  int last_header_line = -1;

  do {
    start = end + 1;
    end = x.find(delimeter, start); // get the entire line
    auto line = x.substr(start, end - start);
    lines.push_back(line);

    // int delim_pos = header.find(header_delim);

    // std::cout << "Header size is " << header.size() << std::endl;
    // std::cout << "Header is " << header << std::endl;
    // std::cout << "Delimeter position is " << delim_pos << std::endl;

    // std::string key = header.substr(0, delim_pos);
    // std::string val = header.substr(delim_pos, header.size() - delim_pos);

    // headers[key] = val;

  } while (end != -1);

  auto http_protocol = lines[0];

  const std::string header_delim = ": ";

  start = -1;
  end = -1;

  for (int i = 1; i < lines.size(); i++) {
    std::string header_line = lines[i];
    if (header_line.compare("\r") == 0) {
      break;
    }

    start = end + 1;
    end = header_line.find(header_delim);
    auto header_key = header_line.substr(start, end - start);
    start = end + 1;
    end = header_line.find("\n");
    auto header_val = header_line.substr(start + 1, end - start);

    headers[header_key] = header_val;
  }

  return headers;
}

# Bullet HTTP Server - Tasks

## High Priority

### 1. Fix HTTP Parser Bugs
- [x] Fix header parsing logic in `HttpParser.cpp:53` (incorrect `start = end + 1;` should be `start = 0;`)
- [ ] Handle `string::npos` case when newline not found in `HttpParser.cpp:57`
- [ ] Strip carriage returns (`\r`) from header values
- [ ] Remove unused variable `last_header_line` in `HttpParser.cpp:19`
- [ ] Remove commented-out code (lines 27-36)

### 2. HTTP Protocol Compliance
- [ ] Parse and validate HTTP method (GET, POST, PUT, DELETE, etc.)
- [ ] Parse and validate request URL/path
- [ ] Validate HTTP version from request line
- [ ] Implement proper HTTP status codes (200, 400, 404, 500, etc.)
- [ ] Add Host header validation (required for HTTP/1.1)
- [ ] Implement proper error responses

### 3. Request Body Handling
- [ ] Parse Content-Length header
- [ ] Read request body based on Content-Length
- [ ] Handle chunked transfer encoding
- [ ] Support POST/PUT request bodies

## Medium Priority

### 4. Connection Management
- [ ] Implement HTTP keep-alive support (Connection: keep-alive)
- [ ] Add timeout handling for slow/stalled clients
- [ ] Handle partial reads properly
- [ ] Implement graceful connection shutdown

### 5. Routing and Response System
- [ ] Create routing system for URL paths
- [ ] Implement dynamic response generation (not hardcoded)
- [ ] Add support for different response content types
- [ ] Implement static file serving

### 6. Error Handling and Robustness
- [ ] Add buffer overflow protection (handle requests > 1024 bytes)
- [ ] Implement proper error logging
- [ ] Add connection error handling
- [ ] Handle EAGAIN/EWOULDBLOCK properly for non-blocking I/O
- [ ] Add signal handling (SIGTERM, SIGINT for graceful shutdown)

### 7. Performance Improvements
- [ ] Make MAX_EVENTS configurable (currently hardcoded to 10)
- [ ] Implement connection pooling
- [ ] Add multi-threading or worker pool
- [ ] Optimize buffer management
- [ ] Add response caching for static content

## Low Priority

### 8. Code Quality
- [ ] Remove unused constants (`POOL_SIZE`, `GOODBYE_MESSAGE`, `WELCOME_MESSAGE`)
- [ ] Add comprehensive code comments
- [ ] Improve const correctness
- [ ] Add type safety (fix implicit conversions)
- [ ] Split main.cpp into separate modules (server, connection handler, etc.)

### 9. Logging and Monitoring
- [ ] Implement structured logging system
- [ ] Add request/response logging
- [ ] Add performance metrics (requests/sec, latency, etc.)
- [ ] Add debug logging for development

### 10. Configuration
- [ ] Make port configurable (currently hardcoded to 3232)
- [ ] Add configuration file support
- [ ] Make buffer size configurable
- [ ] Add command-line argument parsing

### 11. Testing
- [ ] Add unit tests for HTTP parser
- [ ] Add integration tests for request/response cycle
- [ ] Add load testing scripts
- [ ] Add edge case tests (malformed requests, large payloads, etc.)

### 12. Documentation
- [ ] Add API documentation
- [ ] Document architecture and design decisions
- [ ] Add examples of usage
- [ ] Document build and deployment process

## Future Enhancements

### 13. Advanced HTTP Features
- [ ] Implement HTTP/2 support
- [ ] Add WebSocket support
- [ ] Implement compression (gzip, deflate)
- [ ] Add SSL/TLS support (HTTPS)

### 14. Developer Experience
- [ ] Add hot reload during development
- [ ] Create middleware system
- [ ] Add request/response interceptors
- [ ] Create plugin architecture

## Bug Fixes

### Current Known Issues
1. **HTTP Parser** (`src/http/HttpParser.cpp`):
   - Line 53: Incorrect variable assignment in loop
   - Line 57: No check for `string::npos`
   - Headers may contain `\r` characters

2. **Main Server** (`src/main.cpp`):
   - Line 175: WELCOME_MESSAGE functionality commented out
   - No validation of read data
   - Always returns 200 OK regardless of request validity

## Notes

- Project uses epoll (Linux-specific) - consider portability
- Current implementation is single-threaded
- All connections use `Connection: close` (no keep-alive)
- Buffer size is 1024 bytes (stack-allocated)
- Server runs on port 3232

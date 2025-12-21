# Tiny HTTP Server in C++ (Windows)

A minimal HTTP/1.1 server implemented entirely from scratch in C++ using raw TCP sockets via Windows Winsock.  
This project intentionally avoids frameworks and third-party libraries to focus on core networking fundamentals and protocol behavior.

---

## Why This Project Exists

This project exists to deeply understand how an HTTP server actually works at the socket level on Windows.

Rather than abstracting networking behind libraries, it demonstrates:
- How TCP connections are established and terminated
- How HTTP is transmitted over raw byte streams
- How browsers behave when protocol expectations are not met
- How to correctly handle edge cases that are often hidden by frameworks

The goal is learning and correctness, not feature completeness or production readiness.

---

## What This Server Does

- Initializes the Windows Winsock subsystem
- Creates an IPv4 TCP socket
- Binds to port `8080`
- Listens for incoming connections
- Accepts a client connection
- Reads raw TCP data using `recv()` in a loop
- Detects HTTP request boundaries using `\r\n\r\n`
- Correctly handles all `recv()` return values:
  - `> 0` — data received
  - `== 0` — graceful client disconnect
  - `< 0` — socket error
- Parses the request path
- Sends valid HTTP/1.1 responses:
  - `200 OK` for `/` with HTML content
  - `404 Not Found` for all other paths
- Gracefully shuts down the connection using `shutdown()` to ensure browsers do not hang

---

## Socket Lifecycle

The server follows the classic blocking TCP socket lifecycle on Windows:

<img width="448" height="660" alt="image" src="https://github.com/user-attachments/assets/328a2a2b-8c1b-47e3-91ad-c94fd1940de5" />


---

## Core Concepts Demonstrated

- Windows Winsock API usage
- TCP connection establishment and teardown
- Blocking socket behavior
- Partial TCP reads and stream reassembly
- HTTP request boundary detection
- Minimal HTTP/1.1 response formatting
- Proper use of `shutdown()` vs `closesocket()`
- Real browser interoperability considerations

---

## Error Handling and Edge Cases

This project explicitly handles common real-world failure modes:

- Winsock initialization failure
- Socket creation failure
- Bind failure (e.g., port already in use)
- Accept failure
- Partial TCP reads across multiple `recv()` calls
- Graceful client disconnect (`recv() == 0`)
- Socket errors (`recv() < 0`)
- Browser hanging due to missing TCP FIN (explicitly fixed using `shutdown()`)

---

## How to Run

1. Build the project using a Windows C++ compiler (Visual Studio or equivalent).
2. Ensure no other service is using port `8080`.
3. Run the executable.
4. The server will block and wait for an incoming connection.

---

## How to Test

### Browser Test

Navigate to:

http://localhost:8080/

- `/` returns `200 OK` with HTML content
- Any other path returns `404 Not Found`

---

### curl Test

curl -v http://localhost:8080/

<img width="1063" height="473" alt="image" src="https://github.com/user-attachments/assets/df28fd9c-b509-461a-ae89-48e9573b2a40" />

Non-existent path:

curl -v http://localhost:8080/does-not-exist



---

## Intentional Limitations

- Single-connection, blocking design
- No concurrency or threading
- No HTTP header parsing beyond the minimum required
- No request body handling
- No HTTPS / TLS
- No production performance optimizations

These limitations are intentional to preserve clarity and focus on fundamentals.

---

## Possible Extensions

- Multi-client support (threads or `select()`)
- Robust HTTP header parsing
- Additional HTTP methods
- Keep-alive connections
- Logging and metrics
- IPv6 support

---

## Final Note

Understanding this project demonstrates practical knowledge of:

- TCP socket programming on Windows
- HTTP over raw TCP
- Correct connection termination semantics
- Browser behavior and protocol expectations
- What higher-level web frameworks abstract away

This is a foundational systems and networking exercise, not a production server.





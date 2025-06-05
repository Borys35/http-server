# Custom HTTP server in C++

Custom HTTP server written in C++ that handles HTTP GET requests for web pages withing static directory

## How does it work?

In `build` directory, we can focus on `http.exe` and `htdocs` directory. `http.exe` is http server. `htdocs` folder contains all static files that our http server can serve.
HTTP server listens on localhost:7120 for any GET requests. The server uses TCP to not lose any data during transporting.
This program was written for Linux systems.

### Currently supported files for /htdocs
- .html
- .css
- .js

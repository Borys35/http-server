# Custom HTTP server in C++

Custom HTTP server written in C++ that handles HTTP GET requests for web pages withing static directory

## How does it work?

In `build` directory, we can focus on `http.exe` and `htdocs` directory. `http.exe` is http server. `htdocs` folder contains all static files that our http server can serve.
HTTP server listens on localhost:7120 for any requests. The server uses TCP to not lose any data during transporting.
This program was written on WSL2 Ubuntu and some code only works for Linux system.

## How to build the project?
Assuming you are in main directory, in your terminal, enter:
```
1. cd build
2. cmake ..
3. make
4. ./http
```
Now your custom http server should be running and listening for requests.

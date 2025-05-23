#pragma once
#include <string>
#include <stdexcept>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    using socket_t = SOCKET;
    const socket_t INVALID_FD = INVALID_SOCKET;
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    using socket_t = int;
    const socket_t INVALID_FD = -1;
#endif

class SocketException : public std::runtime_error {
public:
    explicit SocketException(const std::string& msg) : std::runtime_error(msg) {}
};

class Socket {
protected:
    socket_t fd{INVALID_FD};
    explicit Socket(socket_t _fd) : fd(_fd) {}
public:
    Socket() = default;
    virtual ~Socket();
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    socket_t getFd() const { return fd; }
};

class SocketCliente : public Socket {
public:
    SocketCliente(const std::string& ip, uint16_t puerto);
    void enviar(const std::string& msg) const;
    std::string recibir(size_t maxBytes = 1024) const;
};

class SocketServidor : public Socket {
    int backlog;
public:
    explicit SocketServidor(uint16_t puerto, int _backlog = 10);
    socket_t aceptar() const;
};

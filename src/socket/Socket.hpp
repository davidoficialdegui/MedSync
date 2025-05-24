// Socket.hpp
#pragma once
#include <exception>
#include <string>
#include <cstdint>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  typedef SOCKET socket_t;
  static const socket_t INVALID_FD = INVALID_SOCKET;
#else
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  typedef int socket_t;
  static const socket_t INVALID_FD = -1;
#endif

class SocketException : public std::exception {
    std::string msg;
public:
    explicit SocketException(const std::string& s): msg(s) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

class Socket {
protected:
    socket_t fd;
    Socket();
public:
    virtual ~Socket();
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
    explicit SocketServidor(uint16_t puerto, int backlog = 5);
    socket_t aceptar() const;
};

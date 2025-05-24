// Socket.cpp
#include "Socket.hpp"
#include <cstring>
#include <cerrno>

static void _throw(const std::string& where) {
#ifdef _WIN32
    int err = WSAGetLastError();
    throw SocketException(where + " (WSA error " + std::to_string(err) + ")");
#else
    throw SocketException(where + ": " + std::strerror(errno));
#endif
}

Socket::Socket(): fd(INVALID_FD) {}

Socket::~Socket() {
    if (fd != INVALID_FD) {
#ifdef _WIN32
        closesocket(fd);
        WSACleanup();
#else
        close(fd);
#endif
    }
}

// -------------------- CLIENTE ----------------------

SocketCliente::SocketCliente(const std::string& ip, uint16_t puerto)
: Socket() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
        _throw("WSAStartup()");
#endif

    fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == INVALID_FD) _throw("socket()");

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(puerto);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (addr.sin_addr.s_addr == INADDR_NONE)
        _throw("inet_addr()");

    if (connect(fd, (sockaddr*)&addr, sizeof(addr)) < 0)
        _throw("connect()");
}

void SocketCliente::enviar(const std::string& msg) const {
    if (send(fd, msg.c_str(), static_cast<int>(msg.length()), 0) < 0)
        _throw("send()");
}

std::string SocketCliente::recibir(size_t maxBytes) const {
    std::string out(maxBytes, '\0');
#ifdef _WIN32
    int n = recv(fd, &out[0], static_cast<int>(maxBytes), 0);
#else
    ssize_t n = recv(fd, &out[0], maxBytes, 0);
#endif
    if (n < 0) _throw("recv()");
    out.resize(n);
    return out;
}

// -------------------- SERVIDOR ----------------------

SocketServidor::SocketServidor(uint16_t puerto, int _backlog)
: Socket(), backlog(_backlog) {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
        _throw("WSAStartup()");
#endif

    fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == INVALID_FD) _throw("socket()");

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(puerto);

    if (bind(fd, (sockaddr*)&addr, sizeof(addr)) < 0)
        _throw("bind()");
    if (listen(fd, backlog) < 0)
        _throw("listen()");
}

socket_t SocketServidor::aceptar() const {
    socket_t nuevo = accept(fd, nullptr, nullptr);
    if (nuevo == INVALID_FD) _throw("accept()");
    return nuevo;
}

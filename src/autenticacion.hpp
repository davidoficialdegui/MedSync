#ifndef MEDSYC_AUTENTICACION_HPP
#define MEDSYC_AUTENTICACION_HPP

#include <string>
#include "sqlite3.h"

namespace MedSyc {

class Autenticacion {
public:
    explicit Autenticacion(sqlite3* db);

    bool login(const std::string& user, const std::string& pass);
    void logout();
    bool estaAutenticado() const;

    int userId() const { return userId_; }
    const std::string& role() const { return role_; }

    std::string getUsername() const;  // <- Getter para logs
    void forzarLogin(const std::string& usuario);
    int obtenerIdDesdeUsuario(const std::string& usuario);
    std::string obtenerRolDesdeUsuario(const std::string& usuario);

private:
    sqlite3*    db_;
    bool        autenticado_;
    int         userId_;
    std::string role_;
    std::string username_;  // <- Guardar el nombre de usuario
};

} // namespace MedSyc

#endif

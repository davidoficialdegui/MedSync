#ifndef MEDSYC_ADMIN_HPP
#define MEDSYC_ADMIN_HPP

#include <string>
#include <vector>
#include <sqlite3.h>

namespace MedSyc {

struct AdminInfo {
    int id;
    std::string username;
};

class Admin {
public:
    explicit Admin(sqlite3* db);
    int crear(const std::string& username, const std::string& password);
    std::vector<AdminInfo> listar();
    bool autenticar(const std::string& username, const std::string& password);

private:
    sqlite3* db_;
};

} 

#endif 

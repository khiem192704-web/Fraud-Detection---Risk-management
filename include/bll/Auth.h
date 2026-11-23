#pragma once
#include <string>

struct AdminUser {
    std::string username{"admin"};
    std::string password{"admin123"};
    std::string fullName{"Super Administrator"};
    std::string role{"RISK_ADMIN"};
    std::string department{"Fraud Risk Operations"};
    std::string lastLogin{""};
};

class Auth {
private:
    AdminUser admin_user;
    bool is_authenticated{false};
    std::string current_session_user{""};

public:
    Auth();
    bool login(const std::string& username, const std::string& password);
    void logout();
    bool isAuthenticated() const;
    const AdminUser& getCurrentUser() const;
    std::string getSessionUsername() const;
    bool loadFromDisk(const std::string& path = "data/admin.txt");
    bool saveToDisk(const std::string& path = "data/admin.txt") const;
    void setAdminCredentials(const std::string&, const std::string&, const std::string&);
};

using AuthService = Auth;

#include "include/services/Auth.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace {
    std::string getCurrentTimeString() {
        auto now = std::chrono::system_clock::now();
        std::time_t in_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }
}

Auth::Auth() {
    loadFromDisk("data/admin.txt");
}

bool Auth::login(const std::string& username, const std::string& password) {
    if (trim(username) == admin_user.username && trim(password) == admin_user.password) {
        is_authenticated = true;
        current_session_user = admin_user.username;
        admin_user.lastLogin = getCurrentTimeString();
        return true;
    }
    return false;
}

void Auth::logout() {
    is_authenticated = false;
    current_session_user = "";
}

bool Auth::isAuthenticated() const {
    return is_authenticated;
}

const AdminUser& Auth::getCurrentUser() const {
    return admin_user;
}

std::string Auth::getSessionUsername() const {
    return current_session_user;
}

bool Auth::loadFromDisk(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        saveToDisk(path);
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string u, p, fn, dept;
        if (std::getline(ss, u, '|') && std::getline(ss, p, '|')) {
            admin_user.username = trim(u);
            admin_user.password = trim(p);
            if (std::getline(ss, fn, '|')) admin_user.fullName = trim(fn);
            if (std::getline(ss, dept, '|')) admin_user.department = trim(dept);
            return true;
        }
    }
    return false;
}

bool Auth::saveToDisk(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) return false;

    file << "# EPFD-RAS Admin Credentials\n";
    file << "# Format: username|pa ssword|fullName|department\n";
    file << admin_user.username << "|"
         << admin_user.password << "|"
         << admin_user.fullName << "|"
         << admin_user.department << "\n";
    return true;
}

void Auth::setAdminCredentials(const std::string& username, const std::string& password,
                                     const std::string& fullName) {
    admin_user.username = trim(username);
    admin_user.password = trim(password);
    admin_user.fullName = trim(fullName);
    saveToDisk("data/admin.txt");
}

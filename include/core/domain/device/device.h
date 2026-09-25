#pragma once
#include "core/domain/location/location.h"

class Device{
    private:
        std::string device_id;
        std::string ip_address;
        std::string device_fingerprint;
        Location location;
        bool is_virtual{false}; // máy ảo hoặc giả lập trên máy tính
        bool is_hacker{false};
    public:
        Device(){};
        Device(const std::string&, const std::string&, const std::string&, const Location&, bool, bool);

        void SetDevice_ID(const std::string&);
        void SetIP_Address(const std::string&);
        void SetDevice_Fingerprint(const std::string&);
        void SetLocation(const Location&);
        void SetIs_Virtual(bool);
        void SetIs_Hacker(bool);

        const std::string& GetDevice_ID() const;
        const std::string& GetIP_Address() const;
        const std::string& GetDevice_Fingerprint() const;
        const Location& GetLocation() const;
        bool GetIs_Virtual() const;
        bool GetIs_Hacker() const;

        bool is_Safe() const;
        std::string summary() const;
};
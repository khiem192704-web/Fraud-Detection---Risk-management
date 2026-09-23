#include"core/domain/device/device.h"

Device::Device(){}
Device::Device(const std::string& device_id, const std::string& ip_address, const std::string& device_fingerprint, const Location& location, bool is_virtual, bool is_hacker)
    : device_id(device_id), ip_address(ip_address), device_fingerprint(device_fingerprint), location(location), is_virtual(is_virtual), is_hacker(is_hacker) {}

void Device::SetDevice_ID(const std::string& Device_ID){ this->device_id = Device_ID; }
void Device::SetIP_Address(const std::string& IP_Address){ this->ip_address = IP_Address; }
void Device::SetDevice_Fingerprint(const std::string& Device_Fingerprint){ this->device_fingerprint = Device_Fingerprint; }
void Device::SetLocation(const Location& Location){ this->location = Location; }
void Device::SetIs_Virtual(bool Is_Virtual){ this->is_virtual = Is_Virtual; }
void Device::SetIs_Hacker(bool Is_Hacker){ this->is_hacker = Is_Hacker; }

std::string& Device::GetDevice_ID() const { return device_id; }
std::string& Device::GetIP_Address() const { return ip_address; }
std::string& Device::GetDevice_Fingerprint() const { return device_fingerprint; }
Location& Device::GetLocation() const { return location; }
bool Device::GetIs_Virtual() const { return is_virtual; }
bool Device::GetIs_Hacker() const { return is_hacker; }

bool Device::is_Safe() const{ return !is_hacker || !is_virtual; }
std::string Device::summary() const { return "Device ID: " + device_id + ", IP Address: " + ip_address + ", Is Virtual: " + (is_virtual ? "Yes" : "No") + ", Is Hacker: " + (is_hacker ? "Yes" : "No"); }
/* This file contains declarations for utility functions
 * Ramsey Nofal, 08/2023
 */


#ifndef UTIL_H
#define UTIL_H


//Standard C/C++
#include <fstream>
#include <iostream>
#include <filesystem>
#include <optional>


//Standard Qt
#include <QWidget>

//3rd-party
#include <nlohmann/json.hpp>
#include <curl/curl.h>

//Defines
#define API_KEY_FILE "api_key.txt"
#define GEO_INFO_FILE "geo_info.txt"
#define STYLE_FILE "style/style.qss"
#define WINDOW_ICON "icons/icon.png"
#define STATIC_ICON "icons/icon_globe.png"
#define MOVING_ICON "icons/icon_globe_loading.gif"


std::string get_geoloc_api_key();

nlohmann::json get_ip_geo_json_info(const std::string& ip_addr);

std::pair<float, float> get_ip_coords(const std::string& ip_addr);

//Return a string describing geographical information regarding the sniffed Packet
std::string get_packet_geo_info(const std::string& src, const std::string& dst);

std::string get_json_val_string(const nlohmann::json& json, const std::string& key);
float get_json_val_float(const nlohmann::json& json, const std::string& key);
std::string get_json_val(const std::vector<std::string>& nested_keys);


//Return json info in a formatted string
std::string parse_geo_info_json(const nlohmann::json& json);

struct delete_ptr { // Helper function to ease cleanup of container
    template <typename P>
    void operator () (P p) {
        delete p;
    }
};

#endif // UTIL_H

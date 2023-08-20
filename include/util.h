/* This file contains declarations for utility functions
 * Ramsey Nofal, 08/2023
 */


#ifndef UTIL_H
#define UTIL_H


//Standard C/C++
#include <fstream>
#include <iostream>
#include <filesystem>

//Standard Qt
#include <QWidget>

//3rd-party
#include <nlohmann/json.hpp>
#include <curl/curl.h>


//Defines
#define API_KEY_FILE "api_key.txt"
#define STYLE_FILE "style/style.json"
#define STATIC_ICON "icons/icon.png"
#define MOVING_ICON "icons/icon_rotate_cube.gif"


//ToDo: modify this to accomodate greater levels of JSON nesting (?) (with an array/list of keys(?))
//ToDo: consider removing this and incorporating its functionality into derived UI/GUI classes
void set_stylesheet_from_json(QWidget& widget, const std::string& sec_key, const std::string& sub_sec_key);

std::string get_geoloc_api_key();

nlohmann::json get_ip_geo_json_info(const std::string& ip_addr);

//Return a string describing geographical information regarding the sniffed Packet
std::string get_packet_geo_info(const std::string& src, const std::string& dst);

std::string get_json_val(const nlohmann::json& json, const std::string& key);

//Return json info in a formatted string
std::string parse_json(const nlohmann::json& json);

struct delete_ptr { // Helper function to ease cleanup of container
    template <typename P>
    void operator () (P p) {
        delete p;
    }
};

#endif // UTIL_H

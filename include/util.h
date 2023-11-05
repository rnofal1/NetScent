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

struct delete_ptr { // Helper function to ease cleanup of container
    template <typename P>
    void operator () (P p) {
        delete p;
    }
};

std::string get_geoloc_api_key();

nlohmann::json curl_get_json(const std::string& request);

std::string get_json_val_string(const nlohmann::json& json, const std::string& key);
float get_json_val_float(const nlohmann::json& json, const std::string& key);
std::string get_json_val(const std::vector<std::string>& nested_keys);

#endif // UTIL_H

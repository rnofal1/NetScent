/* This file contains declarations for utility functions
 * Ramsey Nofal, 08/2023
 */


#ifndef UTIL_H
#define UTIL_H


/* Standard C/C++ */
#include <fstream>
#include <iostream>
#include <filesystem>
#include <optional>
#include <filesystem>

/* Standard Qt */
#include <QWidget>
#include <QFuture>
#include <QString>
#include <QFile>
#include <QFileInfo>

/* 3rd-party */
#include <nlohmann/json.hpp>
#include <curl/curl.h>

/* Defines */
#define API_KEY_FILE_PATH "api_key.txt"
#define GEO_INFO_FILE_PATH "geo_info.txt"
#define STYLE_FILE_PATH "style/style.qss"
#define WINDOW_ICON "icons/icon.png"
#define STATIC_ICON "icons/icon_globe.png"
#define MOVING_ICON "icons/icon_globe_loading.gif"


//Helper function to ease cleanup of container
struct delete_ptr {
    template <typename P>
    void operator () (P p) {
        delete p;
    }
};

int thread_handler(QFuture<int>& future);

QString get_geoloc_api_key();

std::string curl_get_string(const QString& request);
nlohmann::json curl_get_json(const QString& request);

QString get_json_val_string(const nlohmann::json& json, const QString& key);
float get_json_val_float(const nlohmann::json& json, const QString& key);

QString get_my_public_ip();

QString get_cwd();

//mode = QFile::Append to add or mode = QFile::Truncate to erase then write
bool write_to_file(const QString& file_path, const QString& content, const QIODeviceBase::OpenModeFlag& mode);

QString read_from_file(const QString& file_path);

bool file_exists(const QString& file_path);

nlohmann::json get_ip_geo_json_info(const QString& ip_addr);
std::pair<double, double> get_ip_coords(const QString& ip_addr);

//ASTAR
std::pair<double, double> get_my_lat_lon();

#endif // UTIL_H

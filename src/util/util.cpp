/* This file contains definitions for the utility functions declared
 * in util.h
 * Ramsey Nofal, 08/2023
 */


/* Local */
#include "util.h"


/* Miscellaneous helper functions
 *
 * ToDo: Try to incorporate into a class whenever appropriate;
 * (almost) all of these functions should exist within a class eventually
 */


//Callback function used during HTTP GET request (curl)
static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

int thread_handler(QFuture<int>& future) {
    qDebug() << "In thread handler";

    future.waitForFinished();

    if(future.isValid()) {
        qDebug() << "Future thread has valid result";
        return future.result();
    }

    qDebug() << "Future thread has invalid result";
    return 1;
}

QString get_geoloc_api_key() {
    return read_from_file(QString::fromStdString(API_KEY_FILE_PATH));
}

std::string curl_get_string(const QString& request) {
    auto curl = curl_easy_init();
    if(!curl) {
        qDebug() << "Could not initialize curl";
        return "";
    }

    std::string request_string = request.toStdString();
    std::string response_string;

    //Setup curl call
    curl_easy_setopt(curl, CURLOPT_URL, request_string.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    //Execute curl call
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return response_string;
}

nlohmann::json curl_get_json(const QString& request) {
    nlohmann::json ret_json = nlohmann::json::parse(curl_get_string(request));
    return ret_json;
}

QString get_json_val_string(const nlohmann::json& json, const QString& key) {
    std::string key_string = key.toStdString();
    if (json.contains(key_string)) {
        return QString::fromStdString(std::string(json[key_string]));
    } else {
        qDebug() << "Unknown json value in get_json_val_string(): " << key;
        return "Unknown";
    }
}
float get_json_val_float(const nlohmann::json& json, const QString& key) {
    std::string key_string = key.toStdString();
    if(json.contains(key_string)) {
        std::string coord_str = json[key_string];
        float coord_f = std::stod(coord_str);
        return coord_f;
    } else {
        qDebug() << "Unknown json value in get_json_val_float(): " << key;
        return 0.0; // ToDo: better handling of unknown float
    }
}

QString get_my_public_ip() {
    QString ip_request = "http://myexternalip.com/raw";
    QString public_ip = QString::fromStdString(curl_get_string(ip_request));
    qDebug() << "Public ip: " << public_ip;
    return public_ip;
}

QString get_cwd() {
    std::filesystem::path cwd = std::filesystem::current_path();
    return QString::fromStdString(std::string(cwd.u8string())); //ToDo: too many conversions
}

//mode = QFile::Append to add or mode = QFile::Truncate to erase then write
bool write_to_file(const QString& file_path, const QString& content, const QIODeviceBase::OpenModeFlag& flag) {
    //Set mode
    QIODeviceBase::OpenMode mode = QFile::WriteOnly | flag;

    //Open file
    QFile file(file_path);
    if(!file.open(mode)) {
        qDebug() << "write_to_file() error, could not write file: " << file_path;
        return false;
    }

    //Write to file
    QTextStream file_stream(&file);
    file_stream << content;

    //Close file
    file.close();

    return true;
}

QString read_from_file(const QString& file_path) {
    //Open file
    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "write_to_file() error, could not read file: " << file_path;
        return "";
    }

    //Read from file
    QString ret_string = "";
    QTextStream file_stream(&file);
    while (!file_stream.atEnd()) {
        ret_string += file_stream.readLine();
    }

    //Close file
    file.close();

    return ret_string;
}

bool file_exists(const QString& file_path) {
    QFileInfo file(file_path);

    if (file.exists() && file.isFile()) {
        return true;
    } else {
        return false;
    }
}

// ToDo: redundancy with the function in Packet
nlohmann::json get_ip_geo_json_info(const QString& ip_addr) {
    QString key = get_geoloc_api_key();
    QString get_url = "https://api.ipgeolocation.io/ipgeo?apiKey=" + key + "&ip=" + ip_addr;
    return curl_get_json(get_url);
}

// ToDo: redundancy with the function in Packet
std::pair<double, double> get_ip_coords(const QString& ip_addr) {
    nlohmann::json loc_json = get_ip_geo_json_info(ip_addr);
    bool known_location = loc_json.contains("latitude"); //We known that if latitude exists, so will longitude
    double lati = 0.0;
    double longi = 0.0;

    if(known_location) {
        lati = get_json_val_float(loc_json, "latitude");
        longi = get_json_val_float(loc_json, "longitude");
    } else {
        qDebug() << "Unknown location for ip: " << ip_addr;
    }

    return {lati, longi};
}

//ASTAR
std::pair<double, double> get_my_lat_lon() {
    QString my_public_ip = get_my_public_ip();
    auto my_ip_coords = get_ip_coords(my_public_ip);
    qDebug() << "My latitude: " << my_ip_coords.first << " My longitude: " << my_ip_coords.second;
    return my_ip_coords;
}

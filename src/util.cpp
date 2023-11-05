/* This file contains definitions for the utility functions declared
 * in util.h
 * Ramsey Nofal, 08/2023
 */


//Local
#include "util.h"

//ToDo: miscellaneous helper functions; try to incorporate into a class whenever appropriate
//ToDo: ideally all of these functions will be part of a class eventually

//Callback function used during HTTP GET request (curl)
static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

std::string get_geoloc_api_key() {
    std::ifstream api_key_file(API_KEY_FILE);
    std::string ret_string;

    if(api_key_file) {
        std::getline(api_key_file, ret_string);
    } else {
        std::cout << "Geolocation API key not found\n";
    }
    api_key_file.close();

    return ret_string;
}

nlohmann::json curl_get_json(const std::string& request) {
    auto curl = curl_easy_init();
    nlohmann::json ret_json;
    if(!curl) {
        qInfo() << "Could not initialize curl\n";
        return ret_json;
    }


    std::string response_string;

    //Setup curl call
    curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    //Execute curl call
    curl_easy_perform(curl);

    ret_json = nlohmann::json::parse(response_string);

    curl_easy_cleanup(curl);

    return ret_json;
}

std::string get_json_val_string(const nlohmann::json& json, const std::string& key) {
    if (json.contains(key)) {
        return std::string(json[key]);
    } else {
        //qInfo() << "Unknown json value\n";
        return "Unknown";
    }
}
float get_json_val_float(const nlohmann::json& json, const std::string& key) {
    if(json.contains(key)) {
        std::string coord_str = json[key];
        float coord_f = std::stod(coord_str);
        return coord_f;
    } else {
        //qInfo() << "Unknown json value\n";
        return 0.0; // ToDo: better handling of unknown float
    }
}

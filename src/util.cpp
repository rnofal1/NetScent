/* This file contains definitions for the utility functions declared
 * in util.h
 * Ramsey Nofal, 08/2023
 */


//Local
#include "util.h"


//ToDo: this is pretty inefficient (and ugly); find a different way to do this
void set_stylesheet_from_json(QWidget& widget, const std::string& sec_key, const std::string& sub_sec_key) {
    std::ifstream file(STYLE_FILE);
    if(file) {
        nlohmann::json style_json = nlohmann::json::parse(file);
        if(style_json.contains(sec_key) && style_json[sec_key].contains(sub_sec_key)) {
            std::string style_sheet_std;
            for(auto& [key, val] : style_json[sec_key][sub_sec_key].items()) {
                style_sheet_std.append(key).append(" : ").append(val).append("; ");
            }
            QString style_sheet = QString::fromStdString(style_sheet_std);
            widget.setStyleSheet(style_sheet);
        } else {
            std::cout << "Invalid JSON section + subsection: " << sec_key << " + " << sub_sec_key << "\n";
        }
        file.close();
    } else {
        std::cout << "Style file not found; Dynamic style elements will not work\n";
    }
}

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

/* get_ip_geo_json_info() uses curl to make an HTTP GET request to the
 * appropriate ip geolocation API
 *
 * This request requires a valid ip geolocation API key and takes the desired
 * ip address as a parameter
 *
 * ToDo: implement local ip location caching (for recently-inspected locations)
 * and/or switch to a local ip location database
 */
nlohmann::json get_ip_geo_json_info(const std::string& ip_addr) {
    auto curl = curl_easy_init();
    auto key = get_geoloc_api_key();

    if(curl && !key.empty()) {
        std::string response_string;
        std::string get_url = "https://api.ipgeolocation.io/ipgeo?apiKey=" + key + "&ip="+ip_addr;

        //Setup curl call
        curl_easy_setopt(curl, CURLOPT_URL, get_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        //Execute curl call
        curl_easy_perform(curl);

        nlohmann::json json = nlohmann::json::parse(response_string);

        curl_easy_cleanup(curl);
        curl = NULL;

        return json;
    } else if(!curl) {
        std::cout << "Could not initialize curl\n";
    }

    return NULL;
}

//ToDo: split this into two functions, one for src and one for dst
std::string get_packet_geo_info(const std::string& src, const std::string& dst) {
    nlohmann::json src_json = get_ip_geo_json_info(src);
    nlohmann::json dst_json = get_ip_geo_json_info(dst);

    std::string src_info =  "Source Geographical Info:\n" + parse_json(src_json);
    std::string dst_info =  "Destination Geographical Info:\n" + parse_json(dst_json);

    return src_info + dst_info;
}

//ToDo: consider having this function take in a vector of desired json keys
std::string parse_json(const nlohmann::json& json) {
    std::string json_info = "IP: " +                  get_json_val(json, "ip")
                            + "\nCountry: " +         get_json_val(json, "country_name")
                            + "\nState/Province: " +  get_json_val(json, "state_prov")
                            + "\nCity: " +            get_json_val(json, "city")
                            + "\nOrganization: " +    get_json_val(json, "organization") + "\n\n";
    return json_info;
}

std::string get_json_val(const nlohmann::json& json, const std::string& key) {
    if (json.contains(key)) {
        return std::string(json[key]);
    } else {
        return "Unknown";
    }
}

/* This file contains definitions for the utility functions declared
 * in util.h
 * Ramsey Nofal, 08/2023
 */


//Local
#include "util.h"

//ToDo: this whole file is a bit of a mess

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

// ToDo: handling of unknown coords
std::pair<float, float> get_ip_coords(const std::string& ip_addr) {
    nlohmann::json loc_json = get_ip_geo_json_info(ip_addr);
    float lati = get_json_val_float(loc_json, "latitude");
    float longi = get_json_val_float(loc_json, "longitude");
    return std::make_pair(lati, longi);
}

//ToDo: split this into two functions, one for src and one for dst (?)
std::string get_packet_geo_info(const std::string& src, const std::string& dst) {
    nlohmann::json src_json = get_ip_geo_json_info(src);
    nlohmann::json dst_json = get_ip_geo_json_info(dst);

    std::string src_info =  "Source Geographical Info:\n" + parse_geo_info_json(src_json);
    std::string dst_info =  "Destination Geographical Info:\n" + parse_geo_info_json(dst_json);

    return src_info + dst_info;
}

//ToDo: consider having this function take in a vector of desired json keys
//ToDo: rename this function
std::string parse_geo_info_json(const nlohmann::json& json) {
    std::string json_info = "IP: " +                  get_json_val_string(json, "ip")
                            + "\nCountry: " +         get_json_val_string(json, "country_name")
                            + "\nState/Province: " +  get_json_val_string(json, "state_prov")
                            + "\nCity: " +            get_json_val_string(json, "city")
                            + "\nOrganization: " +    get_json_val_string(json, "organization") + "\n\n";
    return json_info;
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

std::string get_json_val(const std::vector<std::string>& nested_keys) {
    std::ifstream file(STYLE_FILE);

    if(file) {
        nlohmann::json json = nlohmann::json::parse(file);
        for(auto& key : nested_keys) {
            if(!json.contains(key)) {
                std::cout << "Invalid JSON section: " << key << "\n";
            }
            json = json[key]; //inserts a null value if key not present
        }
        file.close();
        if(!json.is_null()) {
            return std::string(json);
        }
    } else {
        std::cout << "Style file not found; Dynamic style elements will not work\n";
    }

    return "";
}

//std::map<std::string, std::string> load_geo_info_from_file() {
//    std::map<std::string, std::string> geo_info_map;

//    std::fstream geo_info_file(GEO_INFO_FILE, std::fstream::in | std::fstream::app);
//    if(geo_info_file) {
//        boost::archive::text_iarchive iarch(geo_info_file);
//        iarch >> geo_info_map;
//        geo_info_file.close();
//    } else {
//        std::cout << "API Geo Info file read error\n";
//    }
//    return geo_info_map;
//}

//void save_geo_info_to_file(std::map<std::string, std::string>& geo_info_map) {
//    std::fstream geo_info_file(GEO_INFO_FILE, std::fstream::out | std::fstream::trunc);
//    if(geo_info_file) {
//        boost::archive::text_oarchive oarch(geo_info_file);
//        oarch << geo_info_map;
//        geo_info_file.close();
//    } else {
//        std::cout << "API Geo Info file write error" << std::endl;
//    }
//}

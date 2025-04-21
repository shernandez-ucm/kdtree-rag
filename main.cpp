#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <json/json.h>  // JSON library (e.g. libjsoncpp-dev)

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append((char*)contents, totalSize);
    return totalSize;
}

std::string send_embedding_request(const std::string& text) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        std::string json_payload = "{\"content\": \"" + text + "\", \"embedding_only\": true}";
        
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/embedding");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return response;
}

void extract_embedding(const std::string& json_str) {
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::istringstream stream(json_str);
    std::string errs;

    if (Json::parseFromStream(builder, stream, &root, &errs)) {
        if (root.isMember("embedding")) {
            const Json::Value& embedding = root["embedding"];
            std::cout << "Embedding (dim = " << embedding.size() << "): [";
            for (unsigned int i = 0; i < embedding.size(); ++i) {
                std::cout << embedding[i].asFloat();
                if (i != embedding.size() - 1) std::cout << ", ";
            }
            std::cout << "]" << std::endl;
        } else {
            std::cerr << "No 'embedding' field found in response." << std::endl;
        }
    } else {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <text_file.txt>" << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        std::cout << "Input: " << line << std::endl;
        std::string response = send_embedding_request(line);
        //extract_embedding(response);
        std::cout << std::endl;
    }

    return 0;
}


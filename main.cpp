#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <json/json.h>  // JSON library (e.g. libjsoncpp-dev)
#include <Eigen/Dense>

using Point = Eigen::VectorXd;
using Neighbor = std::pair<double, Point>; // {distancia, punto}

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

void extract_embedding(const std::string& json_str,std::vector<Point>& database) {
    Json::Value jsonData;
    Json::Reader jsonReader;
    if (jsonReader.parse(json_str, jsonData)){
        auto tokens = jsonData[0]["embedding"];
        std::cout << "# Tokens  = " << tokens.size() << std::endl;
        for (unsigned int i = 0; i < tokens.size(); ++i) {
            auto embedding = tokens[i];
            std::cout << "# Embeddings  = " << embedding.size() << std::endl;
            Point embedding_vector(embedding.size());
            for (unsigned int j = 0; j < embedding.size(); ++j) {
                embedding_vector(i) = embedding[i].asFloat();
            }
            database.push_back(embedding_vector);        
        }
    }
    else{
        std::cout << "Could not parse HTTP data as JSON" << std::endl;
    }
}

int find_nearest_neighbor(std::vector<Point>& user_prompt,std::vector<Point>& database){
    double min_dist=std::numeric_limits<double>::max();
    int min_index=0;
    for (unsigned int i = 0; i < user_prompt.size(); ++i) {
        for (unsigned int j = 0; j < database.size(); ++j) {
            double dist=(user_prompt[i]-database[j]).squaredNorm();
            std::cout << "i : "<< i <<", j: "<< j << ", dist: " << dist << ", min index: " << min_index << std::endl;
            if (dist<min_dist){
                min_dist=dist;
                min_index=j;
            }
        }
    }
    return min_index;
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
    std::vector<Point> vector_data;
    std::vector<Point> user_data;
    std::vector<std::string> string_data;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        std::cout << "Input: " << line << std::endl;
        std::string response = send_embedding_request(line);
        string_data.push_back(line);
        extract_embedding(response,vector_data);
        std::cout << std::endl;
    }
    std::cout << "Database size : "<< vector_data.size() << std::endl;
    
    while (true) {
        // get user input
        printf("\033[32m> \033[0m");
        std::string user;
        std::getline(std::cin, user);
        std::string response = send_embedding_request(user);
        extract_embedding(response,user_data);
        std::cout << user_data.size() << std::endl;
        int nn_index=find_nearest_neighbor(user_data,vector_data);
        user_data.clear();
        std::cout << string_data[nn_index] << std::endl;
        if (user.empty()) {
            break;
        }

        
    }

    return 0;
}


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <json/json.h>  // JSON library (e.g. libjsoncpp-dev)
#include <Eigen/Dense>
#include "llama_client.h"
#include <chrono> 

using namespace std::chrono;

struct WikiHowArticle {
    std::string Question;
    std::string Answer;
};

// Función para leer una línea CSV respetando las comas entre comillas
WikiHowArticle parseJSONArray(const std::string& line) {
    std::vector<std::string> result;
    Json::CharReaderBuilder readerBuilder;
    Json::Value root;
    std::string errs;
    std::istringstream iss(line);
    if (!Json::parseFromStream(readerBuilder, iss, &root, &errs)) {
        std::cerr << "Error parsing JSON: " << errs << std::endl;
    }
    WikiHowArticle article = {
        root[0].asString(),
        root[1].asString()
    };
    return article;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <text_file.jsonl> num_rows" <<  std::endl;
        return 1;
    }
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }
    int n_rows=atoi(argv[2]);
    std::string line;
    std::vector<Point> vector_data;
    std::vector<Point> user_data;
    std::vector<std::string> string_data;
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty() || count>n_rows) continue;
        WikiHowArticle article = parseJSONArray(line);
        //std::cout << count << ", Pregunta: " << article.Question << std::endl;
        //std::cout << "Respuesta: " << article.Answer << std::endl;
        std::string response = send_embedding_request(article.Answer);
        string_data.push_back(article.Answer);
        extract_embedding(response,vector_data);
        std::string question = send_embedding_request(article.Question);
        extract_embedding(question,user_data);
        ++count;
    }
    file.close();
    auto start1=high_resolution_clock::now();
    std::vector<int> nn_index = find_nearest_neighbor(user_data,vector_data);
    auto end1=high_resolution_clock::now();
    auto search_time=duration_cast<milliseconds>(end1-start1).count();
    std::cout << "Tiempo :  " << search_time << "[ms]" << std::endl;
    return 0;
}

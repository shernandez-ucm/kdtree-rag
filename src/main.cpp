#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "llama_client.h"


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
        std::vector<int> nn_index=find_nearest_neighbor(user_data,vector_data);
        user_data.clear();
        std::cout << string_data[nn_index[0]] << std::endl;
        if (user.empty()) {
            break;
        }
    }

    return 0;
}


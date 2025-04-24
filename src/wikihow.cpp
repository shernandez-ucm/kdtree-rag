#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct WikiHowArticle {
    std::string Title;
    std::string Headline;
    std::string Text;
};

// Función para leer una línea CSV respetando las comas entre comillas
std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string field;
    bool inQuotes = false;
    char c;
    std::string value;
    std::cout << line << std::endl;
    for (size_t i = 0; i < line.size(); ++i) {
        c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            result.push_back(value);
            value.clear();
        } else {
            value += c;
        }
    }
    result.push_back(value);
    return result;
}

int main() {
    std::ifstream file("wikihowAll.csv");
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo wikihow.csv" << std::endl;
        return 1;
    }

    std::string line;
    std::getline(file, line); // Leer encabezado

    int count = 0;
    while (std::getline(file, line) && count < 5) {
        std::vector<std::string> fields = parseCSVLine(line);
        if (fields.size() < 6) continue; // Validar campos

        WikiHowArticle article = {
            fields[0], fields[1], fields[2]
        };

        std::cout << "Título: " << article.Title << std::endl;
        std::cout << "Resumen: " << article.Headline << std::endl;
        std::cout << "Pasos: " << article.Text << std::endl;
        ++count;
    }

    file.close();
    return 0;
}

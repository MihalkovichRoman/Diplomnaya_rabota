#include "ConverterJSON.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

std::vector<std::string> ConverterJSON::getTextDocuments() const {
    std::ifstream file("config.json");
    if (!file.is_open()) {
        throw std::runtime_error("config file is missing");
    }

    json config;
    file >> config;

    if (config.empty() || !config.contains("config") || !config.contains("files")) {
        throw std::runtime_error("config file is empty");
    }

    // Проверка версии
    if (!config["config"].contains("version") || config["config"]["version"] != "0.1") {
        throw std::runtime_error("config.json has incorrect file version");
    }

    std::vector<std::string> docs;
    for (const auto& doc_path : config["files"]) {
        std::ifstream d(doc_path.get<std::string>());
        if (!d.is_open()) {
            std::cerr << "Warning: file not found: " << doc_path.get<std::string>() << "\n";
            continue; // пропускаем файл, выполнение программы не останавливаем
        }
        std::string content((std::istreambuf_iterator<char>(d)), std::istreambuf_iterator<char>());
        docs.push_back(content);
    }

    return docs;
}

int ConverterJSON::getResponsesLimit() const {
    std::ifstream file("config.json");
    if (!file.is_open()) {
        throw std::runtime_error("config file is missing");
    }

    json config;
    file >> config;

    if (!config.contains("config") || !config["config"].contains("max_responses")) {
        return 5; // значение по умолчанию
    }

    return config["config"]["max_responses"].get<int>();
}

std::vector<std::string> ConverterJSON::getRequests() const {
    std::ifstream file("requests.json");
    if (!file.is_open()) {
        throw std::runtime_error("requests.json file is missing");
    }

    json requests;
    file >> requests;

    if (!requests.contains("requests")) {
        return {};
    }

    return requests["requests"].get<std::vector<std::string>>();
}

void ConverterJSON::putAnswers(const std::vector<std::vector<RelativeIndex>>& answers) {
    json result;

    for (size_t i = 0; i < answers.size(); i++) {
        std::string request_id = std::string("request") + (i < 9 ? "00" : i < 99 ? "0" : "") + std::to_string(i + 1);

        if (answers[i].empty()) {
            result["answers"][request_id]["result"] = false;
        }
        else {
            json relevance = json::array();
            for (const auto& ans : answers[i]) {
                relevance.push_back({ {"docid", static_cast<int>(ans.doc_id)}, {"rank", ans.rank} });
            }

            result["answers"][request_id]["result"] = true;
            result["answers"][request_id]["relevance"] = relevance;
        }
    }

    std::ofstream file("answers.json");
    file << result.dump(4);
}

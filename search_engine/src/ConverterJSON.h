#pragma once

#include <string>
#include <vector>
#include "SearchServer.h"

// Подключение nlohmann/json.hpp из external/nlohmann
#include "nlohmann/json.hpp"

/**
 * Класс для работы с JSON-файлами:
 * - считывает конфигурацию (config.json)
 * - получает поисковые запросы (requests.json)
 * - формирует ответы (answers.json)
 */
class ConverterJSON {
public:
    ConverterJSON() = default;

    /** Считывает содержимое документов из config.json */
    std::vector<std::string> getTextDocuments() const;

    /** Считывает максимальное количество ответов на один запрос */
    int getResponsesLimit() const;

    /** Получает поисковые запросы из requests.json */
    std::vector<std::string> getRequests() const;

    /** Записывает результаты поиска в answers.json */
    void putAnswers(const std::vector<std::vector<RelativeIndex>>& answers);
};

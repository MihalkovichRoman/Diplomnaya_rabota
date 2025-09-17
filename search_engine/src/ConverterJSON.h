#pragma once

#include <string>
#include <vector>
#include "SearchServer.h"

// ����������� nlohmann/json.hpp �� external/nlohmann
#include "nlohmann/json.hpp"

/**
 * ����� ��� ������ � JSON-�������:
 * - ��������� ������������ (config.json)
 * - �������� ��������� ������� (requests.json)
 * - ��������� ������ (answers.json)
 */
class ConverterJSON {
public:
    ConverterJSON() = default;

    /** ��������� ���������� ���������� �� config.json */
    std::vector<std::string> getTextDocuments() const;

    /** ��������� ������������ ���������� ������� �� ���� ������ */
    int getResponsesLimit() const;

    /** �������� ��������� ������� �� requests.json */
    std::vector<std::string> getRequests() const;

    /** ���������� ���������� ������ � answers.json */
    void putAnswers(const std::vector<std::vector<RelativeIndex>>& answers);
};

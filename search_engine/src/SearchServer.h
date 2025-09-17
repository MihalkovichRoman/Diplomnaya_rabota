#pragma once
#include "InvertedIndex.h"
#include <vector>
#include <string>
#include <algorithm>

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex& other) const {
        return doc_id == other.doc_id && std::abs(rank - other.rank) < 1e-6;
    }
};

class SearchServer {
public:
    explicit SearchServer(InvertedIndex& idx) : _index(idx) {}

    /**
     * Обработка поисковых запросов
     * @param queries_input запросы из requests.json
     * @return список релевантных ответов
     */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& _index;
};

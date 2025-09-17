#include "SearchServer.h"
#include <sstream>
#include <map>
#include <set>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> results;

    for (const auto& query : queries_input) {
        std::stringstream ss(query);
        std::string word;
        std::map<std::string, size_t> word_freq;

        // Разделяем запрос на слова
        while (ss >> word) {
            word_freq[word] = _index.GetWordCount(word).size();
        }

        // Сортировка слов по возрастанию частоты (редкие слова первыми)
        std::vector<std::string> sorted_words;
        for (const auto& [w, _] : word_freq) sorted_words.push_back(w);

        std::sort(sorted_words.begin(), sorted_words.end(), [&](const std::string& a, const std::string& b) {
            return word_freq[a] < word_freq[b];
            });

        // Множество документов для поиска
        std::set<size_t> candidate_docs;
        bool first_word = true;

        for (const auto& w : sorted_words) {
            auto entries = _index.GetWordCount(w);
            std::set<size_t> docs_for_word;
            for (const auto& e : entries) docs_for_word.insert(e.doc_id);

            if (first_word) {
                candidate_docs = docs_for_word;
                first_word = false;
            }
            else {
                std::set<size_t> intersect;
                std::set_intersection(candidate_docs.begin(), candidate_docs.end(),
                    docs_for_word.begin(), docs_for_word.end(),
                    std::inserter(intersect, intersect.begin()));
                candidate_docs = intersect;
            }
        }

        // Если нет документов — результат пустой
        if (candidate_docs.empty()) {
            results.push_back({});
            continue;
        }

        // Рассчитываем абсолютную релевантность
        std::map<size_t, float> abs_rank;
        for (const auto& doc_id : candidate_docs) {
            float sum = 0;
            for (const auto& w : sorted_words) {
                for (const auto& e : _index.GetWordCount(w)) {
                    if (e.doc_id == doc_id) sum += static_cast<float>(e.count);
                }
            }
            abs_rank[doc_id] = sum;
        }

        // Находим максимальную абсолютную релевантность
        float max_rank = 0.0f;
        for (const auto& [_, rank] : abs_rank) {
            if (rank > max_rank) max_rank = rank;
        }

        // Формируем результаты с относительной релевантностью
        std::vector<RelativeIndex> rel;
        for (const auto& [doc_id, rank] : abs_rank) {
            rel.push_back({ doc_id, rank / max_rank });
        }

        // Сортировка по убыванию релевантности
        std::sort(rel.begin(), rel.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
            return a.rank > b.rank;
            });

        results.push_back(rel);
    }

    return results;
}

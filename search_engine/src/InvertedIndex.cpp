#include "InvertedIndex.h"
#include <sstream>

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    docs = input_docs;
    freq_dictionary.clear();

    for (size_t doc_id = 0; doc_id < docs.size(); ++doc_id) {
        std::stringstream ss(docs[doc_id]);
        std::string word;
        std::map<std::string, size_t> local_count;

        // Подсчет слов в текущем документе
        while (ss >> word) {
            local_count[word]++;
        }

        // Обновление глобального словаря
        for (const auto& [w, count] : local_count) {
            freq_dictionary[w].push_back({ doc_id, count });
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) const {
    if (freq_dictionary.count(word)) {
        return freq_dictionary.at(word);
    }
    return {};
}

#pragma once
#include <string>
#include <vector>
#include <map>

struct Entry {
    size_t doc_id;
    size_t count;

    // ƒл€ корректной работы тестов GTest
    bool operator==(const Entry& other) const {
        return doc_id == other.doc_id && count == other.count;
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    /**
     * ќбновить или заполнить базу документов
     * @param input_docs содержимое документов
     */
    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    /**
     * ћетод определ€ет количество вхождений слова в загруженной базе документов
     * @param word слово
     * @return вектор Entry с doc_id и count
     */
    std::vector<Entry> GetWordCount(const std::string& word) const;

private:
    std::vector<std::string> docs; // тексты документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};

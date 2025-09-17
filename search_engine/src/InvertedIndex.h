#pragma once
#include <string>
#include <vector>
#include <map>

struct Entry {
    size_t doc_id;
    size_t count;

    // ��� ���������� ������ ������ GTest
    bool operator==(const Entry& other) const {
        return doc_id == other.doc_id && count == other.count;
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    /**
     * �������� ��� ��������� ���� ����������
     * @param input_docs ���������� ����������
     */
    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    /**
     * ����� ���������� ���������� ��������� ����� � ����������� ���� ����������
     * @param word �����
     * @return ������ Entry � doc_id � count
     */
    std::vector<Entry> GetWordCount(const std::string& word) const;

private:
    std::vector<std::string> docs; // ������ ����������
    std::map<std::string, std::vector<Entry>> freq_dictionary; // ��������� �������
};

#include "gtest/gtest.h"
#include "ConverterJSON.h"
#include <fstream>

class ConverterJSONTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаём временные файлы для теста
        std::ofstream config("config.json");
        config << R"({
            "config": { "name": "TestEngine", "version": "0.1", "max_responses": 5 },
            "files": ["file1.txt", "file2.txt"]
        })";
        config.close();

        std::ofstream file1("file1.txt");
        file1 << "hello world";
        file1.close();

        std::ofstream file2("file2.txt");
        file2 << "hello gpt";
        file2.close();

        std::ofstream requests("requests.json");
        requests << R"({ "requests": ["hello", "world"] })";
        requests.close();
    }

    void TearDown() override {
        std::remove("config.json");
        std::remove("file1.txt");
        std::remove("file2.txt");
        std::remove("requests.json");
        std::remove("answers.json");
    }
};

TEST_F(ConverterJSONTest, ReadConfigAndFiles) {
    ConverterJSON converter;
    auto docs = converter.getTextDocuments();
    EXPECT_EQ(docs.size(), 2);
    EXPECT_EQ(docs[0], "hello world");
    EXPECT_EQ(docs[1], "hello gpt");
}

TEST_F(ConverterJSONTest, ReadRequests) {
    ConverterJSON converter;
    auto requests = converter.getRequests();
    EXPECT_EQ(requests.size(), 2);
    EXPECT_EQ(requests[0], "hello");
    EXPECT_EQ(requests[1], "world");
}

TEST_F(ConverterJSONTest, WriteAnswers) {
    ConverterJSON converter;
    std::vector<std::vector<RelativeIndex>> answers = {
        { {0, 1.0f}, {1, 0.5f} },
        {}
    };
    converter.putAnswers(answers);

    std::ifstream file("answers.json");
    std::string content((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    EXPECT_NE(content.find("request001"), std::string::npos);
    EXPECT_NE(content.find("request002"), std::string::npos);
}

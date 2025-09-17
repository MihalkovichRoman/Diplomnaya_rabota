#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <iostream>

int main() {
    try {
        ConverterJSON converter;
        auto docs = converter.getTextDocuments();

        InvertedIndex idx;
        idx.UpdateDocumentBase(docs);

        SearchServer server(idx);
        auto requests = converter.getRequests();
        auto answers = server.search(requests);

        converter.putAnswers(answers);

        std::cout << "Search completed. Results saved to answers.json\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return 0;
}

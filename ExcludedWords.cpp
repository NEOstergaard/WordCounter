/////////////////////////////////////////////////////////////////////////////////////////
// ExcludedWords.cpp
/////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "ExcludedWords.h"
#include "Utility.h"

namespace WordCounter {

    ExcludedWords::SPtr ExcludedWords::create(const fs::path& file) {
        SPtr instance(new ExcludedWords(file));
        if (instance) {
            if (!instance->read_excluded_words()) {
                return SPtr();
            }
        }
        return instance;
    }

    void ExcludedWords::remove_excluded_words(
        std::map<std::string, uint32_t>& word_count,
        std::map<std::string, uint32_t>& excluded_words_count) {

        for (std::string excluded_word : excluded_words_) {
            if (word_count.count(excluded_word) > 0) {
                excluded_words_count.insert({excluded_word, word_count[excluded_word]});
                word_count.erase(excluded_word);
            }
        }
    }

	bool ExcludedWords::read_excluded_words() {
        std::ifstream stream(excluded_word_file_);
        if (!stream) {
            return false;
        }

        std::string line;
        while (std::getline(stream, line)) {
            trim(line);
            to_upper_case(line);
            if (!line.empty()) {
                excluded_words_.insert(line);
            }
        }
        stream.close();
        return true;
	}

    void ExcludedWords::dump() {
        std::cout << "Excluded words: ";
        for (auto entry : excluded_words_) {
            std::cout << entry << ", ";
        }
        std::cout << std::endl;
    }

}

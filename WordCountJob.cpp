/////////////////////////////////////////////////////////////////////////////////////////
// WordCountJob.cpp
/////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <regex>

#include "WordCountJob.h"
#include "Utility.h"

namespace WordCounter {

    WordCountJob::SPtr WordCountJob::create(const fs::path& file) {
        SPtr instance(new WordCountJob(file));
        return instance;
    }

    bool WordCountJob::execute(const Results::WPtr& results) {
        // std::cout << "WordCountJob::execute" << std::endl;

        std::map<std::string, uint32_t> my_result;
        if (!count_words(my_result)) {
            return false;
        }

        if (auto res = results.lock()) {
            res->store_results(my_result);
            return true;
        }

        // Results object does not seem to exist. Should not happen
        std::cout << "Error: Could not save the word count for " << word_count_file_ << std::endl;
        return false;
    }

    bool WordCountJob::count_words(std::map<std::string, uint32_t>& result) {
        // std::cout << "WordCountJob::count_words - " << word_count_file_ << std::endl;

        std::ifstream stream(word_count_file_);
        if (!stream) {
            std::cout << "Error: File could not be read " << word_count_file_  << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(stream, line)) {
            //std::cout << "    Line: " << line << std::endl;

            try {
                // Todo : This will compile the regular expression for each line. 
                //        Consider if placing it outside of the loop be better
                std::regex word_regex("([[:alnum:]]+)");
                auto words_begin = std::sregex_iterator(line.begin(), line.end(), word_regex);
                auto words_end = std::sregex_iterator();

                // Loop through each word on the line and handle it
                for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                    std::string lower_case_word = (*i).str();
                    to_upper_case(lower_case_word);

                    // Note: Inserting an element will either create a new element (value 0) or do nothing if it already exists
                    // An iterator will be returned to the element that we can update.
                    auto element(result.insert({lower_case_word, 0}).first);
                    (*element).second++;
                    //std::cout << "    - Word: " << lower_case_word << std::endl;
                }
            } catch (std::regex_error& e) {
                std::cout << "Error: " << e.what() << std::endl;
                stream.close();
                return false;

            }
        }

        stream.close();
        return true;
    }

}

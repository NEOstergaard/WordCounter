/////////////////////////////////////////////////////////////////////////////////////////
// ExcludedWords.h
// 
// Implements functionality to read the excluded words from a file along
// with removing/counting excluded words
/////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <memory>
#include <filesystem>
#include <fstream>
#include <string>
#include <set>
#include <map>

namespace fs = std::filesystem;

namespace WordCounter {

	class ExcludedWords {
	public:

		typedef std::shared_ptr<ExcludedWords> SPtr;
		typedef std::weak_ptr<ExcludedWords> WPtr;

		ExcludedWords(const ExcludedWords&) = delete;
		ExcludedWords& operator=(const ExcludedWords&) = delete;

		virtual ~ExcludedWords() {}

		// Create an object instance
		// Note: returns empty pointer if the file with excluded words can not be read
		static SPtr create(const fs::path& file);

		// Removes excluded words from 'word_count'. The word count for the excluded words are returned in 'excluded_words_count'
		void remove_excluded_words(
			std::map<std::string, uint32_t>& word_count,
			std::map<std::string, uint32_t>& excluded_words_count);

		// For debugging purposes. Writes the excluded words to stdout
		void dump();

	private:
		ExcludedWords(const fs::path& file) :
			excluded_word_file_(file) {};

		// Read the excluded words from the file 'excluded_word_file_'. Returns false on error.
		bool read_excluded_words();

		fs::path excluded_word_file_;
		std::set<std::string> excluded_words_;
	};

}

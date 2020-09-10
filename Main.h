/////////////////////////////////////////////////////////////////////////////////////////
// Main.h
// 
// Implements the main control logic of the program.
/////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <memory>
#include <filesystem>
#include <list>
#include <map>

namespace fs = std::filesystem;

namespace WordCounter {

	class Main {
	public:

		typedef std::shared_ptr<Main> SPtr;
		typedef std::weak_ptr<Main> WPtr;

		Main(const Main&) = delete;
		Main& operator=(const Main&) = delete;

		virtual ~Main() {}

		// Create an object instance
		static SPtr create();

		// Starts running the program. A return value of true means that the program should iterate once more
		bool run();

	private:
		const int MAX_PARALLEL_THREADS = 3;
		const char* EXCLUDED_WORDS_FILE = ".\\exclude.txt";

		Main() {};

		// Input a directory from console and returns it in 'directory'. The function will check that the directory exists.
		bool input_directory(fs::path& directory) const;

		// Get all filenames (regular files) in 'directory' and return them in 'files'
		bool get_data_filenames(const fs::path& directory, std::list<fs::path>& files) const;

		// Save all word counts from iterator 'word_count_begin' to 'word_count_end' in the file 'file_path'
		// Example of file content
		//     LIKE 3
		//     LORUM 10
		bool save_count_file(
			const fs::path& file_path,
			const std::map<std::string, uint32_t>::const_iterator& word_count_begin,
			const std::map<std::string, uint32_t>::const_iterator& word_count_end) const;
	};
}

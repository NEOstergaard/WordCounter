/////////////////////////////////////////////////////////////////////////////////////////
// Main.cpp
/////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Main.h"
#include "JobExecuter.h"
#include "WordCountJob.h"
#include "ExcludedWords.h"
#include "Utility.h"

namespace WordCounter {

	Main::SPtr Main::create() {
		SPtr instance(new Main());
		return instance;
	}

	bool Main::run() {

		// Read the words to be excluded
		ExcludedWords::SPtr excl_words(ExcludedWords::create(EXCLUDED_WORDS_FILE));
		if (!excl_words) {
			std::cout << "Error: Failed to read excluded words from file " << EXCLUDED_WORDS_FILE << std::endl;
			return false;
		}

		// For debugging only
		//excl_words->dump();

		// Input the directory with all data files
		fs::path data_directory;
		if (!input_directory(data_directory)) {
			return false;
		}

		// Collect all data file names from directory
		std::list<fs::path> data_file_paths;
		if (!get_data_filenames(data_directory, data_file_paths)) {
			return false;
		}

		// Initialize the job executer and add a job for each data file found
		JobExecuter::SPtr je(JobExecuter::create(MAX_PARALLEL_THREADS));
		for (auto& file_path : data_file_paths) {
			je->add_job(WordCountJob::create(file_path));
		}

		// Run all the jobs and collect the results
		Results::SPtr results(Results::create(excl_words));
		je->start(results);

		// Count the excluded words and remove them from the results. Save the excluded words to a file.
		std::map<std::string, uint32_t> excluded_words_count;
		results->exclude_words(excluded_words_count);
		save_count_file("./excluded_words_count.txt", excluded_words_count.begin(), excluded_words_count.end());

		// For debugging only
		//results->dump();

		// Save all the results to files like FILE_A.txt, FILE_C.txt etc.
		results->save_results(
			[this] (char first_letter, 
				const std::map<std::string, uint32_t>::const_iterator& word_count_begin,
				const std::map<std::string, uint32_t>::const_iterator& word_count_end) -> bool {
					std::stringstream filename;
					filename << "FILE_" << std::string(1, std::toupper(first_letter)) << ".txt";

					fs::path file = fs::path(".") / filename.str();

					return save_count_file(file, word_count_begin, word_count_end);
			}
		);

		return true;
	}

	bool Main::input_directory(fs::path& directory) const {
		bool valid_directory = false;
		while (!valid_directory) {
			std::cout << "Type in data directory (type '-' to stop): ";

			std::string input_line;
			std::cin >> input_line;
			trim(input_line);

			// Used to exit program
			if (input_line == "-") {
				return false;
			}

			try {
				if (fs::directory_entry(input_line).is_directory()) {
					valid_directory = true;
					directory = fs::path(input_line);
				} else {
					std::cout << "Error: Not a valid directory - " << input_line << std::endl;
				}
			} catch (const fs::filesystem_error& error) {
				std::cerr << "Error: " << error.what() << std::endl;
			}
		}
		return true;
	}

	bool Main::get_data_filenames(const fs::path& directory, std::list<fs::path>& files) const {
		try {
			for (const auto& entry : fs::directory_iterator(directory)) {
				if (entry.is_regular_file()) {
					//std::cout << "Found data file: " << entry.path() << std::endl;
					files.push_back(entry.path());
				}
			}
			return true;
		} catch (const fs::filesystem_error& error) {
			std::cerr << "Error: " << error.what() << std::endl;
		}
		return false;
	}

	bool Main::save_count_file(
			const fs::path& file_path, 
			const std::map<std::string, uint32_t>::const_iterator& word_count_begin,
			const std::map<std::string, uint32_t>::const_iterator& word_count_end) const {

		// Anything to write
		if (word_count_begin == word_count_end) {
			return true;
		}

		std::ofstream stream(file_path, std::ofstream::out | std::ofstream::trunc);
		if (!stream) {
			std::cout << "Error: Failed to write to file " << file_path << std::endl;
			return false;
		}

		auto it(word_count_begin);
		while (it != word_count_end) {
			stream << it->first << " " << it->second << std::endl;
			it++;
		}

		stream.close();
		return true;
	}

};

int main()
{
	bool run_again = true;
	while (run_again) {
		run_again = WordCounter::Main::create()->run();
		if (run_again) {
			std::cout << "Data processed" << std::endl;
		}
	}
	return 0;
}

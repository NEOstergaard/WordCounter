/////////////////////////////////////////////////////////////////////////////////////////
// WordCountJob.h
// 
// Implements a job for the Job Executer.
// It will read a data file and count the occurrance of each word. 
// The results will be returned.
/////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <memory>
#include <filesystem>
#include <map>

#include "JobBase.h"

namespace fs = std::filesystem;

namespace WordCounter {

	class WordCountJob :
		public JobBase {

	public:

		typedef std::shared_ptr<WordCountJob> SPtr;
		typedef std::weak_ptr<WordCountJob> WPtr;

		WordCountJob(const WordCountJob&) = delete;
		WordCountJob& operator=(const WordCountJob&) = delete;

		virtual ~WordCountJob() {}

		// Create an object instance
		static SPtr create(const fs::path& file);

	private:
		WordCountJob(const fs::path& file) :
			word_count_file_(file),
			JobBase() {};

		// 
		// Implementation of JobBase interface
		//

		// Implements the actual job. The word count will be stored in 'results'
		virtual bool execute(const Results::WPtr& results);

		//
		// Local
		//

		// Read the data file line by line and count the words for each line.
		// 'result' will aggregate the word count for the whole data file. 
		bool count_words(std::map<std::string, uint32_t>& result);

		fs::path word_count_file_;
	};

}


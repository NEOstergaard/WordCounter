/////////////////////////////////////////////////////////////////////////////////////////
// Results.h
// 
// Implements functionality to collect data results from jobs being executed by the Job Executer
// The the data stored are word counts.
// The class also implements various computations that can be done on the collected data
/////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <memory>
#include <map>
#include <string>
#include <functional>
#include <mutex>

#include "ExcludedWords.h"

namespace WordCounter {

	class Results {
	public:

		typedef std::shared_ptr<Results> SPtr;
		typedef std::weak_ptr<Results> WPtr;

		Results(const Results&) = delete;
		Results& operator=(const Results&) = delete;

		virtual ~Results() {}

		// Create an object instance. Argument is the excluded words used for one of the computations
		static SPtr create(const ExcludedWords::SPtr& excluded_words);

		// When a job has finished it should store the collected data using this function.
		// It will aggregate the data in 'result' with the already collected data from other jobs
		void store_results(const std::map<std::string, uint32_t>& result);

		// Calling this function will remove all excluded words from the aggregated results.
		// It will return the word count for the excluded words.
		void exclude_words(std::map<std::string, uint32_t>& excluded_words_count);

		// Loops through all aggregated data and segments it into groups where the words have the same 
		// initial letter. Each group is given to the function 'save' using the two iterators.
		// The function 'save' can then implement appropriate means to save the group.
		void save_results(
			const std::function<void(
				char first_letter,
				const std::map<std::string, uint32_t>::const_iterator& group_begin,
				const std::map<std::string, uint32_t>::const_iterator& group_end)>& save);

		// For debugging purposes
		void dump();

	private:
		Results(const ExcludedWords::SPtr& excluded_words) : 
			excluded_words_(excluded_words) {};

		std::mutex mutex_;

		std::map<std::string, uint32_t> result_data_;
		ExcludedWords::SPtr excluded_words_;
	};

}


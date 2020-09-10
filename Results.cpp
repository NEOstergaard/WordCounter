/////////////////////////////////////////////////////////////////////////////////////////
// Results.cpp
/////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "Results.h"

namespace WordCounter {

	Results::SPtr Results::create(const ExcludedWords::SPtr& excluded_words) {
		SPtr instance(new Results(excluded_words));
		return instance;
	}

	void Results::store_results(const std::map<std::string, uint32_t>& result) {
		std::lock_guard<std::mutex> lock_guard(mutex_);
		// std::cout << "Results::store_results enter" << std::endl;

		for (auto result_entry : result) {
			// Note: Inserting an element will either create a new element (value 0) or do nothing if it already exists
			result_data_.insert({result_entry.first, 0});
			result_data_[result_entry.first] += result_entry.second;
		}

		// std::cout << "Results::store_results exit" << std::endl;
	}

	void Results::exclude_words(std::map<std::string, uint32_t>& excluded_words_count) {
		std::lock_guard<std::mutex> lock_guard(mutex_);
		// std::cout << "Results::exclude_words enter" << std::endl;

		excluded_words_->remove_excluded_words(result_data_, excluded_words_count);

		// std::cout << "Results::exclude_words exit" << std::endl;
	}

	void Results::dump() {
		std::lock_guard<std::mutex> lock_guard(mutex_);
		// std::cout << "Results::dump enter" << std::endl;

		std::cout << "----------------" << std::endl;
		for (auto entry : result_data_) {
			std::cout << "  " << entry.first << " : " << entry.second << std::endl;
		}

		// std::cout << "Results::dump exit" << std::endl;
	}

	void Results::save_results(
			const std::function<void(
				char first_letter,
				const std::map<std::string, uint32_t>::const_iterator&,
				const std::map<std::string, uint32_t>::const_iterator&)> & save) {
		std::lock_guard<std::mutex> lock_guard(mutex_);
		// std::cout << "Results::save_results enter" << std::endl;

		auto it = result_data_.begin();
		if (it == result_data_.end()) {
			// Nothing to do
			return;
		}

		char group_letter = (*it).first[0];
		std::map<std::string, uint32_t>::const_iterator group_begin_it(it);
		std::map<std::string, uint32_t>::const_iterator group_end_it;

		// Loop through all elements
		while (it != result_data_.end()) {

			// Loop though a group with the same initial letter to determine the end of the group. 
			// Note that std::maps store elements in order
			while (it != result_data_.end() && (*it).first[0] == group_letter) {
				// std::cout << it->first << " " << it->second << std::endl;
				it++;
			}
			group_end_it = it;
			save(group_letter, group_begin_it, group_end_it);

			// If not finished yet then prepare for the next group
			if (it != result_data_.end()) {
				group_letter = it->first[0];
				group_begin_it = it;
			}
		}

		// std::cout << "Results::save_results exit" << std::endl;
	}

}

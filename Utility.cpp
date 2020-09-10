/////////////////////////////////////////////////////////////////////////////////////////
// Utility.cpp
/////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <algorithm>

#include "Utility.h"

namespace WordCounter {

	void to_upper_case(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(),	[] (unsigned char c) { return std::toupper(c); });
	}

	//
	// Note: The trim functions have been taken from stack overflow
	//   https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
	//

	void ltrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
		}));
	}

	void rtrim(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	void trim(std::string& s) {
		ltrim(s);
		rtrim(s);
	}

}

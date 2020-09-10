/////////////////////////////////////////////////////////////////////////////////////////
// Utility.h
// 
// Various small utility functions
/////////////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace WordCounter {

	// Convert string to lower case
	void to_upper_case(std::string& str);

	// Trim string of whitespace in-place
	void ltrim(std::string& s);
	void rtrim(std::string& s);
	void trim(std::string& s);

}

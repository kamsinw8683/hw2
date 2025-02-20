#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "util.h"

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/
std::set<std::string> parseStringToWords(string rawWords)
{
    std::set<std::string> keywords;
    stringstream ss(rawWords);
    string word;

    while (ss >> word) {
        // Trim the word
        word = trim(word);

        // Convert to lowercase
        word = convToLower(word);
        
        // Replace punctuation with spaces
        string processedWord;
        for (char c : word) {
            if (ispunct(c)) {
                processedWord += ' ';
            } else {
                processedWord += c;
            }
        }

        // Split punctuation-replaced word
        stringstream wordss(processedWord);
        string subword;
        while (wordss >> subword) {
            // Trim the subword
            subword = trim(subword);

            // Only add keywords of 2 or more characters
            if (subword.length() >= 2) {
                keywords.insert(subword);
            }
        }
    }

    return keywords;
}

/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

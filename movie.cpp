#include "movie.h"
#include "util.h"
#include <sstream>
#include <iomanip>

Movie::Movie(const std::string& category, const std::string& name, double price, int qty, 
             const std::string& genre, const std::string& rating)
    : Product(category, name, price, qty), genre_(genre), rating_(rating) {}

std::set<std::string> Movie::keywords() const {
    std::set<std::string> keywords_set = parseStringToWords(name_);
    std::set<std::string> genre_keywords = parseStringToWords(genre_);
    
    keywords_set.insert(genre_keywords.begin(), genre_keywords.end());
    
    return keywords_set;
}

std::string Movie::displayString() const {
    std::stringstream ss;
    ss << name_ << std::endl
       << "Genre: " << genre_ << " Rating: " << rating_ << std::endl
       << std::fixed << std::setprecision(2) << price_ << " " << qty_ << " left.";
    return ss.str();
}

void Movie::dump(std::ostream& os) const {
    os << category_ << std::endl
       << name_ << std::endl
       << price_ << std::endl
       << qty_ << std::endl
       << genre_ << std::endl
       << rating_ << std::endl;
}
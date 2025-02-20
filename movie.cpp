#include "movie.h"
#include "util.h"
#include <sstream>
#include <iomanip>

Movie::Movie(const std::string& category, const std::string& name, double price, int qty, 
             const std::string& genre, const std::string& rating)
    : Product(category, name, price, qty), genre_(genre), rating_(rating) {}

std::set<std::string> Movie::keywords() const
{
    std::set<std::string> words;

    

    // Parse product name
    std::set<std::string> nameWords = parseStringToWords(name_);
    words.insert(nameWords.begin(), nameWords.end());

    // Parse author name
    std::set<std::string> brand = parseStringToWords(getGenre());
    words.insert(brand.begin(), brand.end());

    // Add ISBN as a keyword
    words.insert(getRating());
    words.insert(getCategory());

    

    return words;
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
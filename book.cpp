#include "book.h"
#include "util.h"
#include <sstream>
#include <iomanip>

Book::Book(const std::string& category, const std::string& name, double price, int qty, const std::string& author, const std::string& isbn)
    : Product(category, name, price, qty), author_(author), isbn_(isbn) 
{
}

std::set<std::string> Book::keywords() const {
    std::set<std::string> keywords_set = parseStringToWords(name_);
    std::set<std::string> author_keywords = parseStringToWords(author_);
    
    keywords_set.insert(isbn_);
    keywords_set.insert(author_keywords.begin(), author_keywords.end());
    
    return keywords_set;
}

std::string Book::displayString() const {
    std::stringstream ss;
    ss << name_ << std::endl
       << "Author: " << author_ << " ISBN: " << isbn_ << std::endl << std::fixed << std::setprecision(2) << price_ << " " << qty_ << " left.";
    return ss.str();
}

void Book::dump(std::ostream& os) const {
    os << category_ << std::endl
       << name_ << std::endl
       << price_ << std::endl
       << qty_ << std::endl
       << isbn_ << std::endl
       << author_ << std::endl;
}
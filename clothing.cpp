#include "clothing.h"
#include "util.h"
#include <sstream>
#include <iomanip>

Clothing::Clothing(const std::string& category, const std::string& name, double price, int qty, const std::string& size, const std::string& brand)
    : Product(category, name, price, qty), size_(size), brand_(brand) {}

std::set<std::string> Clothing::keywords() const {
    std::set<std::string> keywords_set = parseStringToWords(name_);
    std::set<std::string> brand_keywords = parseStringToWords(brand_);
    
    keywords_set.insert(brand_keywords.begin(), brand_keywords.end());
    
    return keywords_set;
}

std::string Clothing::displayString() const {
    std::stringstream ss;
    ss << name_ << std::endl
       << "Size: " << size_ << " Brand: " << brand_ << std::endl
       << std::fixed << std::setprecision(2) << price_ << " " << qty_ << " left.";
    return ss.str();
}

void Clothing::dump(std::ostream& os) const {
    os << category_ << std::endl
       << name_ << std::endl
       << price_ << std::endl
       << qty_ << std::endl
       << size_ << std::endl
       << brand_ << std::endl;
}
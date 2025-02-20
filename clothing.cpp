#include "clothing.h"
#include "util.h"
#include <sstream>
#include <iomanip>

Clothing::Clothing(const std::string& category, const std::string& name, double price, int qty, const std::string& size, const std::string& brand)
    : Product(category, name, price, qty), size_(size), brand_(brand) {}

std::set<std::string> Clothing::keywords() const
{
    std::set<std::string> words;

    

    // Parse product name
    std::set<std::string> nameWords = parseStringToWords(name_);
    words.insert(nameWords.begin(), nameWords.end());

    // Parse author name
    std::set<std::string> brand = parseStringToWords(getBrand());
    words.insert(brand.begin(), brand.end());

    // Add ISBN as a keyword
    words.insert(getSize());
    words.insert(getCategory());

    

    return words;
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
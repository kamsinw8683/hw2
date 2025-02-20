#ifndef BOOK_H
#define BOOK_H

#include "product.h"
#include <string>
#include <set>

class Book : public Product {
public:
    Book(const std::string& category, const std::string& name, double price, int qty, const std::string& author, const std::string& isbn);
    
    std::set<std::string> keywords() const;
    std::string displayString() const;
    void dump(std::ostream& os) const;

    std::string getAuthor() const { return author_; }
    std::string getISBN() const { return isbn_; }

private:
    std::string author_;
    std::string isbn_;
};

#endif
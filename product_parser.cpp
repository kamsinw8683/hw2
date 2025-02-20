#include <iostream>
#include <sstream>
#include "product_parser.h"
#include "book.h"
#include "clothing.h"
#include "movie.h"
#include "util.h"
using namespace std;

ProductParser::ProductParser()
{

}

ProductParser::~ProductParser()
{

}

Product* ProductParser::parse(string category,
                              istream& is,
                              bool& error,
                              string& errorMsg,
                              int& lineno)
{
    // Reset error state
    error = false;
    errorMsg.clear(); 

    // Validate input stream
    if (!is.good()) {
        error = true;
        errorMsg = "Invalid input stream";
        return nullptr;
    }

    // Parse common product details
    parseCommonProduct(is, error, errorMsg, lineno);
    if (error) {
        return nullptr;
    }

    // Parse specific product details
    return parseSpecificProduct(category, is, error, errorMsg, lineno);
}

void ProductParser::parseCommonProduct(std::istream& is,
                                       bool& error,
                                       string& errorMsg,
                                       int& lineno)
{
    string myline;
    
    // Validate stream before reading
    if (!is.good()) {
        error = true;
        errorMsg = "Stream is in a bad state";
        return;
    }

    // Read product name
    getline(is, myline);
    myline = trim(myline);
    
    if (myline.empty()) {
        error = true;
        errorMsg = "Unable to find a product name";
        return;
    }
    
    // Additional name validation
    if (myline.length() > 255) {  // Arbitrary max length
        error = true;
        errorMsg = "Product name too long";
        return;
    }

    prodName_ = myline;
    lineno++;
    
    // Price parsing with robust error checking
    getline(is, myline);
    if (is.fail()) {
        error = true;
        errorMsg = "Expected another line with the price";
        return;
    }
    
    stringstream ss1(myline);
    ss1 >> price_;
    
    if (ss1.fail() || price_ < 0) {
        error = true;
        errorMsg = "Invalid price";
        return;
    }

    lineno++;
    
    // Quantity parsing
    getline(is, myline);
    if (is.fail()) {
        error = true;
        errorMsg = "Expected another line with the quantity";
        return;
    }
    
    stringstream ss2(myline);
    ss2 >> qty_;
    
    if (ss2.fail() || qty_ < 0) {
        error = true;
        errorMsg = "Invalid quantity";
        return;
    }

    lineno++;
}

ProductBookParser::ProductBookParser() : ProductParser()
{
}

Product* ProductBookParser::parseSpecificProduct(std::string category,
        std::istream& is,
        bool& error,
        std::string& errorMsg,
        int& lineno)
{
    string myline;
    getline(is, myline);
    stringstream ss3(myline);
    ss3 >> isbn_;
    if(ss3.fail()) {
        error = true;
        errorMsg = "Unable to read ISBN";
        return NULL;
    }

    lineno++;
    getline(is, author_);
    if(is.fail()) {
        error = true;
        errorMsg = "Unable to read author";
        return NULL;
    }
#ifdef DEBUG
    cout << "Making product " << prodName_ << endl;
#endif
    lineno++;
    if(error) {
        return NULL;
    }
    return makeProduct();
}

std::string ProductBookParser::categoryID()
{
    return "book";
}

Product* ProductBookParser::makeProduct()
{
    return new Book("book", prodName_, price_, qty_, author_, isbn_);
}

ProductClothingParser::ProductClothingParser()
{
}

Product* ProductClothingParser::parseSpecificProduct(std::string category,
        std::istream& is,
        bool& error,
        std::string& errorMsg,
        int& lineno)
{
    string myline;
    getline(is, myline);
    stringstream ss3(myline);
    ss3 >> size_;
    if(ss3.fail()) {
        error = true;
        errorMsg = "Unable to read size";
        return NULL;
    }

    lineno++;
    getline(is, brand_);
    if(is.fail() || (brand_.size() == 0)) {
        error = true;
        errorMsg = "Unable to read brand";
        return NULL;
    }
#ifdef DEBUG
    cout << "Making product " << prodName_ << endl;
#endif
    lineno++;
    if(error) {
        return NULL;
    }
    return makeProduct();
}

std::string ProductClothingParser::categoryID()
{
    return "clothing";
}

Product* ProductClothingParser::makeProduct()
{
    return new Clothing("clothing", prodName_, price_, qty_, size_, brand_);
}

ProductMovieParser::ProductMovieParser()
{
}

Product* ProductMovieParser::parseSpecificProduct(std::string category,
        std::istream& is,
        bool& error,
        std::string& errorMsg,
        int& lineno)
{
    string myline;
    getline(is, myline);
    stringstream ss3(myline);
    ss3 >> genre_;
    if(ss3.fail()) {
        error = true;
        errorMsg = "Unable to read genre";
        return NULL;
    }

    lineno++;
    getline(is, rating_);
    if(is.fail() || (rating_.size() == 0)) {
        error = true;
        errorMsg = "Unable to read rating";
        return NULL;
    }
#ifdef DEBUG
    cout << "Making product " << prodName_ << endl;
#endif
    lineno++;
    if(error) {
        return NULL;
    }
    return makeProduct();
}

std::string ProductMovieParser::categoryID()
{
    return "movie";
}

Product* ProductMovieParser::makeProduct()
{
    return new Movie("movie", prodName_, price_, qty_, genre_, rating_);
}
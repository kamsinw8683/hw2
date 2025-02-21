#include "mydatastore.h"
#include "util.h"
#include <iostream>
#include <sstream>
#include <iomanip>


// Explicit destructor definition
MyDataStore::~MyDataStore() {
    // Clean up all dynamically allocated products and users
    for (Product* p : allProducts) {
        delete p;
    }
    for (User* u : allUsers) {
        delete u;
    }
}

void MyDataStore::addProduct(Product* p) {
    // Validate input pointer
    if (p == nullptr) {
        return;
    }

    // Validate product name
    std::string name = p->getName();
    if (name.empty()) {
        return;
    }

    // Check for duplicate product
    if (allProducts.find(p) != allProducts.end()) {
        return;
    }

    // Add to product set
    allProducts.insert(p);

    // Index keywords
    std::set<std::string> keywords = p->keywords();
    for (const std::string& keyword : keywords) {
        if (!keyword.empty()) {
            productsByKeyword[keyword].insert(p);
        }
    }
}

// Explicit addUser method definition
void MyDataStore::addUser(User* u) {
     // Validate input
    if (!u) {
        return;
    }

    // Validate username
    std::string username = u->getName();
    if (username.empty()) {
        return;
    }

    // Store user in overall user set
    allUsers.insert(u);

    // Add to users map by username
    users[username] = u;
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
    std::set<Product*> results;
    
    if (terms.empty()) return std::vector<Product*>();

    // Convert terms to lowercase for case-insensitive search
    for (std::string& term : terms) {
        term = convToLower(term);
    }

    // First term's results
    std::set<Product*> current_results = productsByKeyword[terms[0]];

    // Intersect or Union based on type
    for (size_t i = 1; i < terms.size(); ++i) {
        std::set<Product*> term_results = productsByKeyword[terms[i]];
        
        if (type == 0) { // AND search
            current_results = setIntersection(current_results, term_results);
        } else { // OR search
            current_results = setUnion(current_results, term_results);
        }
    }

    // Convert set to vector
    return std::vector<Product*>(current_results.begin(), current_results.end());
}

// Explicit dump method definition
void MyDataStore::dump(std::ostream& ofile) {
    void dump(std::ostream& ofile)
{
    // Always write products section
    ofile << "<products>" << std::endl;
    
    // If products exist, dump them
    for (Product* p : products_) {
        p->dump(ofile);
    }
    
    ofile << "</products>" << std::endl;

    // Always write users section
    ofile << "<users>" << std::endl;
    
    // If users exist, dump them
    for (map<string, User*>::iterator it = users_.begin(); 
         it != users_.end(); ++it) {
        it->second->dump(ofile);
    }
    
    ofile << "</users>" << std::endl;
}
}

// Explicit addToCart method definition
void MyDataStore::addToCart(std::string username, Product* p) {
    // Check if user exists
    if (users.find(username) == users.end()) {
        std::cout << "Invalid username" << std::endl;
        return;
    }

    // Add product to user's cart
    userCarts[username].push_back(p);
}

// Explicit viewCart method definition
void MyDataStore::viewCart(std::string username) {
    // Check if user exists
    if (users.find(username) == users.end()) {
        std::cout << "Invalid username" << std::endl;
        return;
    }

    // Display cart items
    std::vector<Product*>& cart = userCarts[username];
    for (size_t i = 0; i < cart.size(); ++i) {
        std::cout << "Item " << i+1 << std::endl;
        std::cout << cart[i]->displayString() << std::endl;
    }
}

// Explicit buyCart method definition
void MyDataStore::buyCart(std::string username) {
    // Check if user exists
    if (users.find(username) == users.end()) {
        std::cout << "Invalid username" << std::endl;
        return;
    }

    User* user = users[username];
    std::vector<Product*>& cart = userCarts[username];
    std::vector<Product*> remainingCart;

    for (Product* p : cart) {
        // Check if user can afford and product is in stock
        if (p->getQty() > 0 && user->getBalance() >= p->getPrice()) {
            user->deductAmount(p->getPrice());
            p->subtractQty(1);
        } else {
            // Keep items that couldn't be purchased
            remainingCart.push_back(p);
        }
    }

    // Replace cart with remaining items
    userCarts[username] = remainingCart;
}
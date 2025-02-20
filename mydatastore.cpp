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

// Explicit addProduct method definition
void MyDataStore::addProduct(Product* p) {
    if (!p) return;

    // Store product in overall product set
    allProducts.insert(p);

    // Add to keyword index
    std::set<std::string> keywords = p->keywords();
    for (const std::string& keyword : keywords) {
        productsByKeyword[keyword].insert(p);
    }
}

// Explicit addUser method definition
void MyDataStore::addUser(User* u) {
    if (!u) return;

    // Store user in overall user set
    allUsers.insert(u);

    // Add to users map by username
    users[u->getName()] = u;
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
    // Dump products
    for (Product* p : allProducts) {
        p->dump(ofile);
    }

    // Dump users
    for (User* u : allUsers) {
        u->dump(ofile);
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
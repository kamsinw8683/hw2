#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include "product.h"
#include "user.h"
#include "util.h"
#include <map>
#include <set>
#include <vector>
#include <string>

class MyDataStore : public DataStore {
public:
    ~MyDataStore();

    virtual void addProduct(Product* p) ;
    virtual void addUser(User* u) ;
    virtual std::vector<Product*> search(std::vector<std::string>& terms, int type) ;
    virtual void dump(std::ostream& ofile);

    // Additional methods for cart and other functionality
    void addToCart(std::string username, Product* p);
    void viewCart(std::string username);
    void buyCart(std::string username);

private:
    // Store products by category and keywords for efficient searching
    std::map<std::string, std::set<Product*>> productsByKeyword;
    std::map<std::string, std::set<Product*>> productsByCategory;
    
    // Store users and their carts
    std::map<std::string, User*> users;
    std::map<std::string, std::vector<Product*>> userCarts;
    
    // Store all products to manage memory
    std::set<Product*> allProducts;
    std::set<User*> allUsers;
};

#endif
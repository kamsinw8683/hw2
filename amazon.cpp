#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"

using namespace std;

struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};

void displayProducts(vector<Product*>& hits);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    // Use the MyDataStore implementation
    MyDataStore ds;

    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ...                  " << endl;
    cout << "  OR term term ...                   " << endl;
    cout << "  ADD username search_hit_number     " << endl;
    cout << "  VIEWCART username                  " << endl;
    cout << "  BUYCART username                   " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND" || cmd == "OR") {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                
                // Handle single term case
                if (terms.empty()) {
                    cout << "No search terms provided" << endl;
                    continue;
                }
                int mode;
                if (cmd == "AND") {
                    mode = 0;
                } else {
                    mode = 1;
                }

                hits = ds.search(terms, mode);
                displayProducts(hits);
            }
            else if ( cmd == "ADD") {
                string username;
                int hit_num;
                if (!(ss >> username >> hit_num)) {
                    cout << "Invalid request" << endl;
                    continue;
                }

                // Validate hit number
                if (hit_num < 1 || hit_num > (int)hits.size()) {
                    cout << "Invalid request" << endl;
                    continue;
                }

                // Add product to cart
                ds.addToCart(username, hits[hit_num-1]);
            }
            else if ( cmd == "VIEWCART") {
                string username;
                if (!(ss >> username)) {
                    cout << "Invalid request" << endl;
                    continue;
                }

                // View user's cart
                ds.viewCart(username);
            }
            else if ( cmd == "BUYCART") {
                string username;
                if (!(ss >> username)) {
                    cout << "Invalid request" << endl;
                    continue;
                }

                // Buy items in user's cart
                ds.buyCart(username);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
            else {
                cout << "Unknown command" << endl;
            }
        }
    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    if (hits.empty()) {
        cout << "No results found!" << endl;
        return;
    }
    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}
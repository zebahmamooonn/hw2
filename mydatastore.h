#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include <map>
#include <vector>
#include <queue>

class MyDataStore : public DataStore{
public:
  ~MyDataStore();
  void addProduct(Product* p) override;
  void addUser(User* u) override;
  std::vector<Product*> search(std::vector<std::string>& terms, int type) override;
  void dump(std::ostream& ofile) override; 

  void addToCart(std::string username, Product * p);
  void viewCart(std::string username);
  void buyCart(std::string username);

  private:
    std::vector<Product*> products_;
    std::map<std::string, User*> users_;
    std::map<std::string, std::set<Product*>> keywordMap;
    std::map<std::string, std::vector<Product*>> userCarts;

}; 

#endif
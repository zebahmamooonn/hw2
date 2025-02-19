#include "mydatastore.h"
#include "util.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>

MyDataStore::~MyDataStore(){
  for(auto p : products_) delete p;
  for(auto u : users_) delete u.second;
}

void MyDataStore::addProduct(Product* p){
  products_.push_back(p);
  for(const std::string& kw : p->keywords() ){
    keywordMap[convToLower(kw)].insert(p); 
  }
}

void MyDataStore::addUser(User* u){
  users_[convToLower(u->getName())] = u; 
}

struct ProdNameSorter{
  bool operator()(Product* a, Product* b) const{
    return a->getName() < b->getName(); 
  }
}; 

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type){
  std::set<Product*> result;
  for(const std::string& term : terms){
    if(keywordMap.find(term) != keywordMap.end()){
      if(type == 0){
        if(result.empty()) result = keywordMap[term];
        else result = setIntersection(result, keywordMap[term]); 
      }
      else{
        result = setUnion(result, keywordMap[term]); 
      }
    }
  }
  std::vector<Product*> sortedResults(result.begin(), result.end());
  std::sort(sortedResults.begin(), sortedResults.end(), ProdNameSorter());
  return sortedResults; 
}

void MyDataStore::addToCart(std::string username, Product* p){
  std::string lowerUsername = convToLower(username); 
 
  if(users_.find(lowerUsername) == users_.end()){
    std::cout<< " Invalid request" << std::endl; 
    return; 
  }
  if(!p){
    std::cout<< "Invalid request" << std::endl;
    return; 
  }
  userCarts[lowerUsername].push_back(p); 
}

void MyDataStore::viewCart(std::string username){
  std::string lowerUsername = convToLower(username);

  if(users_.find(lowerUsername) == users_.end()){
    std::cout<< "Invalid username" << std::endl;
    return; 
  }

  if(userCarts.find(lowerUsername) == userCarts.end() || userCarts[lowerUsername].empty()){
    std::cout<< "Cart is empty" << std::endl;
   return; 
  }
  int i = 1;
  for(Product* p : userCarts[lowerUsername]){
    std::cout<<"Item " << i++ << ":" << std::endl;
    std::cout<< p->displayString() << std::endl << std::endl; 
   }
}

void MyDataStore::buyCart(std::string username){
  std::string lowerUsername = convToLower(username); 

  if(users_.find(lowerUsername) == users_.end()){
    std::cout<<"Invalid username" << std::endl;
    return; 
  }
  User* user = users_[lowerUsername];
  std::vector<Product*>& cart = userCarts[lowerUsername];
  std::vector<Product*> remainingCart; 

  for(Product* p : cart){
    if(p->getQty() > 0 && user->getBalance() >= p->getPrice()){
      p->subtractQty(1);
      user->deductAmount(p->getPrice()); 
    }
    else{
      remainingCart.push_back(p); 
    }
  }
  userCarts[lowerUsername] = remainingCart; 

  if(userCarts[lowerUsername].empty()){
    std::cout<< "Cart is empty" << std::endl;
  }
}

void MyDataStore::dump(std::ostream& ofile){
  ofile << "<products>\n";
  for(Product* p : products_){
    p->dump(ofile);
  }
  ofile << "</products>\n";

  ofile << "<users>\n";
  for(const auto& u : users_){
    u.second->dump(ofile);
  }
  ofile << "</users>\n"; 
}
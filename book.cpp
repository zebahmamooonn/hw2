#include "book.h"
#include "util.h"
#include <sstream>
#include <iomanip>


Book::Book(const std::string& name, double price, int qty, const std::string& isbn, const std::string& author) 
: Product("book", name, price, qty), isbn_(isbn), author_(author){}

std::set<std::string> Book::keywords() const{
  std::set<std::string> keys = parseStringToWords(name_);
  std::set<std::string> authorKeys = parseStringToWords(author_); 
  keys = setUnion(keys, authorKeys); 
  keys.insert(isbn_);
  return keys; 
}

std::string Book::displayString() const{
  std::ostringstream oss;
  oss << name_ << "\n" << "Author: " << author_ << " ISBN: " << isbn_ << "\n" 
  << std::fixed << std::setprecision(2)<< price_ << " " << qty_ << " left.";
  return oss.str();
}

void Book::dump(std::ostream& os) const{
  Product::dump(os);
  os << isbn_ << "\n" << author_ << "\n"; 
}
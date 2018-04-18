#pragma once

#include <string>
#include <vector>
#include <istream>
#include <ostream>

#include "vector.h"

namespace company {

struct Contact {
public:
  Contact();
  Contact(std::string name, std::string number);

  void StringPrint(std::ostream& output);

  const std::string& GetName() const;
  const Vector<std::string>& GetNumbers() const;
  void AddNumber(std::string number);

private:
  std::string name_;
  Vector<std::string> numbers_;
};

class AddressBook {
public:
  void Load(std::istream& input);
  void Save(std::ostream& output);

  void Sort();

  bool FindContact(const std::string& name, Contact& result);
  bool DeleteContact(const std::string& name);

  void StringPrint(std::ostream& output);
 
  const Vector<Contact>& GetContacts() const;

private:
  Vector<Contact> contacts_; // contiguous memory for cache-optimized iteration
};

} // namespace company
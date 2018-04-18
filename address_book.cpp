#include "address_book.h"
#include "vector.h"

#include <string>
#include <algorithm>

namespace company {

Contact::Contact() {}

Contact::Contact(std::string name, std::string number)
  : name_(std::move(name)) {
  AddNumber(std::move(number));
}

void Contact::StringPrint(std::ostream& output) {
  output << "Name: " << name_ << std::endl;
  output << "Numbers: " << std::endl;
  for (auto& number : numbers_) {
    output << number << std::endl;
  }
  output << std::endl;
}

const std::string& Contact::GetName() const {
  return name_;
}

const Vector<std::string>& Contact::GetNumbers() const {
  return numbers_;
}

void Contact::AddNumber(std::string number) {
  numbers_.push_back(std::move(number));
}



void AddressBook::Load(std::istream& input) {
  std::string name;
  std::string number;
  std::getline(input, number); // skip header
  while (std::getline(input, name, ',') && std::getline(input, number)) {
    // O^2, change to hashmap
    auto it = std::find_if(contacts_.begin(), contacts_.end(), [&name](const Contact& contact) {
      return contact.GetName() == name; 
    });

    if (it == contacts_.end()) {
      Contact contact(std::move(name), std::move(number));
      contacts_.push_back(std::move(contact));
    } else {
      it->AddNumber(std::move(number));
    }
  }
}

void AddressBook::Save(std::ostream& output) {
  output << "Name" << "," << "Phone" << std::endl;
  for (auto& contact : contacts_) {
    for (auto& number : contact.GetNumbers()) {
      output << contact.GetName() << "," << number << std::endl;
    }
  }
}

void AddressBook::Sort() {
  std::sort(contacts_.begin(), contacts_.end(), [](const Contact& a, const Contact& b) {
    return a.GetName() < b.GetName(); 
  });
}

bool AddressBook::FindContact(const std::string& name, Contact& result) {
  auto it = std::find_if(contacts_.begin(), contacts_.end(), [&name](const Contact& contact) {
    return contact.GetName() == name;
  });

  if (it != contacts_.end()) {
    result = (*it);
    return true;
  } else {
    return false;
  }
}


bool AddressBook::DeleteContact(const std::string& name) {
  auto it = std::find_if(contacts_.begin(), contacts_.end(), [&name](const Contact& contact) {
    return contact.GetName() == name;
  });

  if (it != contacts_.end()) {
    contacts_.erase(it);
    return true;
  } else {
    return false;
  }
}

void AddressBook::StringPrint(std::ostream& output) {
  for (auto& contact : contacts_) {
    contact.StringPrint(output);
  }
}

const Vector<company::Contact>& AddressBook::GetContacts() const {
  return contacts_;
}

} // namespace company
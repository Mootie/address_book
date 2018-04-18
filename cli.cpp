#include "cli.h"
#include "vector.h"
#include "address_book.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <functional>
#include <map>

namespace company {

int Cli::Run(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
        case 'h': {
          return PrintUsage();
        }
        default: {
          return PrintUsage();
        }
      }
    } else if (i + 1 == argc) {
      return RunInteractive(argv[i]);
    }
  }
  return PrintUsage();
}

int Cli::PrintUsage() {
  std::cout << "Usage: address_book [options] file" << std::endl;
  return 0;
}

int Cli::RunInteractive(const char* filename) {
  // Requirement 3.a:
  std::ifstream csv_input(filename);
  if (!csv_input.is_open())
  {
    std::cout << "Invalid filename" << std::endl;
    return -1;
  }

  AddressBook address_book;
  address_book.Load(csv_input);

  std::string command_help = "Commands: list, search [name], delete [name], unittest, quit";
  std::cout << command_help << std::endl;
  while (true) {
    std::string command;
    std::cin >> command;
    if (command == "quit") {
      break;
    } else if (command == "list") {
      CommandList(address_book);
    } else if (command == "search") {
      CommandSearch(address_book);
    } else if (command == "delete") {
      CommandDelete(address_book);
    } else if (command == "unittest") {
      CommandUnitTestAll();
    } else {
      std::cout << command_help << std::endl;
    }
  }

  // Requirement 4:
  csv_input.close();
  std::ofstream csv_output(filename);
  address_book.Save(csv_output);

  return 0;
}

// Requirement 3.b:
void Cli::CommandList(AddressBook& address_book) {
  address_book.Sort();
  address_book.StringPrint(std::cout);
}

// Requirement 3.c:
void Cli::CommandSearch(AddressBook& address_book) {
  Contact contact;
  std::string name;
  std::cin >> name;
  if (address_book.FindContact(name, contact)) {
    contact.StringPrint(std::cout);
  }
}

// Requirement 3.d:
void Cli::CommandDelete(AddressBook& address_book) {
  std::string name;
  std::cin >> name;
  if (address_book.DeleteContact(name)) {
    std::cout << "Deleted contact" << std::endl;
  }
  else {
    std::cout << "Unable to find contact" << std::endl;
  }
}

// unit testing would be in a separate project on a real application
void Cli::CommandUnitTestAll() {
  std::map<std::string, std::function<bool()>> tests;
  tests.emplace("Use case A", std::bind(&Cli::CommandUnitTestA, this));
  tests.emplace("Use case B", std::bind(&Cli::CommandUnitTestB, this));
  tests.emplace("Use case C", std::bind(&Cli::CommandUnitTestC, this));
  tests.emplace("Use case D", std::bind(&Cli::CommandUnitTestD, this));

  for (auto& test : tests) {
    std::cout << "Running [" << test.first << "] test" << std::endl;
    if (test.second()) {
      std::cout << "Result: success" << std::endl << std::endl;
    }
    else {
      std::cout << "Result: failure" << std::endl << std::endl;
      return;
    }
  }

  std::cout << "All unit tests completed successfully" << std::endl << std::endl;
}

bool Cli::CommandUnitTestA() {
  // arrange
  std::stringstream csv_input;
  AddressBook address_book;
  Contact contact;
  Vector<std::string> expectedNumbers;
  expectedNumbers.push_back("(420)798-1076");
  expectedNumbers.push_back("(347)725-4471");

  csv_input << "Name" << "," << "Phone" << std::endl;
  csv_input << "Nicholas" << "," << "(420)798-1076" << std::endl;
  csv_input << "Nicholas" << "," << "(347)725-4471" << std::endl;
  csv_input << "Barry" << "," << "(580)776-2009" << std::endl;

  // act
  address_book.Load(csv_input);

  // assert
  return address_book.FindContact("Nicholas", contact) && contact.GetNumbers() == expectedNumbers;
}

bool Cli::CommandUnitTestB() {
  // arrange
  std::stringstream csv_input;
  std::stringstream print_output;
  AddressBook address_book;

  csv_input << "Name" << "," << "Phone" << std::endl;
  csv_input << "Nicholas" << "," << "(420)798-1076" << std::endl;
  csv_input << "Nicholas" << "," << "(347)725-4471" << std::endl;
  csv_input << "Barry" << "," << "(580)776-2009" << std::endl;
  address_book.Load(csv_input);

  // act
  address_book.Sort();
  address_book.StringPrint(print_output);
  std::string result = print_output.str();

  // assert
  // not a great assertion, but should be sufficient while avoiding exposing implementation details
  return result.find("Barry") < result.find("Nicolas");
}

bool Cli::CommandUnitTestC() {
  // arrange
  std::stringstream csv_input;
  AddressBook address_book;
  Contact contact1;
  Contact contact2;

  csv_input << "Name" << "," << "Phone" << std::endl;
  csv_input << "Nicholas" << "," << "(420)798-1076" << std::endl;
  csv_input << "Nicholas" << "," << "(347)725-4471" << std::endl;
  csv_input << "Barry" << "," << "(580)776-2009" << std::endl;
  address_book.Load(csv_input);

  // act
  bool ret1 = address_book.FindContact("Nicholas", contact1);
  bool ret2 = address_book.FindContact("Nicholas231", contact2);

  // assert
  return ret1 && !ret2 && contact1.GetName() == "Nicholas";
}

bool Cli::CommandUnitTestD() {
  // arrange
  std::stringstream csv_input;
  AddressBook address_book;
  Contact contact;

  csv_input << "Name" << "," << "Phone" << std::endl;
  csv_input << "Nicholas" << "," << "(420)798-1076" << std::endl;
  csv_input << "Nicholas" << "," << "(347)725-4471" << std::endl;
  csv_input << "Barry" << "," << "(580)776-2009" << std::endl;
  address_book.Load(csv_input);

  // act
  address_book.DeleteContact("Nicholas");

  // assert
  return !address_book.FindContact("Nicholas", contact) && address_book.FindContact("Barry", contact);
}

} // namespace company
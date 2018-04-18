#pragma once

#include "address_book.h"

namespace company {

class Cli {
public:
  int Run(int argc, char** argv);

private:
  int PrintUsage();
  int RunInteractive(const char* filename);
  void CommandList(AddressBook& address_book);
  void CommandSearch(AddressBook& address_book);
  void CommandDelete(AddressBook& address_book);
  void CommandUnitTestAll();
  bool CommandUnitTestA();
  bool CommandUnitTestB();
  bool CommandUnitTestC();
  bool CommandUnitTestD();
};

} // namespace company
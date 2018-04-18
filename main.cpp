#include "cli.h"

int main(int argc, char** argv) {
  company::Cli cli;
  return cli.Run(argc, argv);
}
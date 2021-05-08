#include "driver.hpp"
#include "parser.hpp"
#include "stdlib.h"
#include <string.h>

int main(){
  saltyfish::Driver driver;
  return driver.parse("test.cpp");
}
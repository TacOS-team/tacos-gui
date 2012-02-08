#include <iostream>



int main() {
  std::cout << "ok" << std::endl;
#ifdef DEBUG
  std::cout << "DEBUG" << std::endl;
#endif
}

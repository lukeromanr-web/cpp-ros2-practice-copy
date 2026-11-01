#include <iostream>  // std::cout
#include <string>    // std::string、std::to_string

int main() {
  for (int count = 0; count < 3; ++count) {
    const std::string message = "Hello, world! " + std::to_string(count);
    std::cout << "[topic] " << message << '\n';
  }
  return 0;
}

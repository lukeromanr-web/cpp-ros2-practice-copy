#include <iostream>
#include <string>

class Node {
public:
  explicit Node(const std::string &name) : name_(name) {}
private:
  std::string name_;
};

class MinimalPublisher : public Node {
public:
  MinimalPublisher() : Node("minimal_publisher") {}

  void send() {
    const std::string message = "Hello, world! " + std::to_string(count_++);
    std::cout << "[topic] " << message << '\n';
  }

private:
  int count_{0};
};

int main() {
  MinimalPublisher node;
  for (int i = 0; i < 3; ++i) {
    node.send();
  }
  return 0;
}

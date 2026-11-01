#include <iostream>
#include <memory>  // std::shared_ptr、std::make_shared
#include <string>

template <typename T>
class Publisher {
public:
  explicit Publisher(const std::string &topic) : topic_(topic) {}

  void publish(const T &message) const {
    std::cout << '[' << topic_ << "] " << message << '\n';
  }

private:
  std::string topic_;
};

class Node {
public:
  explicit Node(const std::string &name) : name_(name) {}

  template <typename T>
  std::shared_ptr<Publisher<T>> create_publisher(const std::string &topic) {
    return std::make_shared<Publisher<T>>(topic);
  }

private:
  std::string name_;
};

class MinimalPublisher : public Node {
public:
  MinimalPublisher() : Node("minimal_publisher") {
    publisher_ = this->create_publisher<std::string>("topic");
  }

  void send() {
    const std::string message = "Hello, world! " + std::to_string(count_++);
    publisher_->publish(message);
  }

private:
  std::shared_ptr<Publisher<std::string>> publisher_;
  int count_{0};
};

int main() {
  MinimalPublisher node;
  for (int i = 0; i < 3; ++i) {
    node.send();
  }
  return 0;
}

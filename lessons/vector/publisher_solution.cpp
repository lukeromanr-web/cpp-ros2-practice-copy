#include <iostream>
#include <memory>
#include <string>
#include <vector>

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
  template <typename T>
  std::shared_ptr<Publisher<T>> create_publisher(const std::string &topic) {
    return std::make_shared<Publisher<T>>(topic);
  }
};

class MinimalPublisher : public Node {
public:
  MinimalPublisher() {
    publisher_ = this->create_publisher<std::string>("message");
  }

  void send() {
    for (const std::string &message : messages_) {
      publisher_->publish(message);
    }
  }

private:
  std::vector<std::string> messages_{"Apple", "Banana", "Cherry"};
  std::shared_ptr<Publisher<std::string>> publisher_;
};

int main() {
  MinimalPublisher node;
  node.send();
  return 0;
}

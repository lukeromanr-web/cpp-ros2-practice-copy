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
    // TODO 2：创建 Publisher<std::string>，话题名为 message。
  }

  void send() {
    if (!publisher_) {
      return;  // 未完成时保护空指针。
    }
    // TODO 3：遍历 messages_，把每个字符串交给 publisher_->publish。
  }

private:
  // TODO 1：把此空 vector 初始化为 Apple、Banana、Cherry。
  std::vector<std::string> messages_;
  std::shared_ptr<Publisher<std::string>> publisher_;
};

int main() {
  MinimalPublisher node;
  node.send();
  std::cerr << "TODO unfinished\n";
  return 2;  // TODO 4：完成后删除诊断行，改为 return 0。
}

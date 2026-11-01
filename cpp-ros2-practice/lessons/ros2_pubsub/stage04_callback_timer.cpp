#include <chrono>      // 500ms
#include <functional>  // std::function、std::bind
#include <iostream>
#include <memory>
#include <string>
#include <thread>      // std::this_thread::sleep_for
#include <utility>     // std::move

using namespace std::chrono_literals;

// 教学消息：普通 C++ 结构体；第五阶段再使用真实 std_msgs 消息。
namespace demo_msgs {
namespace msg {
struct String {
  std::string data;
};
}  // namespace msg
}  // namespace demo_msgs

template <typename T>
class Publisher {
public:
  explicit Publisher(const std::string &topic) : topic_(topic) {}

  void publish(const T &message) const {
    std::cout << '[' << topic_ << "] " << message.data << '\n';
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

class SimpleTimer {
public:
  explicit SimpleTimer(std::function<void()> callback)
      : callback_(std::move(callback)) {}

  void run_three_times() {
    for (int i = 0; i < 3; ++i) {
      std::this_thread::sleep_for(500ms);
      callback_();  // 等待结束之后，才在当前线程调用。
    }
  }

private:
  std::function<void()> callback_;
};

class MinimalPublisher : public Node {
public:
  MinimalPublisher() : Node("minimal_publisher") {
    publisher_ = this->create_publisher<demo_msgs::msg::String>("topic");
    timer_ = std::make_shared<SimpleTimer>(
        std::bind(&MinimalPublisher::timer_callback, this));
  }

  void run_three_times() {
    timer_->run_three_times();
  }

private:
  void timer_callback() {
    demo_msgs::msg::String message;
    message.data = "Hello, world! " + std::to_string(count_++);
    publisher_->publish(message);
  }

  std::shared_ptr<Publisher<demo_msgs::msg::String>> publisher_;
  std::shared_ptr<SimpleTimer> timer_;
  int count_{0};
};

int main() {
  MinimalPublisher node;
  node.run_three_times();
  return 0;
}

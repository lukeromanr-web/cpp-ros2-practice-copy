#include <cstddef>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

template <typename T>
T absoluteDifference(T a, T b) {
  return a >= b ? a - b : b - a;
}

class SpeedMonitorLogic {
public:
  SpeedMonitorLogic(int threshold, std::size_t window)
      : threshold_(threshold), window_(window) {}

  void receive(int speed) {
    speed_queue_.push(speed);
    if (speed_queue_.size() < 2) {
      std::cout << "WAIT\n";
      return;
    }

    const int current = speed_queue_.front();
    const int diff = absoluteDifference(current, previous_speed_);
    recent_diffs_.push_back(diff);
    if (recent_diffs_.size() > window_) {
      recent_diffs_.erase(recent_diffs_.begin());
    }

    int warnings = 0;
    for (int value : recent_diffs_) {
      if (value >= threshold_) {
        ++warnings;
      }
    }
    std::cout << "process=" << current
              << " previous=" << previous_speed_
              << " diff=" << diff
              << " warnings=" << warnings
              << " state=" << (diff >= threshold_ ? "WARNING" : "NORMAL")
              << " system=" << (warnings >= 2 ? "UNSTABLE" : "STABLE")
              << '\n';

    previous_speed_ = current;
    speed_queue_.pop();
  }

private:
  int threshold_;
  std::size_t window_;
  std::queue<int> speed_queue_;
  std::vector<int> recent_diffs_;
  int previous_speed_{0};
};

int main(int argc, char *argv[]) {
  const bool v2 = argc == 2 && std::string(argv[1]) == "--v2";
  if (argc > 1 && !v2) {
    std::cerr << "usage: monitor_solution [--v2]\n";
    return 1;
  }
  SpeedMonitorLogic monitor(v2 ? 4 : 3, v2 ? 3 : 4);
  int speed = 0;
  while (std::cin >> speed) {
    if (speed < 0 || speed > 100) {
      std::cerr << "speed must be in [0,100]\n";
      return 1;
    }
    monitor.receive(speed);
  }
  if (!std::cin.eof()) {
    std::cerr << "invalid input\n";
    return 1;
  }
  return 0;
}

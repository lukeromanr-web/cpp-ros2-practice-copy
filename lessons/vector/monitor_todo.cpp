#include <cstddef>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

template <typename T>
T absoluteDifference(T a, T b) {
  // TODO 1：返回两数差值的绝对值；本题速度范围为 0～100。
  (void)a;
  (void)b;
  return 0;  // 占位；编译成功不表示逻辑正确。
}

class SpeedMonitorLogic {
public:
  SpeedMonitorLogic(int threshold, std::size_t window)
      : threshold_(threshold), window_(window) {}

  void receive(int speed) {
    (void)speed;
    // TODO 2：入队；不足 2 条输出 WAIT 并 return。
    // TODO 3：读取队头，用 absoluteDifference 求本次变化量。
    // TODO 4：push_back 变化量；超过 window_ 时删除首元素。
    // TODO 5：遍历 vector，统计 >= threshold_ 的数量并按题目输出。
    // TODO 6：更新 previous_speed_，最后 pop 已处理队头。
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
    std::cerr << "usage: monitor_todo [--v2]\n";
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
  std::cerr << "TODO unfinished\n";
  return 2;  // TODO 7：完成后删除诊断行，改为 return 0。
}

# ROS2 + vector 三面备考大纲（按实际写题的顺序）

一句话：工程能运行是门槛，回调中的 queue + 模板 + vector 是得分核心。

## 一、ROS2 考核：基础到应用（已知题目范围）

### A. 工程层：必须实操
- [ ] 确认环境：`ls /opt/ros`；`source /opt/ros/<版本>/setup.bash`
- [ ] 工作空间：`~/ros2_exam_ws/src/robot_speed_pkg/`
- [ ] 功能包：`ros2 pkg create robot_speed_pkg --build-type ament_cmake --dependencies rclcpp std_msgs`
- [ ] `package.xml`：登记依赖；`CMakeLists.txt`：寻找依赖、声明执行目标、安装可执行文件
- [ ] 终端执行：`colcon build` → `source install/setup.bash` → `ros2 run`；两个终端分别运行
- [ ] 排错：找不到 ros2、包、可执行文件、头文件、话题通信不通

### B. ROS2 代码框架：熟悉并能改名字，不必重造底层
- [ ] `rclcpp::init`、`rclcpp::spin`、`rclcpp::shutdown`
- [ ] `class Xxx : public rclcpp::Node`；构造函数中的 `Node("节点名")`
- [ ] 发布方：`create_publisher<MessageT>("topic", 10)` + `publisher_->publish(message)`
- [ ] 定时器：`create_wall_timer(1s, std::bind(&Class::callback, this))`
- [ ] 订阅方：`create_subscription<MessageT>("topic", 10, std::bind(&Class::callback, this, std::placeholders::_1))`
- [ ] 回调：收到一条消息才运行一次；`spin` 保持节点持续处理回调
- [ ] `std_msgs::msg::Int32 message; message.data = 2;`；消息对象 vs data 字段
- [ ] 共享指针：`Publisher<...>::SharedPtr` / `Subscription<...>::SharedPtr` / `TimerBase::SharedPtr` 位置会抄会改即可
- [ ] 必改的三项：**话题名、消息类型、处理数据代码**；发布订阅两端消息类型应一致
- [ ] `ros2 topic list` / `ros2 topic echo /speed` / `ros2 topic info /speed` 做检查

### C. 可选扩展：仅题目明确要求时才投入
- 自定义 `.msg` / `rosidl_generate_interfaces`
- service/action、参数、QoS 细节、Executor/线程模型、TF、仿真、复杂跨包链接
- 这些出现在 GitBook 课程里不等于这份仿真题要求全部掌握；本题使用标准 Int32，无需新建自定义消息包

## 二、vector 知识链条（重点是变成能写的语法）

### 1. 声明、初始化、访问
```cpp
#include <vector>
std::vector<int> v;          // 空 vector
std::vector<int> v2{2, 5, 4}; // 已有 3 个元素
std::vector<int> v3(4, 0);  // 四个 0（不是 {4,0}）
int x = v2[0];              // 下标访问，不检查越界
int y = v2.at(0);           // 检查越界，越界抛异常
int n = static_cast<int>(v2.size());
```

### 2. 增删、修改
```cpp
v.push_back(3);             // 尾部加入
v.pop_back();               // 尾部删除；空容器不能调用
v.push_back(2);
v.push_back(5);
v.erase(v.begin());        // 删除最前面的元素；需要非空
v[0] = 10;                 // 修改已有元素
v.clear();                  // 清空
```

### 3. 遍历与统计（考试要会独立写）
```cpp
int warnings = 0;
for (int x : v) {
  if (x >= 3) ++warnings;
}
// 也可用 std::count_if(v.begin(), v.end(), [](int x){ return x >= 3; });
```

### 4. 容量与迭代器
- [ ] `size()` = 已存元素个数；`capacity()` = 当前已分配的容量；`empty()` = 是否为空
- [ ] `begin()` 指向首元素；`end()` 指向末尾后的位置，不能直接解引用
- [ ] `reserve(n)` 预留容量，不会把 size 变成 n；`resize(n)` 改变元素个数
- [ ] `erase` 会改变后续元素的位置，相关迭代器/引用可能失效
- [ ] `push_back` 如果发生重新分配，也会使迭代器/引用/指针失效

### 5. 最典型四种考法
- A. 输入一串整数 → push_back → 遍历求和/最大值/计数
- B. 只保留最近 K 个 → push_back → if(size > K) erase(begin())
- C. 找到条件元素 → for/迭代器/`count_if`；数 WARNING
- D. 容器访问/修改 → `[]`/`at()` vs `push_back()`；空容器越界

### 6. 高频易错比较
| 项                                | 区别                   |
| -------------------------------- | -------------------- |
| `vector<int> v(4,0)` vs `v{4,0}` | 四个 0 vs 两个数 4、0      |
| `v[i]` vs `v.at(i)`              | 不做边界检查 vs 做边界检查      |
| `size` vs `capacity`             | 已有元素数 vs 可容纳元素容量     |
| `push_back(x)` vs `v[i]=x`       | 新增元素 vs 修改已存在元素      |
| `erase(begin())` vs `pop_back()` | 删除首元素 vs 删除尾元素       |
| `vector` vs `queue`              | 可随机访问 vs FIFO 仅队头处理  |
| `vector` vs `map`                | 连续序列下标访问 vs 键值映射按键查找 |

## 三、queue / 模板 / thread 与 vector 的结合

### queue（本题必备）
```cpp
#include <queue>
std::queue<int> q;
q.push(2); q.push(5);
if (q.size() >= 2) { int earliest = q.front(); q.pop(); }
```
`front()` 先读、`pop()` 再删；`pop()` 不返回被删除元素。

### template（本题必备）
```cpp
template<typename T>
T absoluteDifference(T a, T b) {return a >= b ? a - b : b - a;}
int a = absoluteDifference(5, 2);
double b = absoluteDifference(5.5, 2.0);
```

### thread（加分，先做基础再做）
```cpp
#include <thread>
std::thread worker(checkSpeedChange, current_speed, previous_speed);
worker.join(); // 等待工作线程完成
```
注意避免线程同时改容器数据；本仿真题特别指出不要求复杂互斥。

## 四、应试最短执行链

题目要求 → 确认消息类型/话题/频率 → 复制 ROS2 发布+订阅模板 → 把业务逻辑写进回调 → 配 CMake → colcon build → ros2 run → 对照输出验收。

优先级：① `queue + vector + 模板` 代码独立写通；② ROS2 构建与运行；③ ROS2 套模板修改；④ 线程加分；⑤ 其他 ROS2 深入机制。

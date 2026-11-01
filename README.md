# cpp-ros2-practice

**一句话判定：一个练习单元 = 题目 + 可编辑源码 + 验收标准 + 一次有意义的提交。**

面向 11 月 3 日 C++ / ROS2 三面：先写通普通 C++，再套入 ROS2 发布订阅框架。
本仓库按此前教学路线重新编写起步代码；保留阶段概念，不声明逐字还原旧版。

## 目录与文件入口

```text
cpp-ros2-practice/
├── README.md
├── CMakeLists.txt                     # 仅构建纯 C++ 练习
├── .gitignore
├── docs/
│   ├── file-map.md                    # 首批每个文件的用途
│   ├── workflow.md                    # WSL / CMake / Git 操作链
│   ├── progress.md                    # 每次练习的记录模板
│   └── validation.md                  # 已验证项与待验证项
├── lessons/
│   ├── ros2_pubsub/
│   │   ├── README.md                  # 五阶段任务、执行链与对应关系
│   │   ├── stage01_plain.cpp
│   │   ├── stage02_class.cpp
│   │   ├── stage03_template_ptr.cpp
│   │   └── stage04_callback_timer.cpp
│   └── vector/
│       ├── README.md                  # 基础题 → 综合题 → V2
│       ├── publisher_todo.cpp
│       ├── publisher_solution.cpp
│       ├── monitor_todo.cpp
│       ├── monitor_solution.cpp
│       └── cases/
│           ├── base.in
│           ├── base.out
│           ├── v2.in
│           └── v2.out
├── ros2_ws/
│   └── src/topic_practice/
│       ├── package.xml
│       ├── CMakeLists.txt
│       └── src/
│           ├── stage05_publisher.cpp
│           └── stage05_subscriber.cpp
└── scripts/
    └── verify_cpp.py
```

完整文件清单：[docs/file-map.md](docs/file-map.md)。

## 两个可持续提交的练习单元

| 单元 | 最小改动 | 验收 | 下一次提交 |
|---|---|---|---|
| [发布订阅五阶段](lessons/ros2_pubsub/README.md) | 每次完成一个阶段的迁移题 | 前四阶段固定三行；第五阶段两终端通信 | 改消息内容 → 改频率 → 两端统一改话题 |
| [vector 综合题](lessons/vector/README.md) | 完成一个 TODO | 固定输入输出、边界和退出码 | 字符串遍历 → 速度监测 → V2 参数 → 迁入回调 |

学习代码 `*_todo.cpp` 可直接修改；`*_solution.cpp` 用于参考。初始 TODO 能编译，但会以退出码 2 标记尚未完成。

## 快速开始：纯 C++

在 WSL/Ubuntu 中把本目录放到 `~/cpp-ros2-practice`。依赖：g++、CMake ≥ 3.20、Python 3。

```bash
cd ~/cpp-ros2-practice
code .
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/bin/stage03
```

`ctest` 验证教学示例和参考答案。完成自己的 TODO 后，另运行：

```bash
cmake --build build --target vector_publisher_todo monitor_todo
python3 scripts/verify_cpp.py --bin build/bin --mode practice
```

**完成标准：practice 全部通过 + 能解释自己的输入 → 处理 → 输出。**

综合题单独运行：

```bash
./build/bin/monitor_solution < lessons/vector/cases/base.in
./build/bin/monitor_solution --v2 < lessons/vector/cases/v2.in
```

## 第五阶段：真实 ROS2

依赖：已安装 ROS2、colcon、rclcpp、std_msgs。命令以 **Humble** 为例；先 `ls /opt/ros`，按实际发行版替换 `humble`。本包采用 Humble / Jazzy 常见的 ament_cmake 写法；其他版本需核对对应文档。

```bash
source /opt/ros/humble/setup.bash
cd ~/cpp-ros2-practice/ros2_ws
colcon build --packages-select topic_practice
source install/setup.bash
ros2 pkg prefix topic_practice
ros2 pkg executables topic_practice
```

最后两条应指向本仓库的 `ros2_ws/install`，并列出 `stage05_publisher`、`stage05_subscriber`。

**终端 A（先开订阅者）：**

```bash
source /opt/ros/humble/setup.bash
cd ~/cpp-ros2-practice/ros2_ws
source install/setup.bash
ros2 run topic_practice stage05_subscriber
```

**终端 B：**

```bash
source /opt/ros/humble/setup.bash
cd ~/cpp-ros2-practice/ros2_ws
source install/setup.bash
ros2 run topic_practice stage05_publisher
```

**验收：**A 持续显示 `Received: 'Hello, world! ...'`；B 持续显示 `Publishing: ...`。接收首条编号可能大于 0；双方 Ctrl+C 停止。

排查时第三个已加载相同环境的终端运行：

```bash
ros2 topic list
ros2 topic info /topic
ros2 topic echo /topic
```

预期类型为 `std_msgs/msg/String`，只启动这两个节点时有一个发布者、一个订阅者；运行 echo 会新增一个订阅者。

## 提交与首次上传

每次：**读题 → 改一个点 → 编译 → 验收 → 写进度 → 查看 diff → 提交**。

从空仓库首次上传的方法、分批提交命令：[docs/workflow.md](docs/workflow.md)。

验证范围见 [docs/validation.md](docs/validation.md)；进度模板见 [docs/progress.md](docs/progress.md)。

# 单元 01：普通 C++ → 真实 ROS2 发布订阅

**一句话判定：每阶段增加一层结构；始终追踪“谁调用谁、消息放在哪里”。**

此处按之前的五阶段思路重写完整示例。阶段 1～4 是本地教学程序；阶段 5 使用 ROS2。

| 阶段 | 文件 / 目标 | 新增结构 | 能回答才过关 |
|---|---|---|---|
| 1 | stage01_plain.cpp / stage01 | string、循环、输出 | count 怎样进入字符串？ |
| 2 | stage02_class.cpp / stage02 | Node、继承、对象成员 | main 调用谁？count_ 属于哪个对象？ |
| 3 | stage03_template_ptr.cpp / stage03 | 类模板、函数模板、智能指针 | std::string 怎样一路传给 T？ |
| 4 | stage04_callback_timer.cpp / stage04 | 消息 data、bind、定时器回调 | 谁保存回调、谁在什么时候执行？ |
| 5 | ros2_ws 包中的两个 .cpp | rclcpp、ROS2 定时器、订阅、spin | 发布与订阅怎样通过相同话题/类型配对？ |

## 先运行，再做最小迁移

仓库根目录中：

```bash
cmake -S . -B build
cmake --build build --target stage01 stage02 stage03 stage04
./build/bin/stage01
./build/bin/stage02
./build/bin/stage03
./build/bin/stage04
python3 scripts/verify_cpp.py --bin build/bin --mode teaching
```

每个程序都输出三行：

```text
[topic] Hello, world! 0
[topic] Hello, world! 1
[topic] Hello, world! 2
```

阶段 4 每行之前等待约 500ms。

| 每次提交一个迁移 | 修改位置 | 验收方式 |
|---|---|---|
| 1：前缀改为 Robot | stage01 字符串 | 三行顺序与编号正确 |
| 2：把输出封装到 send | stage02 类 | 能写出 main → send → 输出 |
| 3：给模板发布器发送 double | stage03 新建独立发布器对象 | `create_publisher<double>` → `Publisher<double>` → `publish(double)`；管理不同类型的智能指针分别声明 |
| 4：次数改 5，间隔改 200ms | stage04 SimpleTimer | 输出五行，解释每次等待后如何调用回调 |
| 5：真实话题名统一改为 robot_text | 第五阶段两端 | `/robot_text` 为 String，两个节点通信 |

迁移题会改变输出；按新要求更新脚本中的对应预期，或把迁移保存在新文件、新目标中并新增验收。原始验收固定检查默认示例。

## 最核心的类型链

`create_publisher<X>(...) → Publisher<X> → shared_ptr<Publisher<X>> → publish(const X&)`

原理：X 是编译时类型；工厂函数把相同 X 传给类模板。智能指针管理的是 `Publisher<X>` 对象，`publish` 接收的是 X 类型的消息。

`message → message.data → publisher_->publish(message)`

原理：message 是整个消息对象，data 是它内部的字段；先写字段，再把整个对象交给发布器。

## 阶段 4：分开观察结构与执行

| 成员属于谁 | 成员 | 保存什么 |
|---|---|---|
| MinimalPublisher 对象 | publisher_ | 管理发布器对象的智能指针 |
| MinimalPublisher 对象 | timer_ | 管理 SimpleTimer 对象的智能指针 |
| SimpleTimer 对象 | callback_ | 可调用任务 |

创建链：`MinimalPublisher 构造 → 创建 publisher → bind(成员函数, this) → SimpleTimer 保存 callback_`。

执行链：`main → node.run_three_times → timer_->run_three_times → for → sleep_for 返回 → callback_() → node.timer_callback → 写 message.data → publish`。

原理：保存函数入口与执行函数是两个时刻。`sleep_for` 阻塞当前线程；当前线程等待期间不会执行这里后面的语句。这个教学定时器没有创建新线程。

## 阶段 4 → 阶段 5 对照

| 教学结构 | 真实 ROS2 对应 |
|---|---|
| 自定义 Node | rclcpp::Node |
| demo_msgs::msg::String | std_msgs::msg::String |
| shared_ptr<Publisher<X>> | rclcpp::Publisher<X>::SharedPtr |
| make_shared<SimpleTimer>(bind(...)) | create_wall_timer(500ms, bind(...)) |
| run_three_times | spin 持续调度就绪回调 |
| 本地终端打印 | publish 交给 ROS2；另一个节点订阅并打印 |

真实执行链：`init → 创建节点并登记定时器/订阅 → spin 调度 → 回调执行 → 回调返回继续 spin → Ctrl+C → shutdown`。

原理：定时器到期或消息到达，会使相应回调就绪；Executor 在 spin 中调度。此例默认单线程执行器的回调依次执行。

第五阶段源码与两终端命令见根 [README](../../README.md#第五阶段真实-ros2)。验收至少包含：编译成功、两个节点运行、topic info 类型匹配、订阅者实际收到数据。

## 用到的头文件

| 头文件 | 提供的积木 |
|---|---|
| iostream / string | 输出、字符串、to_string |
| memory | shared_ptr、make_shared |
| vector | 可增长序列；下一单元使用 |
| functional | bind、function、placeholders::_1 |
| chrono / thread | 时间单位、当前线程等待 |
| rclcpp/rclcpp.hpp | 真实 Node、发布订阅、spin |
| std_msgs/msg/string.hpp | 真实 String 消息类型及 data 字段 |

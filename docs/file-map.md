# 首批文件清单

**一句话判定：源码负责行为，构建文件负责产物规则，验收文件负责判断结果。**

路径均相对仓库根目录；共 26 个文件。

| 文件 | 用途 |
|---|---|
| `README.md` | 仓库总入口、目录树、构建运行、验收入口 |
| `CMakeLists.txt` | 声明 8 个纯 C++ 目标、线程依赖、安装规则、CTest 入口 |
| `.gitignore` | 忽略各层 build / install / log、编辑器和临时产物；保留源码与样例 |
| `docs/file-map.md` | 本文件：逐项说明首批文件 |
| `docs/workflow.md` | 声明层 / 执行层、WSL 路径、首次上传和持续提交 |
| `docs/progress.md` | TODO 完成情况与一次练习的记录模板 |
| `docs/validation.md` | 生成时的实际验证证据和验证限制 |
| `lessons/ros2_pubsub/README.md` | 五阶段任务、语法对应、回调执行链 |
| `lessons/ros2_pubsub/stage01_plain.cpp` | 普通字符串输出：建立数据流起点 |
| `lessons/ros2_pubsub/stage02_class.cpp` | Node / MinimalPublisher、继承、构造、对象成员 |
| `lessons/ros2_pubsub/stage03_template_ptr.cpp` | Publisher<T>、工厂函数模板、shared_ptr |
| `lessons/ros2_pubsub/stage04_callback_timer.cpp` | 消息对象、data、bind、SimpleTimer 同步调用 |
| `lessons/vector/README.md` | 字符串遍历题、queue + 模板 + vector 综合题、V2 要求 |
| `lessons/vector/publisher_todo.cpp` | 学习者填空：容器初始化、创建发布器、遍历发送 |
| `lessons/vector/publisher_solution.cpp` | 字符串发布练习参考答案 |
| `lessons/vector/monitor_todo.cpp` | 学习者填空：排队、差值、滑动窗口、异常统计 |
| `lessons/vector/monitor_solution.cpp` | 完整速度监测逻辑，命令行 --v2 切换题目参数 |
| `lessons/vector/cases/base.in` | 基础题输入：2 5 4 8 3 6 |
| `lessons/vector/cases/base.out` | 基础题逐行预期输出 |
| `lessons/vector/cases/v2.in` | V2 输入：3 7 4 9 2 |
| `lessons/vector/cases/v2.out` | V2 逐行预期输出 |
| `scripts/verify_cpp.py` | 执行样例并核对完整输出与退出码，含边界用例 |
| `ros2_ws/src/topic_practice/package.xml` | ROS2 包名、ament_cmake 构建类型、rclcpp/std_msgs 依赖 |
| `ros2_ws/src/topic_practice/CMakeLists.txt` | 两个 ROS2 目标、链接依赖、安装、ament_package |
| `ros2_ws/src/topic_practice/src/stage05_publisher.cpp` | 500ms 定时发布 std_msgs::msg::String |
| `ros2_ws/src/topic_practice/src/stage05_subscriber.cpp` | 订阅同一话题，回调读取 message.data |

# 交付验证记录

日期：2026-11-01。

| 范围 | 状态 | 证据 |
|---|---|---|
| 前四阶段与四份 vector 源码 | 8/8 编译成功，零警告 | g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread |
| 教学与参考答案验收 | 17/17 通过 | python3 scripts/verify_cpp.py --bin build/bin --mode all |
| 初始 TODO | 有意留空，未声明作答完成 | practice 为 4/13；4 项输入拒绝检查通过，9 项功能检查指出 TODO 未完成 |
| CMake 配置 / CTest / 安装 | 当前环境无 cmake、ctest | 用户 WSL 按根 README 执行 |
| ROS2 编译和节点通信 | 当前环境无 ROS2 | 用户 WSL 按根 README 两终端验收 |
| package.xml / Python / 文件清单 | 通过 | XML 与 Python 语法解析、26 个路径逐项匹配、Markdown 本地链接检查 |
| GitHub 远程仓库 | 未创建 | 当前连接无创建仓库接口；提供起步包 |

脚本结果只覆盖已运行的纯 C++ 程序。当前没有执行 CMake 配置、CTest 或安装命令；这些命令与 ROS2 验收需要在用户的 Ubuntu / WSL 中完成。测试产物未放入交付包。

17 项分别为：4 个教学阶段；字符串遍历；基础题；V2；V2 窗口过期；空输入；单条输入；相同速度；0/100 边界；基础题窗口过期；负数、超范围、文本、未知选项拒绝。

官方参考：

- [ROS2 Humble：C++ 发布订阅教程](https://docs.ros.org/en/humble/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html)
- [ROS2 Humble：发布者示例源码](https://github.com/ros2/examples/blob/humble/rclcpp/topics/minimal_publisher/member_function.cpp)
- [ROS2 Humble：订阅者示例源码](https://github.com/ros2/examples/blob/humble/rclcpp/topics/minimal_subscriber/member_function.cpp)
- [CMake / CTest 3.22 文档](https://cmake.org/cmake/help/v3.22/manual/ctest.1.html)

# ROS2 三面开卷模板（基于机器人速度监测仿真题）

本包为 **ROS2 改写**，不是原题所写的 ROS1/catkin 版本。
ctrl c停止发布

## 目录

```
ros2_exam_bundle/
  robot_speed_pkg/
    package.xml                   # 依赖声明
    CMakeLists.txt                # 编译、链接、安装声明
    src/
      speed_publisher.cpp         # 已完成：每秒循环发布 2 5 4 8 3 6
      speed_monitor.cpp           # 已完成：queue + 模板 + vector + 状态统计
    templates/
      speed_publisher_TODO.cpp    # 修改练习模板
      speed_monitor_TODO.cpp      # 练习模板，专门留了数据处理 TODO
      vector_queue_practice.cpp  # 脱离 ROS2 的独立 C++ 练习
  ROS2与vector备考大纲.md
```

## 准备：确认 ROS2 已安装

```
ls /opt/ros
```

看到例如 `humble` 或 `jazzy` 才继续。以下以 humble 为例；发行版不同应替换路径。

## 操作链（执行层：终端）

```bash
source /opt/ros/humble/setup.bash
mkdir -p ~/ros2_exam_ws/src
# 解压本文件包后，把 robot_speed_pkg 文件夹复制到 ~/ros2_exam_ws/src/
cd ~/ros2_exam_ws
colcon build --packages-select robot_speed_pkg
source install/setup.bash
```

终端 A：

```bash
source /opt/ros/humble/setup.bash
cd ~/ros2_exam_ws
source install/setup.bash
ros2 run robot_speed_pkg speed_publisher
```

终端 B：

```bash
source /opt/ros/humble/setup.bash
cd ~/ros2_exam_ws
source install/setup.bash
ros2 run robot_speed_pkg speed_monitor
```

调试：

```bash
ros2 topic list
ros2 topic echo /speed
ros2 topic info /speed
```

## 声明层（不要和执行层混淆）

- `package.xml`: 声明 `rclcpp`、`std_msgs` 依赖。
- `CMakeLists.txt`: find_package → add_executable → ament_target_dependencies → install → ament_package。
- `.cpp`: 声明两个节点及其具体行为。
- `colcon build`: 读取上述工程声明、执行构建；`ros2 run`: 运行已安装的可执行程序。

## 常见错误

1. `ros2: command not found`：ROS2 未安装或没有 source /opt/ros/<发行版>/setup.bash。
2. `Package 'robot_speed_pkg' not found`：未编译成功或未 source 工作空间 install/setup.bash。
3. `No executable found`：检查 CMakeLists.txt 的 install(TARGETS ... DESTINATION lib/${PROJECT_NAME})。
4. 编译说缺 `std_msgs`：检查 package.xml 和 CMake 的 find_package/ament_target_dependencies。
5. 能启动但没收到数据：两个程序的话题名 `speed`、类型 `Int32` 必须一致；发布器与订阅器都要运行。
6. `std::bind` 报错：检查 `<functional>`、`std::placeholders::_1` 和回调形参。

## 单独练 C++（没有 ROS 环境也可以）

```bash
cd robot_speed_pkg/templates
g++ -std=c++17 -Wall -Wextra -pedantic vector_queue_practice.cpp -o practice
./practice
```

## 验证重点

- 收到 2：等待。
- 收到 5：开始处理 2；差值 2，NORMAL。
- 收到 4：开始处理 5；差值 3，WARNING。
- 收到 8：处理 4；差值 1，NORMAL。
- 收到 3：处理 8；差值 4，WARNING；最近 4 条内有 2 次 WARNING，UNSTABLE。

**限制：** 本文件包的标准 C++ 核心逻辑已用 g++ 验证；生成环境没有 ROS2 安装，ROS2 节点需要在你的 WSL/Ubuntu 中编译并实测。

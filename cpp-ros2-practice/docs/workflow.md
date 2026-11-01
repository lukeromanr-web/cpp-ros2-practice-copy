# WSL、构建与持续提交

## 1. 固定编辑位置

仓库放在 WSL 的 `~/cpp-ros2-practice`，在该目录执行 `code .`。VSCode 左下角应显示 WSL；新文件建在资源管理器里的仓库目录。

```bash
cd ~/cpp-ros2-practice
pwd
code .
```

| 地址 | 代表什么 | 日常操作 |
|---|---|---|
| `lessons/`、`ros2_ws/src/` | 手写源码 | 在这里修改并保存 |
| `build/` | 纯 C++ 构建产物 | 运行 `build/bin/目标名` |
| `ros2_ws/build/` | ROS2 中间构建产物 | 由 colcon 管理 |
| `ros2_ws/install/` | ROS2 安装产物 | source 后供 ros2 run 查找 |
| `ros2_ws/log/` | colcon 日志 | 构建失败时检查 |

## 2. 声明层：写规则

| 文件 | 最小规则链 | 作用 |
|---|---|---|
| 根 `CMakeLists.txt` | project → add_executable → install | 把纯 C++ 源码登记为可构建、可安装目标 |
| ROS2 `package.xml` | 包名 → 构建类型 → 依赖 | 登记功能包及依赖 |
| ROS2 `CMakeLists.txt` | find_package → add_executable → ament_target_dependencies → install → ament_package | 声明节点如何构建、链接、安装 |

这些声明被构建工具读取；写完声明后需要执行下面的命令才产生程序。

## 3. 执行层：终端产生状态变化

纯 C++ 主线：**保存源码 → 配置/生成 → 构建 → 验收 → 运行**。

| 命令（在仓库根目录执行） | 主体 / 对象 / 动作 | 读取 → 写入或结果 |
|---|---|---|
| `cmake -S . -B build` | cmake / 当前源码工程 / 配置生成 | CMakeLists → build 内构建规则与缓存 |
| `cmake --build build` | cmake 调用构建工具 / build 规则 / 编译链接 | .cpp → build/bin 下可执行文件 |
| `ctest --test-dir build --output-on-failure` | CTest / 已登记测试 / 运行检查 | 可执行文件与样例 → 测试结果 |
| `./build/bin/stage03` | shell 启动 / 指定程序 / 运行 | 可执行文件 → 终端输出 |
| `cmake --install build --prefix "$PWD/install"` | cmake / 已构建目标 / 安装 | build/bin → install/bin |

ROS2 主线（当前目录是 `ros2_ws`）：**加载基础环境 → colcon build → 加载本工作空间 → ros2 run**。

| 命令 | 读取 → 状态变化 |
|---|---|
| `source /opt/ros/humble/setup.bash` | 基础 ROS2 环境 → 当前终端能找到 ROS2 |
| `colcon build --packages-select topic_practice` | src 中包声明与源码 → build / install / log |
| `source install/setup.bash` | 本工作空间安装信息 → 当前终端找到本包 |
| `ros2 pkg prefix topic_practice` | 包索引 → 显示实际选中的安装路径 |
| `ros2 run topic_practice stage05_publisher` | 安装目录的可执行文件 → 运行节点 |

每个新终端都加载环境。构建失败时先修复第一条有效错误，再运行程序；已有旧二进制可能仍存在。要确认选中了本次产物，检查 `pwd`、构建结果、`ros2 pkg prefix`。

## 4. 第一次上传空仓库

GitHub 创建空仓库：名称 `cpp-ros2-practice`，建议初始设为 Private，不勾选生成 README / .gitignore / License。仓库有了以后再执行推送。

在包含这些首批文件的根目录运行：

```bash
git init -b main
git add README.md CMakeLists.txt .gitignore docs lessons ros2_ws/src/topic_practice scripts
git diff --cached
git commit -m "chore: initialize C++ and ROS2 practice units"
git remote add origin https://github.com/lukeromanr-web/cpp-ros2-practice.git
git push -u origin main
```

首次提交导入这套起步材料；之后自己的练习按下表分次提交。

若 Git 提示缺少身份，在本仓库配置自己的名字与邮箱：`git config user.name "你的名字"`、`git config user.email "你在 GitHub 使用的邮箱"`。通过 GitHub 官方认证流程登录；远程地址中保留普通 HTTPS 地址。

也可在 GitHub 仓库页选择 **Add file → Upload files**，上传解压后的内容；确保顶层直接是 README.md、lessons、ros2_ws 等。命令行上传能保留分批提交。

## 5. 后续每次只提交一个可解释的改动

| 次序 | 修改 | 验收 | 建议提交信息 |
|---|---|---|---|
| 1 | stage03 的消息前缀 | 更新对应预期并检查该阶段 | `practice: change stage03 message content` |
| 2 | 完成字符串发布 TODO | 脚本 practice 中 vector-strings 通过 | `practice: complete vector publisher traversal` |
| 3 | 完成速度监测 TODO | practice 全部通过 | `practice: complete rolling speed monitor` |
| 4 | 独立重写 V2 参数 | V2 输入输出 + 窗口过期用例通过 | `practice: verify V2 threshold and window` |
| 5 | ROS2 两端话题统一改名 | topic info + 两终端通信 | `practice: rename ROS2 topic on both nodes` |
| 6 | 新增速度话题节点、迁入处理逻辑 | 固定速度序列对照综合题 | `feat: integrate speed monitor with ROS2` |

```bash
git diff
git status --short
git add lessons/vector/publisher_todo.cpp docs/progress.md
git diff --cached
git commit -m "practice: complete vector publisher traversal"
git push
```

只在对应文件改动、验收后执行相应提交。修改预期输出的依据应是题目要求变化；验收失败时先检查代码。含截图或日志的材料只摘录必要证据到 progress.md。

## 6. 新练习复制这一套

| 新练习需要 | 放在哪里 |
|---|---|
| 题意、输入输出、判定公式 | 新单元 README.md |
| 自己写的代码 | 新单元 `*_todo.cpp` 或 `main.cpp` |
| 参考答案（需要时） | `*_solution.cpp` |
| 可重复验收 | cases + 脚本检查；在 CMake 登记新目标 |
| 本次结果 | docs/progress.md |

线程、自定义消息、服务、仿真留给后续独立单元；先完成当前题目要求。

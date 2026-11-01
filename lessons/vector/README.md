# 单元 02：vector 综合题

**一句话判定：容器保存状态，循环处理状态；先用固定输入输出写通，再迁入 ROS2 回调。**

## A. 从阶段 3 迁移：批量发送字符串

编辑 `publisher_todo.cpp`：

1. `messages_` 保存 Apple、Banana、Cherry，类型为 `vector<string>`。
2. 创建 `Publisher<string>` 对象，话题名为 `message`，由智能指针管理。
3. send 遍历容器，每个元素交给 publish；保留 publish 的 `const T&` 参数。
4. 完成后删除 unfinished 诊断并让 main 返回 0。

固定输出：

```text
[message] Apple
[message] Banana
[message] Cherry
```

数据链：`vector<string> → 当前元素 const string& → publish(const T&) 中 T=string → cout`。

原理：vector 存多个消息；发布器每次接收一个消息。传 const 引用让函数读取原元素而不修改它。

## B. 综合题：排队处理 + 变化量窗口

编辑 `monitor_todo.cpp`。输入是空白分隔的整数速度，范围 0～100；`previous_speed_` 初始为 0。

| 参数 | 基础题 | V2 |
|---|---|---|
| 输入样例 | 2 5 4 8 3 6 | 3 7 4 9 2 |
| 单次 WARNING 条件 | diff >= 3 | diff >= 4 |
| vector 保存最近 K 次 diff | K = 4 | K = 3 |
| UNSTABLE 条件 | 当前窗口至少 2 次 WARNING | 相同 |
| 后续 ROS2 发布周期 | 1000ms | 500ms，即 2Hz |

ROS2 发布周期是之后集成速度节点时的要求；本独立程序读取标准输入，不模拟时间。

处理链：

`收到 speed → queue.push → size < 2 则 WAIT/return → front 读取最早速度 → 求 diff → vector 追加并裁剪 → 统计 → 输出 → 更新 previous → queue.pop`

公式：`d = |current - previous|；W = 当前最近 K 个 d 中满足 d >= threshold 的个数；W >= 2 → UNSTABLE`。

原理：queue 规定处理顺序；vector 保存有长度上限的历史变化量。两者保存的数据不同。窗口不足 K 条时，统计已有条目。

**保留原题延迟规则：至少收到两条才处理最早一条；输入结束后，最新一条仍留在队列，不额外处理。**

输出格式：

```text
WAIT
process=<本次处理速度> previous=<上次处理速度> diff=<变化量> warnings=<窗口异常数> state=<NORMAL或WARNING> system=<STABLE或UNSTABLE>
```

以基础题前两条为例：收到 2 输出 WAIT；收到 5 时处理 2，previous=0，diff=2。完整预期见 `cases/base.out` 与 `cases/v2.out`。

模板函数 `absoluteDifference<T>` 用比较和相减实现，调用整数速度时 T=int；本题数值界限保证 int 差值安全。把它推广到任意整数范围时需另行考虑溢出。

## C. 编译与验收

仓库根目录：

```bash
cmake -S . -B build
cmake --build build
python3 scripts/verify_cpp.py --bin build/bin --mode reference
python3 scripts/verify_cpp.py --bin build/bin --mode practice
```

| 模式 | 检查什么 | 意义 |
|---|---|---|
| reference | 参考答案 | 确认题目样例和参考行为 |
| practice | 自己修改的两个 TODO | 判断本次作答是否达标 |
| teaching | 前四阶段 | 确认教学程序默认输出 |
| all | teaching + reference | 起步包基础验收 |

脚本检查：字符串顺序、基础/V2、空输入、单条不处理、重复速度、边界速度、旧异常滑出窗口、非法输入与未知选项。它同时比较完整输出与退出码。

单独看自己的综合题输出：

```bash
./build/bin/monitor_todo < lessons/vector/cases/base.in
./build/bin/monitor_todo --v2 < lessons/vector/cases/v2.in
```

## D. 分次提交

| 提交 | 完成内容 | 达标证据 |
|---|---|---|
| 1 | 字符串遍历 TODO | vector-strings 通过 |
| 2 | 差值模板与 queue 规则 | WAIT 与前几条处理结果正确；未完成时标记 wip |
| 3 | vector 裁剪、统计、状态输出 | practice 全部通过 |
| 4 | 根据题意独立重写 V2 | threshold=4、K=3，两个 V2 用例通过 |
| 5 | 新增 ROS2 速度发布/订阅目标 | String 教学版本保留；新节点使用两端一致的 Int32 |

提交前把命令、结果和一个错误原因写进 `docs/progress.md`。

## E. 下一次迁入 ROS2 的改造题

1. 新增 `speed_publisher.cpp`、`speed_subscriber.cpp`，在 ROS2 CMake 登记并安装两个新目标。
2. 两端都使用 `std_msgs::msg::Int32` 和同一速度话题；发布器循环发送样例序列。
3. 将 `SpeedMonitorLogic` 抽为头文件；订阅者保存一个处理对象，回调中执行 `logic.receive(message.data)`。
4. 基础题 1000ms，V2 500ms。启动订阅者后再启动发布者，按已实际收到的序列核对输出。

数据链：`ROS2 message.data → receive(int) → 已验收的 queue / vector 逻辑`。

原理：通信负责把数据交进来，处理函数负责状态更新；先固定处理规则，集成时更容易定位问题。

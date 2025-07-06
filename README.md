# QLink Game 连连看游戏

A modern implementation of the classi**c Link ****Game** (连连看) with RPG elements, built using Qt.

一个基于Qt开发的具有RPG元素的现代连连看游戏实现。

## Features 特性

### Game Modes 游戏模式
- Single Player Mode 单人模式
- Two Player Mode 双人模式
- Save/Load Game Support 存档/读档支持

### RPG Mechanics RPG机制
- Character Movement 角色移动
- Block Activation 方块激活
- Block Elimination 方块消除
- Score System 计分系统

### Items 道具系统
Single Player Mode 单人模式:
- +1s: Extends remaining time by 30s 延长剩余时间30秒
- Shuffle: Rearranges all blocks 重排所有方块
- Hint: Highlights matching blocks for 10s 高亮可连接方块10秒
- Flash: Click-to-move for 5s 点击移动5秒

Two Player Mode 双人模式:
- All single player items (except Flash) 所有单人模式道具(除Flash外)
- Freeze: Opponent can't move for 3s 使对手静止3秒
- Dizzy: Reverses opponent's controls for 10s 使对手方向颠倒10秒

## Game Rules 游戏规则

1. Players control characters moving on the map 玩家控制角色在地图上移动
2. Activate blocks by moving next to them 移动到方块旁激活方块
3. Connect matching blocks with two or fewer line segments 用两次以内的折线连接相同方块
4. Game ends when: 游戏结束条件：
   - Timer runs out 时间耗尽
   - No more valid matches 无可消除方块

## Development Progress 开发进度

- [x] Step 1: Map Generation and Display 地图生成和显示
- [x] Step 2: Character Movement and Block Interaction 角色移动与方块交互
- [x] Step 3: Block Connection Logic and Unit Tests 方块连接逻辑和单元测试
- [x] Step 4: Timer and Score System 时间和计分系统
- [x] Step 5: Item System 道具系统
- [x] Step 6: Pause and Save/Load 暂停和存档
- [x] Step 7: Two Player Mode 双人模式
- [x] Step 8: Start Menu 开始菜单

## Technical Details 技术细节

### Prerequisites 环境要求
- Qt Framework Qt框架
- C++ Compiler C++编译器

### Project Structure 项目结构
- `blocks.*`: Block management 方块管理
- `customgraphicsview.*`: Custom graphics view 自定义图形视图
- `mainwindow.*`: Main game window 主游戏窗口
- `map.*`: Map management 地图管理
- `player.*`: Player character management 玩家角色管理
- `timer.*`: Game timer management 游戏计时器管理
- `scoreboard.*`: Score management 计分板管理

### Testing 测试
Unit tests are implemented using Qt's testing framework
使用Qt测试框架实现单元测试

## Controls 控制方式

### Single Player Mode 单人模式
- Arrow Keys: Move character 方向键：移动角色
- Space: Pause game 空格键：暂停游戏

### Two Player Mode 双人模式
Player 1 玩家1:
- Arrow Keys: Move character 方向键：移动角色

Player 2 玩家2:
- WASD: Move character WASD键：移动角色

## Building and Running 构建和运行

1. Open QLINK.pro in Qt Creator 在Qt Creator中打开QLINK.pro
2. Configure project 配置项目
3. Build and run 构建并运行


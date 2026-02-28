#Git #GitHub

### 📋 工作区域划分

- **工作区** (Working Directory)
    ↓  git **add**
- **暂存区** (Staging Area/Index)
    ↓  git **commit**
- **本地仓库** (Repository)
    ↓  git **push**
- **远程仓库** (Remote)


### 🚀 初始化仓库

#### 方法1: 本地初始化（推荐新项目）

在项目目录下执行：
```bash
# 进入项目根目录
cd e:\motion_planning\motion_planning\script

git init   # 初始化Git仓库
git add .  # 添加所有文件
git commit -m "Initial commit: Motion Planning v2.1"  # 提交初始版本

# （可选）关联远程仓库  
git remote add origin https://github.com/your-username/your-repo.git
git push -u origin main
```

  ✅ 对已有的程序仓库进行**初始化** 变为一个**Git仓库**
  ✅ **添加**所有的文件到**暂存区**
  ✅ **提交**最初的版本 将**暂存区**中的内容**提交**到**本地仓库**
  💡 关联**远程仓库**
  💡 将**本地仓库**中的内容 **推送**到**远程仓库**

### 方法2: 克隆现有仓库

```bash
git clone https://github.com/your-username/your-repo.git
cd your-repo
```

---

### 🔄 基本工作流程

#### 1. 查看状态
```bash
git status   # 查看当前状态
git diff     # 查看修改内容
```
#### 2. 添加修改
```bash
git add filename.py   # 添加特定文件
git add .   # 添加所有修改
git add *.py  # 添加所有Python文件
```
#### 3. 提交更改

```bash
git commit -m "描述性的提交信息"  # 提交暂存的更改
git commit -m "简短标题" -m "详细描述"  # 提交并添加详细说明
```
#### 4. 推送到远程
```bash
git push   # 推送到远程仓库
git push -u origin main   # 首次推送时
```
#### 5. 拉取更新
```bash
git pull   # 拉取并合并
git fetch   # 仅拉取不合并
```

---
### 🌿 分支管理

#### 创建和切换分支

```bash
git branch feature-name  # 创建新分支
git checkout feature-name  # 切换到分支

git checkout -b feature-name  # 创建并切换（推荐）
git branch -a  # 查看所有分支
```
#### 合并分支

```bash
git checkout main  # 切换到主分支
git merge feature-name  # 合并其他分支
git branch -d feature-name  # 删除已合并的分支
```
#### 推荐的分支策略

- `main` / `master` - 主分支，稳定版本
- `develop` - 开发分支
- `feature/xxx` - 功能分支
- `bugfix/xxx` - 修复分支
- `hotfix/xxx` - 紧急修复

---
###  📝常用命令

#### 日志查看

```bash
git log  # 查看提交历史
git log --oneline  # 简洁模式
git log --graph --oneline --all  # 图形化显示
git log -p filename.py  # 查看某个文件的历史
```
#### 撤销操作

```bash
git checkout -- filename.py  # 撤销工作区修改
git reset HEAD filename.py  # 撤销暂存区的文件
git reset --soft HEAD~1  # 撤销最后一次提交（保留更改）
git reset --hard HEAD~1  # 撤销最后一次提交（丢弃更改）
```
#### 标签管理

```bash
git tag -a v2.1.0 -m "Release v2.1.0"  # 创建标签
git tag  # 查看所有标签
git push origin v2.1.0  # 推送标签
git push --tags  # 推送所有标签
```

---
### 💡 最佳实践

#### 1. 提交信息规范

使用清晰的提交信息：
```bash
# 好的例子
git commit -m "feat: 添加配置验证工具"
git commit -m "fix: 修复CUDA内存泄漏问题"
git commit -m "docs: 更新API文档"
git commit -m "refactor: 重构碰撞检测模块"
git commit -m "perf: 优化批处理性能"

# 不好的例子
git commit -m "update"
git commit -m "修改"
git commit -m "fix bug"
```
##### 提交类型（Conventional Commits）

- `feat`: 新功能
- `fix`: 修复bug
- `docs`: 文档更新
- `style`: 代码格式调整
- `refactor`: 重构代码
- `perf`: 性能优化
- `test`: 测试相关
- `chore`: 构建/工具链更新
#### 2. 忽略文件配置

已配置 `.gitignore` 文件，自动忽略：

- Python缓存文件 (`__pycache__`, `*.pyc`)
- 模型权重 (`weights/*.pth`)
- 训练结果 (`result/`, `logs/`)
- 数据集 (`data/`)
- IDE配置 (`.idea/`, `.vscode/`)
#### 3. 小步提交

```bash
# 推荐：每个逻辑功能单独提交
git add validate_config.py
git commit -m "feat: 添加配置验证工具"
git add run_train.bat
git commit -m "feat: 添加训练一键脚本"

# 不推荐：一次提交太多不相关的修改
git add .
git commit -m "各种更新"
```
#### 4. 定期同步

```bash
git pull  # 每天开始工作前
git push  # 每天结束工作后
```

---

### 🔧 项目特定的Git工作流

#### 场景1: 添加新功能

```bash
# 1. 创建功能分支
git checkout -b feature/new-planner

# 2. 进行开发
# 编辑代码...
  
# 3. 提交更改
git add model/new_planner.py
git commit -m "feat: 实现新的路径规划器"

# 4. 合并到主分支
git checkout main
git merge feature/new-planner

# 5. 删除功能分支
git branch -d feature/new-planner
```
#### 场景2: 修复Bug

```bash
# 1. 创建修复分支
git checkout -b bugfix/collision-detection

# 2. 修复bug
# 修改代码...

# 3. 提交
git add utils/collision.py
git commit -m "fix: 修复自碰撞检测错误"

# 4. 合并并推送
git checkout main
git merge bugfix/collision-detection
git push
```
#### 场景3: 更新文档

```bash
# 直接在主分支更新文档
git checkout main

# 修改文档
# 编辑 docs/...

# 提交
git add docs/
git commit -m "docs: 更新API文档示例"
git push
```

---
### ❓ 问题排查

#### Q1: 不小心提交了大文件

```bash
# 从Git历史中删除文件
git filter-branch --force --index-filter \
  "git rm --cached --ignore-unmatch path/to/large/file" \
  --prune-empty --tag-name-filter cat -- --all

# 强制推送
git push origin --force --all
```
#### Q2: 合并冲突

```bash
# 拉取时发生冲突
git pull
# 会提示冲突文件
# 手动编辑冲突文件，解决标记：
# <<<<<<< HEAD
# 你的更改
# =======
# 其他人的更改
# >>>>>>>

# 解决后提交
git add .
git commit -m "merge: 解决合并冲突"
```
#### Q3: 查看.gitignore是否生效

```bash
# 查看被忽略的文件
git status --ignored

# 测试某个文件是否被忽略
git check-ignore -v filename
```

#### Q4: 恢复已删除的文件

```bash
# 查看删除记录
git log -- deleted_file.py

# 恢复到最近的提交
git checkout HEAD -- deleted_file.py
```

---
### 🔐 敏感信息处理
  
**绝不提交**以下内容：
- 密码和API密钥
- 数据库连接字符串
- 私钥文件
- 大型数据集

**使用环境变量**

```python

# 不要这样

API_KEY = "sk-1234567890abcdef"

# 应该这样
import os
API_KEY = os.getenv('API_KEY')

```

创建 `.env` 文件（已在.gitignore中）：

```bash
API_KEY=sk-1234567890abcdef
DATABASE_URL=postgresql://...
```

---
### 🚀 快速参考

```bash
# 日常工作流
git pull                          # 1. 拉取最新代码

# ... 编辑代码 ...
git add .                         # 2. 添加修改
git commit -m "描述"              # 3. 提交
git push                          # 4. 推送

# 紧急修复流程
git stash                         # 暂存当前工作
git checkout main                 # 切换到主分支
git checkout -b hotfix/xxx        # 创建修复分支

# ... 修复 ...
git commit -m "fix: xxx"
git checkout main
git merge hotfix/xxx
git push
git stash pop                     # 恢复之前的工作
```

  
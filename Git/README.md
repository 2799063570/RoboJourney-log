# Git 入口

这里存放 Git 使用、仓库同步、版本管理相关笔记。

## 主要笔记

- [[Git使用指南]]
- [[method]]

## 当前 Vault 的 Git 原则

- 只使用 Vault 根目录的 Git 仓库。
- 子文件夹不要保留自己的 `.git`。
- 构建产物、IDE 缓存、Obsidian 本地状态不要进入 Git。
- 重要整理动作尽量单独提交，方便回滚和查看历史。

## 常用检查

```bash
git status
git log --oneline -5
git diff --cached --stat
```


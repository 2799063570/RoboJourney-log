---
type: script
status: 🟢 测试通过
module: debug
dependencies:
  - matplotlib
description: 读取数据集中路径CSV文件，绘制轨迹变化中各个关节的变化情况
---
绘制csv文件中轨迹下 各个关节的变化图像
    配置：
        FILE_PATH： 轨迹路径
        TARGET_SCENE_ID： 场景ID
        TARGET_PATH_ID： 轨迹ID，-1则保存该场景下的所有轨迹
        SAVE_FIGURES： 是否保存路径
        SAVE_ROOT_DIR： 图像保存的根目录
        MAX_SHOW_LIMIT： 程序最多显示的窗口个数
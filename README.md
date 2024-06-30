# Structure-Visualizer-Imgui

基于C++ imgui框架实现数据结构可视化

美好愿景是实现各种数据结构和算法的可视化，目前仅完成avl平衡树的最基本操作。

# 运行环境

linux环境下编码，后端渲染使用glfw+opengl2；

可跨平台，安装对应依赖，选择对应后端渲染代码即可。

imgui文件未给出，详细查看[imgui官网](https://github.com/ocornut/imgui)

# 功能

已实现：

- dfs遍历avl树渲染

- avl树的节点插入操作

- avl树的节点删除操作

- avl树的节点查询操作

- 计数统计

待实现：

- 前驱、后继、排名查询

- 更简易的数据测试方法

待优化：

- 节点水平距离计算


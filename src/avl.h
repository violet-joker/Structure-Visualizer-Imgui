#include <iostream>
#include <algorithm>
#include <math.h>
#include <thread>
#include <memory>
#include "../build/imgui/imgui.h"

namespace avl {
    // 定义节点
    struct AvlNode {
    public:
        AvlNode(int key_);
    public:
        int key;
        int h;
        int cnt;
        int size;
        std::shared_ptr<AvlNode> l;
        std::shared_ptr<AvlNode> r;
        bool select = false;
    };

    typedef std::shared_ptr<AvlNode> NodePtr;
    static const int d = 30, r = 25;
    static NodePtr root = nullptr;

    // 延时后恢复状态
    void delay_restorea(int second, NodePtr p);
    // 申请节点
    NodePtr get_node(int key);
    // 更新节点高度
    void pushup(NodePtr &p);
    // 左旋
    void rotate_left(NodePtr &p);
    // 右旋
    void rotate_right(NodePtr &p);
    // 计算平衡因子Balance Factor
    int get_balance_factor(std::shared_ptr<AvlNode> & p1, std::shared_ptr<AvlNode> & p2);
    // 检查平衡性并调整
    void check(NodePtr &p);
    // 添加节点
    void add(int key, NodePtr &p);
    // 删除节点
    void remove(int key, NodePtr &p);
    // 查询节点，将查询到的进行标记
    void search(int key, NodePtr &p, int sec = 0);
    // 绘点，仅画父节点，由父节点向下延伸
    void draw_node(ImVec2 pos, ImVec2 p1, ImVec2 p2, std::string msg, bool select);
    // dfs遍历绘画节点，由于渲染不分次序，dfs怎样遍历都行，这里采用中序遍历 
    void dfs(NodePtr &p, ImVec2 pos);
    // 根据输入数据初始化平衡树
    void init(int n, NodePtr &root);
}
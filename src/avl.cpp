#include "avl.h"

namespace avl {
    AvlNode::AvlNode(int key_) {
        key = key_;
        h = 0;
        cnt = 1;
        l = r = nullptr;
    }

    // 延时后恢复状态
    void delay_restorea(int second, NodePtr p) {
        std::this_thread::sleep_for(std::chrono::seconds(second));
        p->select = true;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        p->select = false;
    }

    // 申请节点
    NodePtr get_node(int key) {
        return NodePtr {new AvlNode(key)};
    }

    // 更新节点
    void pushup(NodePtr &p) {
        if (!p) return;
        // 匿名函数获取节点信息，(处理节点为空的情况，便于运算 )
        auto get_h([](NodePtr p) {
            return p ? p->h : -1;
        });
        auto get_size([](NodePtr p) {
            return p ? p->size : 0;
        });

        p->h = std::max(get_h(p->l), get_h(p->r)) + 1;
        p->size = p->cnt + get_size(p->l) + get_size(p->r);
    }

    // 左旋
    void rotate_left(NodePtr &p) {
        NodePtr q = p->r;
        p->r = q->l;
        q->l = p;
        p = q;
        pushup(p->l);
        pushup(p);
    }

    // 右旋
    void rotate_right(NodePtr &p) {
        NodePtr q = p->l;
        p->l = q->r;
        q->r = p;
        p = q;
        pushup(p->r);
        pushup(p);
    }

    // 计算平衡因子Balance Factor
    int get_balance_factor(std::shared_ptr<AvlNode> & p1, std::shared_ptr<AvlNode> & p2) {
        return (p1 ? p1->h : -1) - (p2 ? p2->h : -1);
    }
    // 检查平衡性并调整
    void check(NodePtr &p) {
        if (!p) return;
        if (get_balance_factor(p->l, p->r) == 2) {
            NodePtr &q = p->l;
            if (get_balance_factor(q->l, q->r) == -1)
                rotate_left(q), rotate_right(p);
            else
                rotate_right(p);
        }
        if (get_balance_factor(p->l, p->r) == -2) {
            NodePtr &q = p->r;
            if (get_balance_factor(q->l, q->r) == 1)
                rotate_right(q), rotate_left(p);
            else
                rotate_left(p);
        }
    }

    // 添加节点
    void add(int key, NodePtr &p) {
        if (!p)
            p = get_node(key);
        else if (key == p->key)
            p->cnt++;
        else if (key < p->key)
            add(key, p->l);
        else
            add(key, p->r);
        pushup(p);
        check(p);
    }

    // 删除节点
    void remove(int key, NodePtr &p) {
        if (!p)
            return;
        else if (key < p->key)
            remove(key, p->l);
        else if (key > p->key)
            remove(key, p->r);
        else if (p->cnt > 1)
            p->cnt--;
        else {
            if (!p->l && !p->r)
                p = nullptr;
            else if (!p->l)
                p = p->r;
            else if (!p->r)
                p = p->l;
            else {
                // 由于原先将相同值的节点插入到了右子树，所以删除策略选择寻找后继
                NodePtr q = p->r;
                while (q->l) q = q->l;
                std::swap(q->key, p->key);
                remove(key, p->r);
            }
        }
        pushup(p);
        check(p);
    }

    // 查询节点，将查询到的进行标记
    void search(int key, NodePtr &p, int sec) {
        if (!p)
            return;
        std::thread t {delay_restorea, sec, p};
        t.detach();
        if (key < p->key)
            search(key, p->l, sec+1);
        else if (key > p->key)
            search(key, p->r, sec+1);
    }

    // 绘点，仅画父节点，由父节点向下延伸
    void draw_node(ImVec2 pos, ImVec2 p1, ImVec2 p2, std::string msg, bool select) {
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImColor color1 {255, 0, 0, 255}; 
        draw_list->AddLine(pos, p1, color1);
        draw_list->AddLine(pos, p2, color1);
        // 坐标，圆的半径，颜色，线宽
        ImColor color2 = select ? ImColor(0, 255, 0, 255) : color1;
        draw_list->AddCircle(pos, r, color2, 0, 1.0f);
        draw_list->AddText(pos, IM_COL32_WHITE, msg.c_str());
    }

    // dfs遍历绘画节点，由于渲染不分次序，dfs怎样遍历都行，这里采用中序遍历 
    void dfs(NodePtr &p, ImVec2 pos) {
        if (!p) return;
        
        ImVec2 p1 {pos};
        ImVec2 p2 {pos};

        if (p->l) p1 = {pos.x - (float)pow(2, p->l->h) * d, pos.y + d + d};
        if (p->r) p2 = {pos.x + (float)pow(2, p->r->h) * d, pos.y + d + d};

        dfs(p->l, p1);
        std::string msg = std::to_string(p->key) + "|" + std::to_string(p->cnt);
        // std::string msg = std::to_string(p->key);
        draw_node(pos, p1, p2, msg, p->select);
        dfs(p->r, p2);
    }

    // 根据输入数据初始化平衡树
    void init(int n, NodePtr &root) {
        n = std::max(n, 0);
        n = std::min(n, 30);
        int f[30];
        for (int i = 0; i < n; i++) f[i] = i;
        std::random_shuffle(f, f+n);
        for (int i = 0; i < n; i++) add(f[i], root);
    }
}
#include <iostream>
#include <algorithm>
#include <math.h>
#include <thread>
#include <memory>
#include "../build/imgui/imgui.h"

namespace avl {
    struct Node {
        Node(int k) : key(k) { l = r = nullptr; }
        int key, h;
        Node *l, *r;
        bool select = false;
        // 计算平衡因子BalanceFactor
        static int getBF(const Node* p1, const Node* p2) {
            return (p1 ? p1->h : -1) - (p2 ? p2->h : -1);
        }
    };

    static const int d = 25, r = 20;
    Node *root = nullptr, *tmp = nullptr;

    // 申请节点
    Node* get_node(int key) {
        return new Node(key);
    }

    // 更新节点高度
    void pushup(Node* &p) {
        if (!p) return;
        if (!p->l && !p->r)
            p->h = 0;
        else if (!p->l)
            p->h = p->r->h + 1;
        else if (!p->r)
            p->h = p->l->h + 1;
        else
            p->h = std::max(p->l->h, p->r->h) + 1;
    }

    // 左旋
    void zig(Node* &p) {
        Node* q = p->r;
        p->r = q->l;
        q->l = p;
        p = q;
        pushup(p->l);
        pushup(p);
    }

    // 右旋
    void zag(Node* &p) {
        Node* q = p->l;
        p->l = q->r;
        q->r = p;
        p = q;
        pushup(p->r);
        pushup(p);
    }

    // 检查平衡性并调整
    void check(Node* &p) {
        if (!p) return;
        if (Node::getBF(p->l, p->r) == 2) {
            Node* &q = p->l;
            if (Node::getBF(q->l, q->r) == 1)
                zag(p);
            else
                zig(q), zag(p);
        }
        if (Node::getBF(p->l, p->r) == -2) {
            Node* &q = p->r;
            if (Node::getBF(q->l, q->r) == -1)
                zig(p);
            else
                zag(q), zig(p);
        }
    }

    // 添加节点
    void add(int key, Node* &p) {
        if (!p)
            p = get_node(key);
        else if (key < p->key)
            add(key, p->l);
        else
            add(key, p->r);
        pushup(p);
        check(p);
    }

    // 删除节点
    void remove(int key, Node* &p) {
        if (!p)
            return;
        else if (key < p->key)
            remove(key, p->l);
        else if (key > p->key)
            remove(key, p->r);
        else {
            if (!p->l && !p->r)
                delete p, p = nullptr;
            else if (!p->l)
                tmp = p, p = p->r, delete tmp;
            else if (!p->r)
                tmp = p, p = p->l, delete tmp;
            else {
                // 由于原先将相同值的节点插入到了右子树，所以删除策略选择寻找后继
                Node* q = p->r;
                while (q->l) q = q->l;
                std::swap(q->key, p->key);
                remove(key, p->r);
            }
        }
        pushup(p);
        check(p);
    }

    // 查询节点，将查询到的进行标记
    void search(int key, Node* &p) {
        if (!p)
            return;
        p->select = true;
        if (key < p->key)
            search(key, p->l);
        else if (key > p->key)
            search(key, p->r);
    }

    // 绘点，仅画父节点，由父节点向下延伸
    void draw(ImVec2 pos, ImVec2 p1, ImVec2 p2, std::string msg, bool select) {
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
    void dfs(Node* &p, ImVec2 pos) {
        if (!p) return;
        
        ImVec2 p1 {pos};
        ImVec2 p2 {pos};

        if (p->l) p1 = {pos.x - (float)pow(2, p->l->h) * d, pos.y + d + d};
        if (p->r) p2 = {pos.x + (float)pow(2, p->r->h) * d, pos.y + d + d};

        dfs(p->l, p1);
        draw(pos, p1, p2, std::to_string(p->key), p->select);
        dfs(p->r, p2);
    }

    // 根据输入数据初始化平衡树
    void init(int n, Node* &root) {
        n = std::max(n, 0);
        n = std::min(n, 30);
        int f[30];
        for (int i = 0; i < n; i++) f[i] = i;
        std::random_shuffle(f, f+n);
        for (int i = 0; i < n; i++) add(f[i], root);
    }
}
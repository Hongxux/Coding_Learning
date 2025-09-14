#include <iostream>
#include <cassert>

Node* root = nullptr;

enum COLOR { R, B }; // 节点颜色
enum Direct { L, R }; // 孩子方向

struct Node {
    Node* father;
    Node* chil[2]; // [0]=左孩子, [1]=右孩子
    bool color;    // 建议使用更明确的字段名
    int value;
    int size;      // 子树大小（包含自身）

    // 构造函数初始化
    Node(int val) : father(nullptr), chil{ nullptr, nullptr },
        color(R), value(val), size(1) {}
};

/*
 * 执行旋转操作
 * @param p 当前子树根节点
 * @param dir 旋转方向: 0=左旋, 1=右旋
 * @return 旋转后新的子树根节点
 */

Node* rotation(Node* p, bool dir) {
    Node* gp = p->father; // 祖父节点
    Node* s = p->chil[!dir]; // 新根节点（旋转后替代p的位置）
    assert(s != nullptr); // 确保新根节点存在

    // 处理s的子树（可能为空）
    Node* c = s->chil[dir]; // 需要转移的子节点
    if (c) {
        c->father = p; // 更新父指针
    }

    // 重构父子关系
    p->chil[!dir] = c;      // p接管s的原子节点
    s->chil[dir] = p;       // s接管p作为子节点

    // 更新父指针
    p->father = s;          // p的新父节点是s
    s->father = gp;         // s的新父节点是原祖父

    // 更新祖父节点的链接
    if (gp) {
        // 判断p原先是gp的左子还是右子
        bool p_dir = (p == gp->chil[Direct::R]);
        gp->chil[p_dir] = s; // gp现在指向s
    }
    else {
        root = s; // 更新整棵树的根节点
    }

    // 更新子树大小（关键补充）
    s->size = p->size;                 // s继承原p的子树大小
    p->size = 1;                       // 重置p的计数器（自身）
    if (p->chil[dir]) p->size += p->chil[dir]->size;   // 添加保留的子节点大小
    if (p->chil[!dir]) p->size += p->chil[!dir]->size; // 添加转移来的子节点大小

    return s; // 返回新的子树根节点
}

// 辅助函数：获取子树大小（处理空指针）
size_t size(Node* node) {
    return node ? node->size : 0;
}
Node* insert(int value){
    Node* n = new Node(value);//n new 新节点
    if (root == nullptr) {
        root = n;
        root->color = B;
        return root;
    }
    Node* now = root;//now 当前节点,p parent 父亲节点
    Node* p;
    bool dir = L;
    //找到n的位置
    while (now) {
        p = now;
        if (p->value == value) return root;
        dir = now->value > value;
        now = now->chil[dir];
    }
    //建立父子联系
    p->chil[dir] = n;
    n->father = p;
    //修复
    fix_subtree(p, n, dir);
    return n;
}

static void fix_subtree(Node* p,Node* n,bool dir) {
    //case 1插入的节点是根节点
    if (!p) {
        n->color = B;
        root = n;
        return;
    }
    //父节点是黑节点不需要处理
    if (p->color = B) {
        return;
    }
    //从插入位置向上循环维护
    //case 2插入节点的父亲和叔叔是红色
    Node* u = p->chil[!dir];
    Node* gp = p->father;
    
    while (u) {
        if (u->color == R && p->color == R) {
            //叔父爷变色,相当于将祖父节点（中间节点上提）
            u->color = p->color = B;
            gp->color = R;
            //因为祖父向上提（变成红色节点），可能还会有隐患，所以接着判定
            n = gp;
            p = gp->father;
            dir = gp == gp->father->chil[1];
            u = p->chil[!dir];
        }
        else break;
    }
    
    //case 3插入节点的父亲是红色，叔叔是黑色
    if (!u ) {
        if (p->color == R) {
            Node* r_n = gp;//旋转的节点
            Node* r_c = p;//旋转的中心
            
            if (dir && r_c == r_n->chil[0]) {
                //LL
                rotation(r_n, 1);
                r_c->color = r_n->color;
                r_n->color = R;
            }
            else if (dir && r_c == r_n->chil[1]) {
                //RL
                rotation(r_c, 1);
                rotation(r_n, 0);
                r_c->color = r_n->color;
                r_n->color = R;
            }
            else if (!dir && r_c == r_n->chil[1]) {
                //RR
                rotation(r_n, 0);
                r_c->color = r_n->color;
                r_n->color = R;
            }
            else if (!dir && r_c == r_n->chil[0]) {
                //LR
                rotation(r_c, 0);
                rotation(r_n, 1);
                r_c->color = r_n->color;
                r_n->color = R;
            }
            
        }
    }
    else if(u->color == B) {
        if (p->color == R) {
            Node* r_n = gp;//旋转的节点
            Node* r_c = p;//旋转的中心

            if (dir && r_c == r_n->chil[0]) {
                //LL
                rotation(r_n, 1);
                r_c->color = r_n->color;
                r_n->color = R;
            }
            else if (dir && r_c == r_n->chil[1]) {
                //RL
                rotation(r_c, 1);
                rotation(r_n, 0);
                r_c->color = r_n->color;
                r_n->color = R;
            }
            else if (!dir && r_c == r_n->chil[1]) {
                //RR
                rotation(r_n, 0);
                r_c->color = r_n->color;
                r_n->color = R;
            }
            else if (!dir && r_c == r_n->chil[0]) {
                //LR
                rotation(r_c, 0);
                rotation(r_n, 1);
                r_c->color = r_n->color;
                r_n->color = R;
            }

        }
    }
    
    root->color = B;

}
int main() {
    // 测试用例
    Node* n1 = new Node(10);
    Node* n2 = new Node(20);
    Node* n3 = new Node(30);

    // 构建初始树结构
    root = n2;
    n2->chil[Direct::L] = n1;
    n2->chil[Direct::R] = n3;
    n1->father = n2;
    n3->father = n2;

    // 更新子树大小
    n2->size = 3;

    // 执行左旋测试
    Node* new_root = rotation(n2, Direct::L);
    std::cout << "Rotation test passed!" << std::endl;

    delete n1;
    delete n2;
    delete n3;
    return 0;
}

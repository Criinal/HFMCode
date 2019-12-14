
#include <iostream>
#include "HFMTree.h"

//初始化哈夫曼树
void HFMTree::init(std::vector<double> v, std::string s) {
    root = nullptr;
//优先队列，堆结构，用于存放节点并每次取最小
//插入和查询的复杂度都是o(logn)
    std::priority_queue<HFMNode> priorityQueue;
    while (!priorityQueue.empty())priorityQueue.pop();

//插入初始节点
//cnt用于统计节点的个数，包括由两个节点合成的新节点
    int cnt = 0;
    for (int i = 0; i < v.size(); ++i) {
        HFMNode hfmNode;
//        调用哈夫曼树节点的初始化
        hfmNode.init(cnt);
        cnt++;
//        对节点赋值，将value赋值成频度，ch赋值成它代表的字符
        hfmNode.value = v[i];
        hfmNode.ch = s[i];
//        将点加进优先队列里。
        priorityQueue.push(hfmNode);
    }

    while (!priorityQueue.empty()) {
//取出最小的点
        HFMNode *hfmNode1 = new HFMNode;

//        优先队列的top()能获取整个优先队列的最小值，pop()则将这个最小值弹出
        *hfmNode1 = priorityQueue.top();
        priorityQueue.pop();

//        优先队列为空表示只剩下一个点，即为root，此时应将root赋值为当前仅剩的哈夫曼树节点
        if (priorityQueue.empty()) {
            root = hfmNode1;
            return;
        }

//        取出次小的点
        HFMNode *hfmNode2 = new HFMNode;
        *hfmNode2 = priorityQueue.top();
        priorityQueue.pop();

//        创建新的节点并加入优先队列
        HFMNode hfmNode;
        hfmNode.init(cnt);
//        由两个节点合成的新节点，是这两个节点的父节点，其频度是它的两个子节点之和
        hfmNode.value = hfmNode1->value + hfmNode2->value;
        hfmNode.left = hfmNode1;
        hfmNode.right = hfmNode2;
//        将合成的新节点再一次加进优先队列里。
        priorityQueue.push(hfmNode);
        cnt++;
    }
}

//从文件创建哈夫曼树
void HFMTree::build_from_file(FILE *fp) {
//map由是节点标号映射到该节点指针的储存结构
    std::map<int, HFMNode *> map;
    map.clear();
    map[-1] = nullptr;

//由于采用后序遍历储存入文件中，在创建根节点时它的子节点若存在，则一定已经被创建
//文件的第一行是节点个数
    int tot;
    fscanf(fp, "%d", &tot);
    for (int i = 0; i < tot; ++i) {
        HFMNode *hfmNode = new HFMNode;
        int left, right;
//        输入节点的信息
        fscanf(fp, "%lf,%c,%d,%d,%d,%d", &hfmNode->value,
               &hfmNode->ch,
               &hfmNode->index,
               &hfmNode->depth,
               &left,
               &right
        );

//        map储存的是某个点的序号对应的该节点的指针
//        比如序号为1的节点指针是map[1],序号为2的节点指针是map[2]
        hfmNode->left = map[left];
        hfmNode->right = map[right];
        map[hfmNode->index] = hfmNode;
//        最后一个节点一定是根节点，因此将它赋值给根节点指针root
        if (i == tot - 1)root = hfmNode;
    }
}

//将哈夫曼树写入文件
void HFMTree::write_to_file(FILE *fp) {
    fprintf(fp, "%d\n", root->index + 1);
    root->dfs_write(fp);
}

//编码
void HFMTree::encoding() {
    root->dfs_encoding("", encode);
}

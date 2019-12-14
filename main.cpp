#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <fstream>
#include "HFMTree.h"

HFMTree *hfmTree;

//hfmTree表示第一个样例的哈夫曼树
//hfmTree2表示第二个样例的哈夫曼树
const char *HFMFILE = "hfmTree2";

//字符
std::string code;
//词频
std::vector<double> frequency;

//显示菜单
void menu();

//初始化
int Initialization();

//编码
int Encoding();

//内存中不存在哈夫曼树，从文件中创建
int build_from_file();

//译码
int Decoding();

//打印代码，以50字符为一行
int print();

//以上函数的返回值都为是否出错。1代表出错，0代表未出错

//按广度优先遍历的顺序打印
void print_tree();

int main() {
    menu();
    return 0;
}

//菜单
void menu() {
//    初始化哈夫曼树
    delete hfmTree;
    hfmTree = nullptr;

    while (true) {
        printf("\n");
        printf("I: Initialization\n");
        printf("E: Encoding\n");
        printf("D: Decoding\n");
        printf("P: Print\n");
        printf("T: Tree printing\n");
        printf("Q: Quit\n");

//        用一个error来判断程序是否出错
        int error = 0;
        char oper;
//        输入操作类型
        std::cin >> oper;
        switch (oper) {
//            Initialization
            case 'I':
            case 'i':
                error = Initialization();
                if (error)continue;
                break;
//            Encoding
            case 'E':
            case 'e':
                if (build_from_file()) {
                    error = 1;
                    break;
                }
                Encoding();
                break;
//            Decoding
            case 'D':
            case 'd':
                if (build_from_file()) {
                    error = 1;
                    break;
                }
                Decoding();
                break;
//            print
            case 'P':
            case 'p':
                print();
                break;
//            print tree
            case 'T':
            case 't':
                if (build_from_file()) {
                    error = 1;
                    break;
                }
                print_tree();
                break;

//            quit
            case 'Q':
            case 'q':
                return;

            default:
                printf("Please try again.\n");
        }
        if (error) {
            std::cout << "ERROR.\n";
            break;
        }
    }
}

//初始化
int Initialization() {
//    清空哈夫曼树、字符集、频度表
    delete hfmTree;
    hfmTree = new HFMTree;
    frequency.clear();
    code = "";

//    输入字符集大小
    printf("Please input the size of the Charset.\n");
    int n;
    if (scanf("%d", &n) != 1) {
        return 1;
    }

    printf("Please input every character and its frequency.\n");
    for (int i = 0; i < n; ++i) {
//        读入字符及其频度
        std::string ch;
        double d;
        std::cout << "character: (use \"space\" instead of ' ')\n";
        std::cin >> ch;
//    由于cin会自动冲掉空格，用space来代替空格输入
        if (ch == "space")ch = " ";
        else if (ch.size() > 1)
            return 1;
        std::cout << "frequency: \n";
        std::cin >> d;
        code += ch;
        frequency.push_back(d);
    }
//    从读入的字符集和频度表构建哈夫曼树
    hfmTree->init(frequency, code);

//写入文件
    hfmTree->root->depth = 1;
    FILE *fp = fopen(HFMFILE, "w");
    hfmTree->write_to_file(fp);
    fclose(fp);

    return 0;
}

//根据广度优先遍历的方式打印树的节点
void print_tree() {

    FILE *fp = fopen("TreePrint", "w");

    if (build_from_file())
        return;
//用队列先进先出的特性实现广度优先搜索
    std::queue<HFMNode> queue;

//清空队列
    while (!queue.empty())queue.pop();

//depth表示深度，当深度变大时，需要换行
    int depth = 1;
    queue.push(*hfmTree->root);
    while (!queue.empty()) {
        HFMNode front = queue.front();
        queue.pop();

//由第n层搜索到第n+1层时，需要换行
        if (front.depth > depth) {
            printf("\n");
            fprintf(fp, "\n");
        }
        depth = front.depth;

//如果某个节点的左右子节点为空，说明它是叶子节点，则它代表一个字符，它的value即为它的频度
//否则它是一个节点，它的value代表它的左右子节点的value之和
        if (front.left == nullptr || front.right == nullptr) {
            printf("%f(%c) ", front.value, front.ch);
            fprintf(fp, "%f(%c) ", front.value, front.ch);
            continue;
        } else {
            printf("%f ", front.value);
            fprintf(fp, "%f ", front.value);
        }
//将它的左右子节点进队列
        queue.push(*front.left);
        queue.push(*front.right);
    }
    printf("\n");
    fprintf(fp, "\n");
    fclose(fp);
}

int Encoding() {
    if (build_from_file())
        return 1;

//对这棵哈夫曼树进行编码，得出每个字符对应的二进制串
    hfmTree->encoding();

//从文件读取并编码
    FILE *fp_tobetran = fopen("ToBeTran", "r");
    FILE *fp_codefile = fopen("CodeFile", "w");

    char cha;
//如果是字符集内的字符，则输出编码，否则直接输出原字符
    while ((cha = getc(fp_tobetran)) != EOF) {
        if (hfmTree->encode.find(cha) != hfmTree->encode.end()) {
            fprintf(fp_codefile, "%s", hfmTree->encode[cha].c_str());
        } else {
            fprintf(fp_codefile, "%c", cha);
        }

    }

//关闭文件
    fclose(fp_tobetran);
    fclose(fp_codefile);

    return 0;
}

//从文件中读取并构建哈夫曼树
int build_from_file() {
//已存在哈夫曼树，不需要读取
    if (hfmTree != nullptr)return 0;
//不存在，从文件中读取
    if (hfmTree == nullptr) {
        hfmTree = new HFMTree;
        FILE *fp = fopen(HFMFILE, "r");
        hfmTree->build_from_file(fp);
        fclose(fp);
        if (hfmTree == nullptr || hfmTree->root == nullptr) {
            return 1;
        }
    }
    return 0;
}

//译码
int Decoding() {

    if (hfmTree == nullptr)
        return 1;

//CodeFile是由原文件ToBeTran翻译得到的二进制文件
//Textfile是由Codefile译码得到的原文件
    FILE *fp_codefile = fopen("CodeFile", "r");
    FILE *fp_textfile = fopen("Textfile", "w");

//从文件中读取01串，遇到非01字符就直接输出，get_c逐字读入
    char ch;
    HFMNode *hfmNode = hfmTree->root;
    while ((ch = getc(fp_codefile)) != EOF) {
        if (ch == '0' || ch == '1') {
//0则表示从左子树往下走  1则表示右子树
            if (ch == '0') {
                if (hfmNode->left == nullptr)return 1;
                hfmNode = hfmNode->left;
//判断是否走到底了  是的话则重置
                if (hfmNode->left == nullptr) {
                    fprintf(fp_textfile, "%c", hfmNode->ch);
                    hfmNode = hfmTree->root;
                }
            } else {
                if (hfmNode->right == nullptr)return 1;
                hfmNode = hfmNode->right;
//判断是否走到底了  是的话则重置
                if (hfmNode->right == nullptr) {
                    fprintf(fp_textfile, "%c", hfmNode->ch);
                    hfmNode = hfmTree->root;
                }
            }
//非01字符原样输出
        } else {
            fprintf(fp_textfile, "%c", ch);
        }
    }

//关闭文件
    fclose(fp_codefile);
    fclose(fp_textfile);

    return 0;
}

//打印CodeFile，并且按50字符一行的形式呈现
int print() {
    int cnt = 0;
    FILE *fp_codefile = fopen("CodeFile", "r");
    FILE *fp_codeprin = fopen("CodePrin", "w");

    char ch;
//    从CodeFile中逐字读入，直到读到文件结尾EOF为止
    while ((ch = getc(fp_codefile)) != EOF) {
        if (ch == '\n')continue;
        printf("%c", ch);
        fprintf(fp_codeprin, "%c", ch);
        cnt++;
//        cnt为50时说明需要换行了，此时应该重新打印
        if (cnt == 50) {
            printf("\n");
            fprintf(fp_codeprin, "\n");
            cnt = 0;
        }
    }
    printf("\n");
//关闭文件
    fclose(fp_codefile);
    fclose(fp_codeprin);
    return 0;
}


//happy every day

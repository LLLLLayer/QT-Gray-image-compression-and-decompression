#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <fstream>
#include <QString>
#include <queue>
#include <io.h>
#include <stack>
#include <map>
#include <cstdlib>
#include <QFileDialog>
#include <cstdlib>
#include <QDebug>
#include <QMessageBox>

struct head {
    unsigned char b;					//记录字符在数组中的位置
    long count;							//字符出现频率（权值）
    long parent,lch,rch;				//定义哈夫曼树指针变量
    char bits[256];						//定义存储哈夫曼编码的数组
};


class Huffman
{
public:
    Huffman(QString in);
    void HuffmanCompress();
    void HuffmandeCompress(QString&out);

    QString input;
    head header[512];
    head tmp;
};

#endif // HUFFMAN_H

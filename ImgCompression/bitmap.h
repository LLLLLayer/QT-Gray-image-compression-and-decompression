#ifndef BITMAP_H
#define BITMAP_H

#include <QFile>
#include <string>
#include <QByteArray>
#include <cmath>
#include <QFileDialog>
#include <QMainWindow>
#include<QDebug>
#include<QGraphicsObject>
//#include<vector>
using namespace std;

typedef unsigned __int64 unint64;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef long LONG;
//BITMAPFILEHEADER结构的长度是固定的14个字节，描述文件的有关信息。
typedef struct tagBITMAPFILEHEADER{
    WORD bfType;//must be 0x4D42.
    DWORD bfSize;//the size of the whole bitmap file.
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;//the sum bits of BITMAPFILEHEADER,BITMAPINFOHEADER and RGBQUAD;the index byte of the image data.
}BITMAPFILEHEADER;//位图头文件定义
//BITMAPINFOHEADER结构的长度是固定的40个字节，描述图像的有关信息。
typedef struct tagBITMAPINFOHEADER{
    DWORD biSize;//信息头大小
    LONG biWidth;//图像宽度
    LONG biHeight;//图像高度
    WORD biPlanes;//位平面数，必须为1
    WORD biBitCount;//每像素位数
    DWORD  biCompression; //压缩类型
    DWORD  biSizeImage; //压缩图像大小字节数
    LONG  biXPelsPerMeter; //水平分辨率
    LONG  biYPelsPerMeter; //垂直分辨率
    DWORD  biClrUsed; //位图实际用到的色彩数
    DWORD  biClrImportant; //本位图中重要的色彩数
}BITMAPINFOHEADER; //位图信息头定义
//调色板是一个数组，数组中每个元素就是一个rgb颜色，对于8位图像，最多可表达256种颜色，调色板的大小就是256。
typedef struct tagRGBQUAD{
    BYTE rgbBlue; //该颜色的蓝色分量
    BYTE rgbGreen; //该颜色的绿色分量
    BYTE rgbRed; //该颜色的红色分量
    BYTE rgbReserved; //保留值
}RGBQUAD;//调色板定义

class Bitmap:public QObject
{
    Q_OBJECT
public:
    Bitmap(QString name);//将文件名传入类的构造函数
    void readBitmap();//读取位图，获取图象文件头、图象信息头、调色板数据、图像数据
    void writeBitmap(QString fileName);//写入位图，写入图象文件头、图象信息头、调色板数据、图像数据
    void Compress(QString fileName);//压缩位图
    void UnCompress();//解压位图

    QString BYTEtoBit(BYTE data,int bit);//将unsigned char转为二进制数位保存在QString中
    BYTE StrtoBYTE(QString str,int bit);//将QString中的二进制数位转为unsigned char
    int StrtoInt(QString str,int bit);//将QString中的二进制数位转为int

signals:
    void Signal(int value);

private:
    QString fileName;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBQUAD rgb[256];
    int width;
    int height;
    BYTE **pixelData = NULL;
    BYTE *pixel = NULL;
    QFile outFile;
    QString LengthBits="";
    QString AllPixels="";
    QString All="";


};

#endif // BITMAP_H

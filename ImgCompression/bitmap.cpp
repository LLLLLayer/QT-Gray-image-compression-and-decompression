#include "bitmap.h"

int length(int i){//获取像素的位数
    int k = 1;
    i = i / 2;
    while (i>0){
        k++;
        i = i / 2;
    }
    return k;
}

Bitmap::Bitmap(QString name){
    this->fileName=name;
}

void Bitmap::readBitmap(){//读取位图信息
    emit Signal(1);
    QByteArray ba = fileName.toLocal8Bit();
    char* file = ba.data();
    FILE *fp;
    fp = fopen(file,"rb");
    //获取位图头文件定义
    fread(&fileHeader.bfType,1,sizeof(fileHeader.bfType),fp);
    fread(&fileHeader.bfSize,1,sizeof(fileHeader.bfSize),fp);
    fread(&fileHeader.bfReserved1,1,sizeof(fileHeader.bfReserved1),fp);
    fread(&fileHeader.bfReserved2,1,sizeof(fileHeader.bfReserved2),fp);
    fread(&fileHeader.bfOffBits,1,sizeof(fileHeader.bfOffBits),fp);
    fread(&infoHeader,1,sizeof(tagBITMAPINFOHEADER),fp);
    emit Signal(3);
    //获取位图调色板
    for(int i=0;i<infoHeader.biClrUsed;i++){
        fread((char*)&(rgb[i]),1,sizeof(tagRGBQUAD),fp);
    }
    emit Signal(5);
    //获取size信息
    width = infoHeader.biWidth;
    height = infoHeader.biHeight;
    width = (width*sizeof(BYTE) + 3) / 4 * 4;
    emit Signal(7);
    //读取像素信息
    pixelData = new BYTE* [height];
    for(int i=0;i<height;i++){
        pixelData[i]=new BYTE[width];
        fread(pixelData[i],sizeof(BYTE)*width,1,fp);
    }
    fclose(fp);
    emit Signal(10);
    //将二位的像素表蛇形读成一维像素
    pixel = new BYTE[height*width];
    bool change = true;
    for(int i=0;i<height;i++){
        if(change == true){
            for(int j=0;j<width;j++)
                pixel[i*width+j]=pixelData[i][j];
            change = false;
        }
        else{
            for(int j=0;j<width;j++)
                pixel[i*width+j]=pixelData[i][width-j-1];
            change = true;
        }
    } 
    //释放指针
    for(int i=0;i<height;i++)
        delete[]pixelData[i];
    delete[]pixelData;
    emit Signal(15);
}

void Bitmap::writeBitmap(QString fileName){
    emit Signal(85);
    QByteArray ba = fileName.toLocal8Bit();
    char*file = ba.data();
    FILE *fp;
    fp = fopen(file,"wb");
    //写入位图头文件定义
    fwrite(&fileHeader.bfType,1,sizeof(fileHeader.bfType),fp);
    fwrite(&fileHeader.bfSize,1,sizeof(fileHeader.bfSize),fp);
    fwrite(&fileHeader.bfReserved1,1,sizeof(fileHeader.bfReserved1),fp);
    fwrite(&fileHeader.bfReserved2,1,sizeof(fileHeader.bfReserved2),fp);
    fwrite(&fileHeader.bfOffBits,1,sizeof(fileHeader.bfOffBits),fp);
    fwrite(&infoHeader,1,sizeof(tagBITMAPINFOHEADER),fp);
    emit Signal(87);
    //写入调色板
    for(int i=0;i<infoHeader.biClrUsed;i++){
        fwrite((char*)&(rgb[i]),1,sizeof(tagRGBQUAD),fp);
    }
    emit Signal(93);
    //写入像素值
    for(int i=0;i<height;i++)
       fwrite(pixelData[i],sizeof(BYTE)*width,1,fp);
    fclose(fp);
    emit Signal(100);
}

void Bitmap::Compress(QString fileName){
    emit Signal(20);
    QByteArray ba = fileName.toLocal8Bit();
    char*file = ba.data();
    FILE *fp;
    //将其他必要信息放入
    fp = fopen(file,"wb");
    fwrite(&fileHeader.bfType,1,sizeof(fileHeader.bfType),fp);
    fwrite(&fileHeader.bfSize,1,sizeof(fileHeader.bfSize),fp);
    fwrite(&fileHeader.bfReserved1,1,sizeof(fileHeader.bfReserved1),fp);
    fwrite(&fileHeader.bfReserved2,1,sizeof(fileHeader.bfReserved2),fp);
    fwrite(&fileHeader.bfOffBits,1,sizeof(fileHeader.bfOffBits),fp);
    fwrite(&infoHeader,1,sizeof(tagBITMAPINFOHEADER),fp); 
    emit Signal(23);
    //写入调色板
    for(int i=0;i<infoHeader.biClrUsed;i++){
        fwrite((char*)&(rgb[i]),1,sizeof(tagRGBQUAD),fp);
    }
    emit Signal(25);
    //压缩
    int size = height * width;
    int *l = new int[size+1];
    int *b = new int[size+1];
    int *s = new int[size+1];
    int Lmax = 256;
    int header = 11;
    s[0] = 0;
    int bmax;
    for (int i = 1; i <= size; i++){
        b[i] = length(pixel[i-1]);//计算像素点pixel需要的存储位数
        bmax = b[i];
        s[i] = s[i - 1] + bmax;
        l[i] = 1;
        for (int j = 2; j <= i && j <= Lmax; j++){
            if (bmax<b[i - j + 1]){
                bmax = b[i - j + 1];
            }
            if (s[i]>s[i - j] + j * bmax){
                s[i] = s[i - j] + j * bmax;
                l[i] = j;
            }
        }
        s[i] += header;
    }
    emit Signal(50);
    //获取每一段的长度存入Length数组
    int Size = size;
    vector<int> deLength;
    while (Size>0){
        if(Size-l[Size]>=0)
             deLength.push_back(l[Size]);
        Size = Size - l[Size];
    }
    vector<int> Length;
    for(int i=deLength.size()-1;i>=0;i--)
        Length.push_back(deLength[i]);
    emit Signal(70);
    //获得LengthBits信息和AllPixels信息
    int start=1;
    for(int i=0;i<Length.size();i++){
        BYTE bits = 0;
        for(int j=start;j<start+Length[i];j++){
            if(b[j]>bits)
                bits=b[j];
        }
        LengthBits += BYTEtoBit(Length[i]-1,8);
        LengthBits += BYTEtoBit(bits-1,3);
        for(int j=start;j<start+Length[i];j++)
            AllPixels+=BYTEtoBit(pixel[j-1],bits);
        start+=Length[i];
    }
    All=LengthBits+AllPixels;
    emit Signal(80);
    //All长度整成8的倍数，编辑成BYTE数组放入文件
    int AllLength=All.length();
    int Num=AllLength/8;
    int Add=AllLength-Num*8;
    if(Add!=0){
        for(int i=0;i<8-Add;i++)
            All+='0';
        Num++;
    }
    BYTE *inData=new BYTE[Num];
    for(int i=0;i<Num;i++){
        QString temp;
        for(int j=i*8;j<i*8+8;j++)
            temp+=All[j];
        inData[i]=StrtoBYTE(temp,8);
    }
    emit Signal(90);
    int partSize = Length.size();
    fwrite((char*)&partSize,1,4,fp);//写入段数
    fwrite((char*)&Num,1,4,fp);//写入长度
    fwrite((char*)inData,1,Num*sizeof(BYTE),fp);//写入段
    fclose(fp);
    delete[] l, b, s, pixel;
    emit Signal(100);
}

void Bitmap::UnCompress(){
    emit Signal(1);
    QByteArray ba = fileName.toLocal8Bit();
    char* file = ba.data();
    FILE *fp;
    fp = fopen(file,"rb");
    //获取位图头文件定义
    fread(&fileHeader.bfType,1,sizeof(fileHeader.bfType),fp);
    fread(&fileHeader.bfSize,1,sizeof(fileHeader.bfSize),fp);
    fread(&fileHeader.bfReserved1,1,sizeof(fileHeader.bfReserved1),fp);
    fread(&fileHeader.bfReserved2,1,sizeof(fileHeader.bfReserved2),fp);
    fread(&fileHeader.bfOffBits,1,sizeof(fileHeader.bfOffBits),fp);
    fread(&infoHeader,1,sizeof(tagBITMAPINFOHEADER),fp);
    emit Signal(10);
    //获取调色板
    for(int i=0;i<infoHeader.biClrUsed;i++){
        fread((char*)&(rgb[i]),1,sizeof(tagRGBQUAD),fp);
    }
    emit Signal(15);
    int partSize,Num;
    fread((char*)&partSize,1,4,fp);
    fread((char*)&Num,1,4,fp);
    BYTE *inData=new BYTE[Num];
    fread((char*)inData,1,Num*sizeof(BYTE),fp);
    for(int i=0;i<Num;i++){
        All+=BYTEtoBit(inData[i],8);
    }
    emit Signal(25);
    //获取size
    width = infoHeader.biWidth;
    height = infoHeader.biHeight;
    width = (width*sizeof(BYTE) + 3) / 4 * 4;
    pixel = new BYTE[height*width];
    int *l = new int[partSize];
    int *b = new int[partSize];
    bool ok = true;
    QString temp="";
    for(int i=0;i<partSize;i++){
        for(int j=i*11;j<i*11+8;j++)
            temp+=All[j];
        l[i]=StrtoInt(temp,8)+1;
        temp="";
        for(int j=i*11+8;j<i*11+11;j++)
            temp+=All[j];
        b[i]=StrtoInt(temp,3)+1;
        temp="";
    }
    emit Signal(40);
    int start=11*partSize;
    int begin=0,pixelNum=0;
    while(pixelNum+1<height*width){
        for(int i=0;i<l[begin];i++){
            for(int j=0;j<b[begin];j++){
                temp+=All[start];
                start++;
            }
            pixel[pixelNum]=StrtoBYTE(temp,b[begin]);
            temp="";
            pixelNum++;
        }
        begin++;
    }
    fclose(fp);
    emit Signal(60);
    pixelData = new BYTE* [height];
    for(int i=0;i<height;i++)
        pixelData[i]=new BYTE[width];
    for(int i=0;i<height;i++){
        if(ok == true){
            for(int j=0;j<width;j++)
                pixelData[i][j]=pixel[i*width+j];
            ok = false;
        }
        else{
            for(int j=0;j<width;j++)
                pixelData[i][width-j-1]=pixel[i*width+j];
            ok = true;
        }
    }
    emit Signal(70);
}

QString Bitmap::BYTEtoBit(BYTE data,int bit){
    QString str;
    BYTE temp;
    for(int i=8-bit;i<8;i++)
    {
        temp=data<<i;
        temp=temp>>7;
        if(temp==0)
            str+='0';
        else
            str+='1';
    }
    return str;
}

BYTE Bitmap::StrtoBYTE(QString str,int bit){
    BYTE value=0;
    for(int i=0;i<bit;i++){
        if(str[bit-i-1]=='1')
            value+=pow(2,i);
    }
    return value;
}

int Bitmap::StrtoInt(QString str,int bit){
    int value=0;
    for(int i=0;i<bit;i++)
    {
        if(str[bit-i-1]=='1')
            value+=pow(2,i);
    }
    return value;
}


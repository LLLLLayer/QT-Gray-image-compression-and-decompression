#include "huffman.h"

Huffman::Huffman(QString in)
{
    input=in;
}

void Huffman::HuffmanCompress(){
    char buf[512];
    unsigned char c;
    long n,m,i,j,f;
    long min1,pt1,flength=0,length1,length2; 	//记录最小的结点及文件的长度
    double div;                                 //保存压缩的比率
    FILE *ifp=NULL;                             //输入文件的指针
    FILE *ofp=NULL;                             //输出文件的指针

    QByteArray bty = input.toLocal8Bit();       //文件名QS听转换为char*
    const char* Cfilename = bty.data();

    ifp=fopen(Cfilename,"rb");                  //判断文件名是否有效
    if(ifp==NULL){
            return;                             //无效则推出程序
    }

    QString output=input;                        //设置压缩后文件
    output.replace(output.size()-4,4,".Huf");
    QByteArray bty2 = output.toLocal8Bit();       //文件名QS听转换为char*
    const char* outfilename = bty2.data();
    ofp=fopen(outfilename,"wb");

    flength=0;
    while(!feof(ifp)){                          //读取文件
            fread(&c,1,1,ifp);
            header[c].count++;
            flength++;
        }
    flength--;
        length1=flength;
        header[c].count--;
        for(i=0;i<512;i++){
            if(header[i].count!=0)
                header[i].b=(unsigned char)i;
            else
            header[i].b=0;
            header[i].parent=-1;header[i].lch=header[i].rch=-1;
        }
        for(i=0;i<256;i++){
                for(j=i+1;j<256;j++){
                    if(header[i].count<header[j].count){
                        tmp=header[i];
                        header[i]=header[j];
                        header[j]=tmp;
                    }
                }
            }
            for(i=0;i<256;i++)
                if(header[i].count==0)
                    break;
            n=i;
            m=2*n-1;
            for(i=n;i<m;i++){
                min1=99999999;
                for(j=0;j<i;j++){
                    if(header[j].parent!=-1)
                        continue;
                    if(min1>header[j].count){
                        pt1=j;
                        min1=header[j].count;
                        continue;
                    }
                }
                header[i].count=header[pt1].count;
                header[pt1].parent=i;
                header[i].lch=pt1;
                min1=999999999;
                for(j=0;j<i;j++){
                    if(header[j].parent!=-1)
                        continue;
                    if(min1>header[j].count){
                        pt1=j;
                        min1=header[j].count;
                        continue;
                    }
                }
                header[i].count+=header[pt1].count;
                header[i].rch=pt1;
                header[pt1].parent=i;
            }
            for(i=0;i<n;i++){
                f=i;
                header[i].bits[0]=0;
                while(header[f].parent!=-1){
                    j=f;
                    f=header[f].parent;
                    if(header[f].lch==j){
                        j=strlen(header[i].bits);
                        memmove(header[i].bits+1,header[i].bits,j+1);
                        header[i].bits[0]='0';
                    }
                    else{
                        j=strlen(header[i].bits);
                        memmove(header[i].bits+1,header[i].bits,j+1);
                        header[i].bits[0]='1';
                    }
                }
            }
            fseek(ifp,0,SEEK_SET);
            fwrite(&flength,4,1,ofp);

            fseek(ofp,8,SEEK_SET);
            buf[0]=0;
            f=0;
            pt1=8;
            while(!feof(ifp)){
                c=fgetc(ifp);
                f++;
                for(i=0;i<n;i++){
                    if(c==header[i].b)
                        break;
                }
                strcat(buf,header[i].bits);
                j=strlen(buf);
                c=0;
                while(j>=8){
                    for(i=0;i<8;i++){
                        if(buf[i]=='1')
                            c=(c<<1)|1;
                        else
                            c=c<<1;
                    }
                    fwrite(&c,1,1,ofp);
                    pt1++;
                    strcpy(buf,buf+8);
                    j=strlen(buf);
                }
                if(f==flength)
                    break;
            }
            if(j>0){
                strcat(buf,"00000000");
                for(i=0;i<8;i++){
                    if(buf[i]=='1')
                        c=(c<<1)|1;
                    else
                        c=c<<1;
                }
                fwrite(&c,1,1,ofp);
                pt1++;
            }
            fseek(ofp,4,SEEK_SET);
            fwrite(&pt1,sizeof(long),1,ofp);
            fseek(ofp,pt1,SEEK_SET);
            fwrite(&n,sizeof(long),1,ofp);
            for(i=0;i<n;i++){
                fwrite(&(header[i].b),1,1,ofp);
                c=strlen(header[i].bits);
                fwrite(&c,1,1,ofp);
                j=strlen(header[i].bits);
                if(j%8!=0){
                    for(f=j%8;f<8;f++)
                        strcat(header[i].bits,"0");
                }
                while(header[i].bits[0]!=0){
                    c=0;
                    for(j=0;j<8;j++){
                        if(header[i].bits[j]=='1')
                            c=(c<<1)|1;
                        else
                            c=c<<1;
                    }
                    strcpy(header[i].bits,header[i].bits+8);
                    fwrite(&c,1,1,ofp);
                }
            }
            length2=pt1--;
            div=((double)length1-(double)length2)/(double)length1;
            fclose(ifp);
            fclose(ofp);
            //QMessageBox::information(this,"压缩成功","压缩率为"+QString::number(div*100)+"%","确定");
}

void Huffman::HuffmandeCompress(QString&out){
    char buf[255],bx[255];
        unsigned char c;
        long i,j,m,n,f,p,l;
        long flength;
        FILE *ifp=NULL;
        FILE *ofp=NULL;
        QByteArray bty = input.toLocal8Bit();
        const char* Cfilename = bty.data();
        ifp=fopen(Cfilename,"rb");
        if(ifp==NULL){
            return;                             //无效推出程序
        }

        QString output=input;
        output.replace(output.size()-5,5,"");                      //设置压缩后文件
        out=output;
        QByteArray bty2 = output.toLocal8Bit();       //文件名QS听转换为char*
        const char* outfilename = bty2.data();
        ofp=fopen(outfilename,"wb");

        fread(&flength,sizeof(long),1,ifp);
        fread(&f,sizeof(long),1,ifp);
        fseek(ifp,f,SEEK_SET);
        fread(&n,sizeof(long),1,ifp);
        for(i=0;i<n;i++){
            fread(&header[i].b,1,1,ifp);
            fread(&c,1,1,ifp);
            p=(long)c;
            header[i].count=p;
            header[i].bits[0]=0;
            if(p%8>0)
                m=p/8+1;
            else
                m=p/8;
            for(j=0;j<m;j++){
                fread(&c,1,1,ifp);
                f=c;
                itoa(f,buf,2);
                f=strlen(buf);
                for(l=8;l>f;l--){
                    strcat(header[i].bits,"0");
                }
                strcat(header[i].bits,buf);
            }
        header[i].bits[p]=0;
        }
        for(i=0;i<n;i++){
            for(j=i+1;j<n;j++){
                if(strlen(header[i].bits)>strlen(header[j].bits)){
                    tmp=header[i];
                    header[i]=header[j];
                    header[j]=tmp;
                }
            }
        }
        p=strlen(header[n-1].bits);
        fseek(ifp,8,SEEK_SET);
        m=0;
        bx[0]=0;
        while(1){
            while(strlen(bx)<(unsigned int)p){
                fread(&c,1,1,ifp);
                f=c;
                itoa(f,buf,2);
                f=strlen(buf);
                for(l=8;l>f;l--){
                    strcat(bx,"0");
                }
                strcat(bx,buf);
            }
        for(i=0;i<n;i++){
            if(memcmp(header[i].bits,bx,header[i].count)==0)
                break;
        }
        strcpy(bx,bx+header[i].count);
        c=header[i].b;
        fwrite(&c,1,1,ofp);
        m++;
        if(m==flength)
            break;
        }
        fclose(ifp);
        fclose(ofp);
}

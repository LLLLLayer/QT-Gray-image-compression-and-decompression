#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->progressBar->setRange(0,100);
    this->ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpen_clicked()
{
    QString Qfilename = QFileDialog::getOpenFileName(this,"Open", QDir::currentPath());
    this->ui->FileName->setText(Qfilename);
    on_btnOK_clicked();
}

void MainWindow::on_btnOK_clicked()
{
    input=this->ui->FileName->text();
    QByteArray bty = input.toLocal8Bit();
    const char* Cfilename = bty.data();
    stat(Cfilename, &buf);
    //图片信息输出
    ui->Show->setText("图片地址：" + input);
    ui->Show->append("图片大小："+QString::number(buf.st_size)+ "字节");
    QFileInfo fileInfo(input);
    QDateTime buildTime = fileInfo.created();
    QDateTime lastMdTime = fileInfo.lastModified();
    QDateTime lastVisitTime = fileInfo.lastRead();
    ui->Show->append("创建时间："+buildTime.toString());
    ui->Show->append("最近修改："+lastMdTime.toString());
    ui->Show->append("最近访问："+lastVisitTime.toString());
    QFile theFile(input);
    theFile.open(QIODevice::ReadOnly);
    QByteArray ba1 = QCryptographicHash::hash(theFile.readAll(), QCryptographicHash::Md5);
    QByteArray ba2 = QCryptographicHash::hash(theFile.readAll(), QCryptographicHash::Sha1);
    theFile.close();
    ui->Show->append("MD5："+ba1.toHex());
    ui->Show->append("Sha1："+ba2.toHex());
}

void MainWindow::on_Compress_clicked()
{
    if (input.isEmpty()){//打开文件失败
        QMessageBox::warning(NULL, "Error","读取图片失败!",QMessageBox::Yes);
        return;
    }
    QImage img;
    img.load(input);
    if(img.depth()!=8){//位深度非8
        QMessageBox::warning(NULL, "Error", "位深度非8!", QMessageBox::Yes);
        return;
    }
    output=input;
    output.replace(output.size()-4,4,".Img");
    output=QFileDialog::getSaveFileName(this,"Save",output,"(.Img)");
    if (output.isEmpty()){
        QMessageBox::warning(this, tr("Error"),tr("保存文件失败!"));
        return;
    }

    timeValue=0;
    //QTimer *timer = new QTimer();
    //connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
    //timer->start(1);
    Bitmap Object1(input);
    connect(&Object1, SIGNAL(Signal(int)), this, SLOT(updateProgressBar(int)));

    Object1.readBitmap();
    Object1.Compress(output);
    //Huffman ObjectHuff1(output);
    //ObjectHuff1.HuffmanCompress();
    QMessageBox::information(this, tr("提示"),tr("压缩完成!"));
    //disconnect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
    this->ui->progressBar->setValue(0);
}

void MainWindow::on_Decompress_clicked()
{
    if (input.isEmpty()){//打开文件失败
        QMessageBox::warning(NULL, "Error","读取失败!",QMessageBox::Yes);
        return;
    }
    output=input;
    output.replace(output.size()-4,4,"New.bmp");
    output=QFileDialog::getSaveFileName(this,"Save",output);
    if (output.isEmpty()){
        QMessageBox::warning(this, tr("Error"),tr("保存文件失败!"));
        return;
    }
    //timeValue=0;
    //QTimer *timer = new QTimer();
    //connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
    //timer->start(1);


    //Huffman ObjectHuff2(input);
    //ObjectHuff2.HuffmandeCompress(input);
    Bitmap Object2(input);
    connect(&Object2, SIGNAL(Signal(int)), this, SLOT(updateProgressBar(int)));
    Object2.UnCompress();
    Object2.writeBitmap(output);
    QMessageBox::information(this, tr("提示"),tr("解压完成!"));
    //disconnect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
    this->ui->progressBar->setValue(0);

}

void MainWindow::on_btnClose_clicked()
{
    this->close();
}

void MainWindow::on_progressBar_valueChanged(int value)
{
 //
}

void MainWindow::updateProgressBar(int value){
    this->ui->progressBar->setValue(value);
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<iostream>
#include<QString>
#include<QFileDialog>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<math.h>
#include<fstream>
#include<QDateTime>
#include<QCryptographicHash>
#include<qDebug>
#include<QThread>
#include<QMessageBox>
#include<bitmap.h>
#include<QTimer>
#include<huffman.h>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    struct stat buf;

private slots:
    void on_btnOpen_clicked();

    void on_btnOK_clicked();

    void on_Compress_clicked();

    void on_Decompress_clicked();

    void on_btnClose_clicked();

    void on_progressBar_valueChanged(int value);

    void updateProgressBar(int value);

private:
    Ui::MainWindow *ui;
    QString input,output;

    int timeValue;
};

#endif // MAINWINDOW_H

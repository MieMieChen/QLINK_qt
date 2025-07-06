#include "mainwindow.h"
#include <QApplication>
//测试的时候注释掉下面的部分
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

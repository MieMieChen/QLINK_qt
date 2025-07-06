#pragma once
#include <QObject>
#include <QtTest/QtTest>
#include <QLineEdit>
#include "constNum.h"
class SimpleTest :public QObject
{
    Q_OBJECT;
private slots:
    //在测试类中，通过private slots的方式，声明对应的测试函数
    void testcase();
public:
    SimpleTest();
};


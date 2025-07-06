#pragma once

#include<QTimer>
#include <QLabel>
#include <QWidget>
#include <QFont>
#include "constNum.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
class Timer :public QWidget
{
    Q_OBJECT;
public:
    Timer(QWidget *parent = nullptr);
    ~Timer();
    QTimer *time= NULL;
    QLabel *timeLine= NULL;//tips:使用指针可以把这个子部件固定在Timer的对象上
    void sentBonus();
    QJsonObject getJsonData() const;
    void setRemainTime(int x);
    void setShowHint(bool x);
public slots:
    //countdown from 60
    void reLoad();
    void addTime();
    void stop();
    void Continue();
    void toJson() const;
    void SetHint(bool x);
signals:
    void timeOut();
    void shuffle();
    void hint();
    void flash();
    void add();
    void bonus(int value);
    void finalTime(int value);
    void freeze();
    void melt();
private:
    mutable QJsonObject TimerData;
    int remainTime = GAMETIME;
    bool showHint = false;
};


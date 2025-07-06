#include "timer.h"
#include "constNum.h"
#include <QRandomGenerator>

Timer::Timer(QWidget *parent):QWidget(parent){
    QFont font("Arial",80,QFont::Bold);
    this->resize(200,110);
    timeLine = new QLabel(this);
    timeLine->resize(200,110);
    timeLine->setFont(font);
    timeLine->setStyleSheet("color:darkgreen;background-color:lightgreen;");
    timeLine->setAlignment(Qt::AlignCenter);
    time = new QTimer(this);
    time->start(1000);
    connect(time, &QTimer::timeout,this,&Timer::reLoad);
}
void Timer::stop()
{
    time->stop();
    emit freeze();
}
void Timer::Continue()
{
    time->start(1000);
}
void Timer::reLoad() //实现倒计时的更新
{
    if(remainTime>0){
        if(showHint)
            timeLine->setStyleSheet("color:red;background-color:lightgreen;");
        else
            timeLine->setStyleSheet("color:darkgreen;background-color:lightgreen;");
        timeLine->setText(QString("%1").arg(remainTime--));
        this->sentBonus();
    }
    else{
        QFont font("Arial",50,QFont::Bold);
        this->resize(320,110);
        timeLine->resize(320,110);
        timeLine->setFont(font);
        timeLine->setStyleSheet("color:darkred;background-color:lightred");
        timeLine->setText("TIME OUT!");
        time->stop();//可以把时间暂停 这个可以！
        emit timeOut();
    }
}
void Timer::sentBonus()
{
    int Moment = QRandomGenerator::global()->bounded(1,60);
    int value = QRandomGenerator::global()->bounded(1,5);//1,2,3，4
    if(remainTime==Moment)
    {
        if(value==1)
        {
            emit bonus(1);
        }
        if(value==2)
            emit bonus(2);//从计时器发出信号，已经和map的shuffle联系在一起了，
        if(value==3)
        {
            emit bonus(3);
        }
        // if(value==4)
        // {
        //     emit bonus(4);
        // }
    }
    // if(remainTime==55)
    // {
    //     //qDebug() <<"send bonus";
    //     emit bonus(1);
    // }
    // if(remainTime==50)
    // {
    //     //qDebug() <<"send bonus";
    //     emit bonus(3);
    // }
    // if(remainTime==45)
    // {
    //     //qDebug() <<"send bonus";
    //     emit bonus(1);
    // }
}
void Timer::addTime()
{
    qDebug()<<remainTime;
    remainTime+=30;
    qDebug()<<remainTime;

}
void Timer::SetHint(bool x)
{
    showHint  = x;
}
Timer::~Timer()
{
    delete time;
    delete timeLine;
}
QJsonObject Timer::getJsonData() const
{
    return TimerData;
}
//tips
//如果在类的成员函数后面加上 const 关键字，那么这个函数就被称为 const 成员函数。const 成员函数不能修改类的成员变量，除非这些成员变量被声明为 mutable
//const 成员函数通常与 const 对象一起使用。如果一个对象被声明为 const，只能调用它的 const 成员函数
void Timer::toJson()const
{
    qDebug() <<"timer json";
    QJsonObject timeLineJ;
    timeLineJ["width"] = timeLine->width();
    timeLineJ["height"] = timeLine->height();
    timeLineJ["fontfamily"] = timeLine->font().family();
    timeLineJ["fontsize"] = timeLine->font().pointSize();
    timeLineJ["fontbold"] = timeLine->font().bold();
    timeLineJ["stylesheet"] = timeLine->styleSheet();
    timeLineJ["Text"] = timeLine->text();
    QJsonValue alignmentStr;
    if (timeLine->alignment() == Qt::AlignLeft) {
        alignmentStr = "AlignLeft";
    } else if (timeLine->alignment() == Qt::AlignRight) {
        alignmentStr = "AlignRight";
    } else if (timeLine->alignment() == Qt::AlignHCenter) {
        alignmentStr = "AlignHCenter";
    } else if (timeLine->alignment() == Qt::AlignCenter) {
        alignmentStr = "AlignCenter";
    }
    timeLineJ["alignment"] = alignmentStr;
    TimerData["timeLine"] = timeLineJ;
    TimerData["remainTime"] = remainTime;
    TimerData["timerState"] = time->isActive();
    TimerData["showHint"] = showHint;
    //qDebug() << "TimerData content:" << QJsonDocument(TimerData).toJson(QJsonDocument::Indented);

}
void Timer::setRemainTime(int x)
{
    remainTime = x;
}
void Timer::setShowHint(bool x)
{
    showHint = x;
}
























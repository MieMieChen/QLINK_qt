#pragma once
#include <QPainter>
#include <QQueue>
#include <QWidget>
#include <QKeyEvent>
#include <QLabel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QBuffer>
class Player :public QWidget
{
    Q_OBJECT
public:
    Player(QWidget *parent = nullptr);
    enum Style{
        style1,//hero1
        style2//hero2
    };
    Style random();
    void setStyle(Style style);
    //used for move
    void pMove(int dir);
    QPixmap pixmap;
    void randomPosition();
    void setPosition(int x,int y);
    QPoint pointA ; //tips:不可以写一个指针但是不指向任何东西！！！！！要不然后面访问->rx()就g了
    //if i want to get player's position we visit pointB
    QPoint pointB ;
    ~Player()
    {
        delete pixmap1;
        delete pixmap2;
        delete imageLabel;
        delete trc_Line;
    }
    Style currentStyle;
    void paintPlayer(QPainter &painter);
    //to record player's path:not used for testify answer,but used for testify whether crash
    std::vector<QLine> *trc_Line;
    //used for record trace that will be demonstrated
    QQueue<QLine> *trace = NULL;

    std::vector<QPoint> highlightBlock;
    int highlightNum = 0;
    bool highlightState = false;
    int scores = 0;
    int anchorNum = 0;
    QJsonObject getJsonData() const;
    // convert QPixmap into  string in Base64
    QJsonValue pixmapToJsonValue(const QPixmap &pixmap)const;
    // read strings in Base64 from JSON and convert into QPixmap  tips:Base64 是一种基于 64 个可打印字符来表示二进制数据的编码方式。它常用于在需要以文本格式传输或存储二进制数据时
    QPixmap jsonValueToPixmap(const QJsonValue &value)const;
    QLabel *imageLabel = NULL;
signals:
    void match();
    void testifyCrash();
public slots:
    void getScore(int value);
    void toJson()const ;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    mutable QJsonObject PlayerData;
    QPixmap *pixmap1 = new QPixmap("C:\\data\\QT_PRACTICE\\QLINK\\image\\hero1.jpg");
    QPixmap *pixmap2 = new QPixmap("C:\\data\\QT_PRACTICE\\QLINK\\image\\hero2.jpg");
};

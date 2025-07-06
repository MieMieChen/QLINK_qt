#pragma once
#include<QWidget>
#include<QKeyEvent>
#include<QPainter>
#include "blocks.h"
#include "player.h"
#include <QTimer>
#include <QQueue>
#include <QPoint>
#include <QPropertyAnimation>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QByteArray>
#include <QBuffer>
#include "constNum.h"
class Map :public QWidget
{
    Q_OBJECT
public:
    //two methods for building map
    //map for gaming
    Map(QWidget *parent=nullptr);
    //map for test
    Map(QWidget *parent,int x);
    //two states of map
    enum Mode{
        mode1,//solo mode
        mode2//battle mode
    };
    ~Map();
    //set Map'sprivate members
    void setMode(Mode mode);
    void setPaintmap(bool x);
    //functions for painting gadgets on map
    void paintMap(QPainter &painter);
    void paintFinalTrace(QPainter &painter, Player *hero);
    void paintHighlight(QPainter &painter,Player *hero);
    void paintBonus(QPainter &painter);
    void paintGains(QPainter &painter);
    void paintHints(QPainter &painter);
    //to testify whether it is in range
    bool isValid(int x,int y,Blocks*** a);
    //to testify whether game over
    bool isGameOver();
    bool isLock();

    Mode currentMode;
    //to pause 1 seocnd because i want to make highlights more visible
    void pause(int milliseconds)
    {
        QEventLoop loop;
        QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
        loop.exec();
    }
    //affiliate players to map
    Player *hero1 = NULL,*hero2 = NULL;
    //affiliate bonus to map
    Blocks *bonus_1= NULL,*bonus_2= NULL,*bonus_3= NULL,*bonus_4= NULL;
    //used for tracing
    struct Node{
        int i,j;//i,j对应的是这个node在blocks里面对应的下标
        int corners = 0;
        int dir;//1,2,3,4——对应上下左右
        Node(int x,int y,int a,int b):i(x),j(y),corners(a),dir(b){};
    };
    //affiliate blocks to map
    Blocks ***blocks= NULL;

    // to record blocks that have been erased(or the edge) we record coordiantes instead of i/j
    std::vector<QPoint> *deadBlock;
    // used for timing; timer1/timer2 is used to detect long/short push; timer3 is used for couting down for the hint;
    QTimer *timer1= NULL,*timer2= NULL,*timer3= NULL;

    QJsonObject getJsonData() const;
    // used for setting private members;
    void setGameOver(bool x);
    void setLock(bool x);
    void setPaintMap(bool x);
    void setPaintTrace1(bool x);
    void setPaintTrace2(bool x);
    void setErase(bool x);
    void setBonus1(bool x);
    void setBonus2(bool x);
    void setBonus3(bool x);
    void setBonus4(bool x);
    void setIsKeyPressed1(bool x);
    void setIsKeyPressed2(bool x);
    void setPressedKey1(int x);
    void setPressedKey2(int x);
    void setTarget1(int x,int y);
    void setTarget2(int x,int y);
    void setTarget3(int x,int y);
    void setTarget4(int x,int y);
    void setType1Count(int x);
    void setType2Count(int x);
    void setType3Count(int x);
    void setClickPoint(int x,int y);
    void setCouple1(int x,int y);
    void setCouple2(int x,int y);
    void setShowHint(bool x);
    void setPaintFinalTrace1(bool x);
    void setPaintFinalTrace2(bool x);
    void setRemainTime(int x);

    QPixmap jsonValueToPixmap(const QJsonValue &value)const;
    //used for testing corners
    bool oneLineLink(int i_1,int i_2,int j_1,int j_2,int playerId);
    bool doubleLineLink(int i_1,int i_2,int j_1,int j_2,int playerId);
    bool tripleLineLink(int i_1,int i_2,int j_1,int j_2,int platerId);
    // to testify whether this game is solvable or whether there is a player win
    bool testify_Game();
    void shuffle( );
signals:
    void gameFinished();
    void lock();
    void attention(int value);//不同的value对应不同的情况 1对应anchor了两个 2对应
    void paint_trc(int value);
    void addtime();
    void erase_Couple();
    void markHint(bool showHint);
public slots:
    void freeze();
    void melt();
    void bonus(int value);
    //获得scoreboard传来的分数
    void getGains();
    void hint();
    void toJson()const;
    void reload();
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    mutable QJsonObject MapData;
    //used to testify some game states;
    bool testify(int x,int y,Blocks*** a);
    bool testify_Erase(Player *hero);
    bool testify_Crash(int dir,const Player *hero);
    bool testify_Angle(const QLine &a,const QLine &b);
    //for hint skills
    QPoint getCouple(int i,int j,Blocks ***a);
    // used for highlighting block which is activated
    void highLight(QPoint x,Player *hero);

    void anchor(QPoint X,Player *hero);
    // not implement
    void flash(Player *hero,QPoint *a);
    //to generate a random vector(for shuffle)
    std::vector<int> randomGenerator(int a, int b, int c,int d);

    bool paintFinalTrace1 = false;
    bool paintFinalTrace2 = false;
    bool Lock= true;
    bool paintmap = true;
    bool painttrace1 = false;
    bool painttrace2 = false;
    bool erase = false;
    bool gameOver = false;

    //used for manage bonus
    bool bonus1 = false,bonus2 = false,bonus3 = false,bonus4 = false;//1，2，3，4的对应
    //used for long/short push
    bool isKeyPressed1 = false;
    int pressedKey1;
    bool isKeyPressed2 = false;
    int pressedKey2;

    //position where bonus will be placed
    QPoint target1=QPoint(-1,-1),target2=QPoint(-1,-1),target3=QPoint(-1,-1),target4=QPoint(-1,-1);
    //used for shuffle to record the number of different type of blocks still alive.
    int type1Count = 0,type2Count = 0,type3Count = 0;
    QPoint *clickPoint = NULL;
    //used for hint skills
    QPoint couple1=QPoint(0,0),couple2=QPoint(0,0);
    bool showHint = false;
    int timerHRemain = 10;
private slots:
    void handleLongPress1();
    void handleLongPress2();
};


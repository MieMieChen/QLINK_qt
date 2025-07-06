#pragma once
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QRandomGenerator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QBuffer>
class Blocks :public QWidget
{
    Q_OBJECT
public:
    //用于生成地图当中的blocks,用户输入的m*n对应的就是blocks看起来的个数，但是实际上是都要单独+2的。
    Blocks(QWidget *parent=nullptr);
    //用于生成最初游戏开始时map外围的blocks
    Blocks(QWidget *parent,int x = 0);
    //用于在使用shuffle技能后，重新生成blocks，其中state代表deadState,x对应类型type_ :x = 0,type0; x = 1，type1; x = 2,type2; x = 3,type3;
    Blocks(QWidget *parent,bool state = false,int x = -1);
    ~Blocks();
    //type0 对应一开始就生成的空白边框类型，type1,type2,type3则对应了三种不同的方块
    enum Type{
        type0,
        type1,
        type2,
        type3
    };
    //对于type1-3进行类型的随机生成
    Type random();
    Type currentType;
    //设置block位于map上面的位置。以及对应的imagelabel的位置
    void setPosition(int x, int y);
    int X,Y;
    //分别对应于两个玩家，这块block是被hero1选中，还是被hero2选中
    bool anchorState1 = false;
    bool anchorState2 = false;
    //用于实现消消乐，是否已经被角色消除掉，如果这块已经变成了黄色（视觉上），那么就是true，能显示自己的样子的是false
    bool deadState = false;
    //用于实现hints技能 coupled则是被算法选中和另外一个block配对，展现给用户
    bool coupled = false;
    //禁用类的拷贝构造函数和拷贝赋值运算
    Blocks(const Blocks&) = delete;
    Blocks& operator=(const Blocks&) = delete;
    QPixmap pixmap;
    QJsonObject getJsonData() const;
    // 将 QPixmap 转换为 Base64 字符串
    QJsonValue pixmapToJsonValue(const QPixmap &pixmap)const;
    // 从 JSON 读取 Base64 字符串并转换回 QPixmap
    QPixmap jsonValueToPixmap(const QJsonValue &value)const;
    QLabel *imageLabel = NULL;
public slots:
    void toJson()const;
private:
    mutable QJsonObject BlocksData;
    QPixmap resizePixmap(QPixmap *pixmap_tmp1);
    QPixmap *pixmap1 = new QPixmap("C:\\data\\QT_PRACTICE\\QLINK\\image\\type1.jpg");
    QPixmap *pixmap2 = new QPixmap("C:\\data\\QT_PRACTICE\\QLINK\\image\\type2.jpg");
    QPixmap *pixmap3 = new QPixmap("C:\\data\\QT_PRACTICE\\QLINK\\image\\type3.jpg");
    QPixmap *pixmap4 = new QPixmap("C:\\data\\QT_PRACTICE\\QLINK\\image\\addtime.png");
    QPixmap *pixmap5 = new QPixmap("C:\\data\\QT_PRACTICE\\QLINK\\image\\shuffle.png");
    QPixmap *pixmap6 = new QPixmap("C:\\data\\QT_PRACTICE\\QLINK\\image\\hint.png");
    QPixmap *pixmap7 = new QPixmap("C:\\data\\QT_PRACTICE\\QLINK\\image\\flash.png");
    //被按照比例裁剪好的pixmap。
    QPixmap resizedPixmap1;
    QPixmap resizedPixmap2;
    QPixmap resizedPixmap3;
};

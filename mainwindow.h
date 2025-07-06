#pragma once
#include "map.h"
#include <QMainWindow>
#include <QKeyEvent>
#include <QPainter>
#include <QPalette>
#include <QTime>
#include <QTimer>
#include <QWidget>
#include <QGraphicsView>
#include <QList>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "constNum.h"
#include <QToolButton>
#include "timer.h"
#include "scoreboard.h"
#include <QLineEdit>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QBuffer>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //这个MODE对应不同的游戏类型
    enum MODE{
        MODE1,//单人游戏
        MODE2,//双人游戏
    };
    QJsonObject getMainWindowData() const;
    void paintGains(QPainter &painter);
public slots:
    //welcome 界面
    void TO_HOME();
    //从welcome（home）界面转换到login界面
    void HOME_LOGIN();
    //去到about界面，选择load game or new game or exit game
    void TO_ABOUT();
    //在about界面选择了new game 然后进入到选择solo or battle的menu界面
    void LOGIN_MENU();
    //创建solo界面，并显示
    void MENU_SOLO();
    //创建battle界面，并显示
    void MENU_BATTLE();
    //将游戏暂存
    void toJson();
    //重载游戏数据
    void JsonToALL();
    //用户在游戏界面点击pause game所弹出的窗口，用于进行save and load
    void showPopUpWindow();
    //用户在about界面选择load game 进行load
    void loadGameWindow();
    void open();
    void save();

private:
    mutable QJsonObject MainWindowData;
    Ui::MainWindow *ui;
    MODE currentMode;
    Map *map = NULL;
    //对应按键
    QToolButton *soloButton= NULL,*battleButton= NULL,*startButton= NULL,*aboutButton= NULL,*exitButton= NULL,*logInButton= NULL,*quitGameButton= NULL,*startGameButton = NULL,*loadGameButton = NULL,*pauseButton= NULL,*continueButton= NULL,*saveButton= NULL,*loadButton= NULL;
    //对应全局记时的时钟的显示器
    Timer *TimeCounter = NULL;
    //计分板
    ScoreBoard *ScoreBoard1= NULL,*ScoreBoard2= NULL;
    //用于切换的不同的scene；
    QGraphicsScene *HOME= NULL,*LOG_IN= NULL,*ABOUT= NULL,*SOLO= NULL,*MENU= NULL,*BATTLE= NULL;
    QLineEdit *user= NULL,*password= NULL;
    QString filePath; //获得了load game的文件路径
    QString saveFilePath;
    bool load = false;
protected:
    void paintEvent(QPaintEvent *event) override;
    QTimer *timer = NULL;
    // void mouseMoveEvent(QMouseEvent *event) override {
    //     // 获取鼠标在视图中的坐标
    //     QPoint viewPos = event->pos();

    //     // 将视图坐标转换为场景坐标
    //     QPointF scenePos = mapToScene(viewPos);

    //     // 更新 QLabel 显示内容
    //     label->setText(QString("View: [%1, %2]\nScene: [%3, %4]")
    //                        .arg(viewPos.x()).arg(viewPos.y())
    //                        .arg(scenePos.x()).arg(scenePos.y()));
    //     label->move(viewPos + QPoint(10, 10));  // 将标签移动到鼠标附近
    //     label->setFixedSize(150, 40);
    //     // 调用父类的 mouseMoveEvent
    //     QGraphicsView::mouseMoveEvent(event);
    // }
    // void keyPressEvent(QKeyEvent *event)override;
    void paintTrace(QPainter &painter);
private:
    void repositionMap() {
        int side = qMin(width(), height());
        map->setGeometry(side / 2 , side / 2 - HEIGHT/2, WIDTH, HEIGHT);  // 重新设置 map widget 的几何属性
    }
    QLabel *label= NULL;
};

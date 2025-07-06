#include <QGraphicsProxyWidget>
#include "mainwindow.h"
#include "player.h"
#include <QPalette>
#include <QDebug>
#include <QToolTip>
#include <QMouseEvent>
#include <QPushButton>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QFont>
#include <QLineEdit>
#include <QScrollBar>
#include "constNum.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QLabel>
MainWindow::MainWindow(QWidget *parent)
    : QGraphicsView(parent){
    label = new QLabel(this);
    label->setStyleSheet("QLabel { background-color : white; color : black; }");
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    this->horizontalScrollBar()->blockSignals(true);
    this->verticalScrollBar()->blockSignals(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QColor lightYellow(255, 255, 204);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, lightYellow);
    this->setPalette(palette);
    resize(1500, 800);
    TO_HOME();
}

MainWindow::~MainWindow(){
    delete map;
    delete timer;
    delete soloButton;
    delete battleButton;
    delete startButton;
    delete aboutButton;
    delete exitButton;
    delete logInButton;
    delete quitGameButton;
    delete loadGameButton;
    delete startGameButton;
    delete continueButton;
    delete saveButton;
    delete loadButton;
    delete TimeCounter;
    delete ScoreBoard1;
    delete ScoreBoard2;
    delete HOME;
    delete SOLO;
    delete MENU;
    delete BATTLE;
    delete LOG_IN;
    delete ABOUT;
    delete user;
    delete password;
    delete label;
}
void MainWindow::toJson(){

    qDebug()<<"mainwindow json";
    MainWindowData["currentMode"] = currentMode;
    QJsonObject exitButtonJ,pauseButtonJ,scoreBoard1J,scoreBoard2J,mapJ,timerJ,continueButtonJ;
    if(exitButton){
        exitButtonJ["Autoraise"] = exitButton->autoRaise();
        exitButtonJ["StyleSheet"] = exitButton->styleSheet();
        exitButtonJ["Text"] = exitButton->text();
        exitButtonJ["FontFamily"] = exitButton->font().family();
        exitButtonJ["FontSize"] = exitButton->font().pointSize();
        exitButtonJ["FontBold"] = exitButton->font().bold();
        exitButtonJ["Width"] = exitButton->width();
        exitButtonJ["Height"] = exitButton->height();
        exitButtonJ["X"] = exitButton->x();
        exitButtonJ["Y"] = exitButton->y();
        MainWindowData["exitButton"] = exitButtonJ;
    }
    if(pauseButton){
        pauseButtonJ["Autoraise"] = pauseButton->autoRaise();
        pauseButtonJ["StyleSheet"] = pauseButton->styleSheet();
        pauseButtonJ["Text"] = pauseButton->text();
        pauseButtonJ["FontFamily"] = pauseButton->font().family();
        pauseButtonJ["FontSize"] = pauseButton->font().pointSize();
        pauseButtonJ["FontBold"] = pauseButton->font().bold();
        pauseButtonJ["Width"] = pauseButton->width();
        pauseButtonJ["Height"] = pauseButton->height();
        pauseButtonJ["X"] = pauseButton->x();
        pauseButtonJ["Y"] = pauseButton->y();
        MainWindowData["pauseButton"] = pauseButtonJ;
    }
    if(continueButton){
        continueButtonJ["Autoraise"] = continueButton->autoRaise();
        continueButtonJ["StyleSheet"] = continueButton->styleSheet();
        continueButtonJ["Text"] = continueButton->text();
        continueButtonJ["FontFamily"] = continueButton->font().family();
        continueButtonJ["FontSize"] = continueButton->font().pointSize();
        continueButtonJ["FontBold"] = continueButton->font().bold();
        continueButtonJ["Width"] = continueButton->width();
        continueButtonJ["Height"] = continueButton->height();
        continueButtonJ["X"] = continueButton->x();
        continueButtonJ["Y"] = continueButton->y();
        MainWindowData["continueButton"] = continueButtonJ;
    }
    if(ScoreBoard1){
        scoreBoard1J["X"] = ScoreBoard1->x();
        scoreBoard1J["Y"] = ScoreBoard1->y();
        MainWindowData["ScoreBoard1"] = ScoreBoard1->getJsonData();
        MainWindowData["scoreBoard1J"] =scoreBoard1J;
    }
    if(map){
        mapJ["X"] = map->x();
        mapJ["Y"] = map->y();
        MainWindowData["mapJ"] = mapJ;
        MainWindowData["map"] = map->getJsonData();
    }
    if(TimeCounter){
        timerJ["X"] = TimeCounter->x();
        timerJ["Y"] = TimeCounter->y();
        MainWindowData["TimeCounterJ"] = timerJ;
        qDebug() << "TimerData in mainwindow:" << QJsonDocument(TimeCounter->getJsonData()).toJson(QJsonDocument::Indented);
        MainWindowData["TimeCounter"] = TimeCounter->getJsonData();
    }
    if(currentMode==MODE2){
        if(ScoreBoard2){
            scoreBoard2J["X"] = ScoreBoard2->x();
            scoreBoard2J["Y"] = ScoreBoard2->y();
            MainWindowData["scoreBoard2J"] = scoreBoard2J;
            MainWindowData["ScoreBoard2"] = ScoreBoard2->getJsonData();
        }
    }
    MainWindowData["WIDTH"] = WIDTH;
    MainWindowData["HEIGHT"] = HEIGHT;
    MainWindowData["MAP_SIZE"] = MAP_SIZE;
    MainWindowData["MAP_SIZEX"] = MAP_SIZEX;
    MainWindowData["MAP_SIZEY"] = MAP_SIZEY;
    MainWindowData["BLOCKS_SIZE"] = BLOCKS_SIZE;
    MainWindowData["num"] = num;
    MainWindowData["GAMETIME"] = GAMETIME;
    delete map;
    delete TimeCounter;
    delete ScoreBoard1;
    delete ScoreBoard2;
    delete exitButton;
    delete pauseButton;
    delete continueButton;
    if(currentMode==MODE1){
        this->setScene(nullptr);
        delete SOLO;
    }
    if(currentMode==MODE2){
        this->setScene(nullptr);
        delete BATTLE;
    }
    QJsonDocument doc(MainWindowData);
    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

    //这里确定了存储json文件的时候的文件路径，这里需要修改一下！
    QFile file(saveFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Couldn't open the file for writing.");
        return;
    }
    file.write(jsonData);
    file.close();
    qDebug() << saveFilePath;
}
void MainWindow::JsonToALL(){
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qWarning("Can not open this file!");
        return;
    }
    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    QJsonObject w = jsonDoc.object();
    MainWindow::MODE currentMode = static_cast<MainWindow::MODE>(w["currentMode"].toInt());
    WIDTH = w["WIDTH"].toInt();
    HEIGHT = w["HEIGHT"].toInt();
    MAP_SIZE = w["MAP_SIZE"].toInt();
    MAP_SIZEX = w["MAP_SIZEX"].toInt();
    MAP_SIZEY = w["MAP_SIZEY"].toInt();
    BLOCKS_SIZE = w["BLOCKS_SIZE"].toInt();
    num = w["num"].toInt();
    GAMETIME = w["GAMETIME"].toInt();
    //重载scoreboard1
    QJsonObject ScoreBoard1J = w["ScoreBoard1"].toObject();
    ScoreBoard1 = new ScoreBoard;//ScoreBoard1的board好像能用构造函数自己构造完。
    ScoreBoard1->move(w["scoreBoard1J"].toObject()["X"].toInt(),w["scoreBoard1J"].toObject()["Y"].toInt());
    qDebug() << w["ScoreBoard1J"].toObject()["X"].toInt();
    qDebug() << w["ScoreBoard1J"].toObject()["Y"].toInt();
    ScoreBoard1->total_Score = ScoreBoard1J["total_Score"].toInt();
    //重载exitButton；
    exitButton = new QToolButton();
    QJsonObject exitButtonJ = w["exitButton"].toObject();
    exitButton->setAutoRaise(exitButtonJ["Autoraise"].isBool());
    exitButton->setStyleSheet(exitButtonJ["StyleSheet"].toString());
    exitButton->setText(exitButtonJ["Text"].toString());
    exitButton->setFont(QFont(exitButtonJ["FontFamily"].toString(),exitButtonJ["FontSize"].toInt(),exitButtonJ["Font"].toBool()));
    exitButton->setFixedSize(exitButtonJ["Width"].toInt(),exitButtonJ["Height"].toInt());
    exitButton->move(exitButtonJ["X"].toInt(),exitButtonJ["Y"].toInt());
    //重载pauseButton
    pauseButton = new QToolButton();
    QJsonObject pauseButtonJ = w["pauseButton"].toObject();
    pauseButton->setAutoRaise(pauseButtonJ["Autoraise"].isBool());
    pauseButton->setStyleSheet(pauseButtonJ["StyleSheet"].toString());
    pauseButton->setText(pauseButtonJ["Text"].toString());
    pauseButton->setFont(QFont(pauseButtonJ["FontFamily"].toString(),pauseButtonJ["FontSize"].toInt(),pauseButtonJ["Font"].toBool()));
    pauseButton->setFixedSize(pauseButtonJ["Width"].toInt(),pauseButtonJ["Height"].toInt());
    pauseButton->move(pauseButtonJ["X"].toInt(),pauseButtonJ["Y"].toInt());
    //重载continueButton
    continueButton = new QToolButton();
    QJsonObject continueButtonJ = w["continueButton"].toObject();
    continueButton->setAutoRaise(continueButtonJ["Autoraise"].isBool());
    continueButton->setStyleSheet(continueButtonJ["StyleSheet"].toString());
    continueButton->setText(continueButtonJ["Text"].toString());
    continueButton->setFont(QFont(continueButtonJ["FontFamily"].toString(),continueButtonJ["FontSize"].toInt(),pauseButtonJ["Font"].toBool()));
    continueButton->setFixedSize(continueButtonJ["Width"].toInt(),continueButtonJ["Height"].toInt());
    continueButton->move(continueButtonJ["X"].toInt(),continueButtonJ["Y"].toInt());
    //重载map
    map = new Map();
    QJsonObject mapJ = w["map"].toObject();
    map->move(w["mapJ"].toObject()["X"].toInt(),w["mapJ"].toObject()["Y"].toInt());
    map->currentMode = static_cast<Map::Mode>(mapJ["currentMode"].toInt());
    map->grabKeyboard();
    map->setLock(mapJ["Lock"].toBool());
    map->setPaintFinalTrace1(mapJ["paintFianlTrace1"].isBool());
    map->setPaintFinalTrace2(mapJ["paintFianlTrace2"].isBool());
    //重载map的blocks
    QJsonArray blocksJ = w["map"].toObject()["blocks"].toArray();
    for(int i = 0;i<MAP_SIZEX;i++){
        for(int j = 0;j<MAP_SIZEY;j++){
            map->blocks[i][j]->anchorState1 = blocksJ[i][j]["anchorState1"].toInt();
            map->blocks[i][j]->anchorState2 = blocksJ[i][j]["anchorState2"].toInt();
            map->blocks[i][j]->coupled = blocksJ[i][j]["coupled"].toBool();
            map->blocks[i][j]->currentType = static_cast<Blocks::Type>(blocksJ[i][j]["currentType"].toInt());
            map->blocks[i][j]->deadState = blocksJ[i][j]["deadState"].toBool();
            map->blocks[i][j]->pixmap = map->jsonValueToPixmap(blocksJ[i][j]["pixmap"].toString());
            if(map->blocks[i][j]->imageLabel)
                map->blocks[i][j]->imageLabel->setPixmap(map->blocks[i][j]->pixmap);
            map->blocks[i][j]->setPosition(blocksJ[i][j]["X"].toInt(),blocksJ[i][j]["Y"].toInt());
        }
    }
    map->setClickPoint(mapJ["clickPointX"].toInt(),mapJ["clickPointY"].toInt());
    map->setBonus1(mapJ["bonus1"].toBool());
    map->setBonus2(mapJ["bonus2"].toBool());
    map->setBonus3(mapJ["bonus3"].toBool());
    map->setBonus4(mapJ["bonus4"].toBool());
    map->setRemainTime(mapJ["timerHRemain"].toInt());
    //重载map的deadBlock
    QJsonArray deadBlockJ = mapJ["deadBlock"].toArray();
    map->deadBlock = new std::vector<QPoint>();
    for(int i = 0; i < mapJ["deadBlockSize"].toInt(); i++) {
        QJsonObject obj = deadBlockJ[i].toObject();
        int x = obj["X"].toInt();
        int y = obj["Y"].toInt();
        map->deadBlock->push_back(QPoint(x, y));
    }
    map->setErase(mapJ["erase"].toBool());
    map->setGameOver(mapJ["gameOver"].toBool());
    //重载map的HERO1 HEOR2
    QJsonObject hero1J = mapJ["hero1"].toObject();
    map->hero1->anchorNum = hero1J["anchorNum"].toInt();
    map->hero1->scores = hero1J["scores"].toInt();
    map->hero1->currentStyle = static_cast<Player::Style>(hero1J["currentStyle"].toInt());
    for(int i = 0;i<hero1J["highlightBlockSize"].toInt();i++){
        map->hero1->highlightBlock.push_back(QPoint(hero1J["highlightBlock"].toArray()[i].toObject()["X"].toInt(),hero1J["highlightBlock"].toArray()[i].toObject()["Y"].toInt()));
    }
    map->hero1->highlightNum = hero1J["highlightNum"].toInt();
    map->hero1->highlightState = hero1J["highlightState"].toBool();
    map->hero1->pointA = QPoint(hero1J["pointAx"].toInt(),hero1J["pointAy"].toInt());
    map->hero1->pointB = QPoint(hero1J["pointBx"].toInt(),hero1J["pointBy"].toInt());
    map->hero1->pixmap = map->jsonValueToPixmap(hero1J["pixmap"].toString());
    map->hero1->imageLabel->setPixmap(map->hero1->pixmap);
    QJsonArray trc_LineJ = hero1J["trc_Line"].toArray();
    for(int i = 0;i<hero1J["trc_LineSize"].toInt();i++){
        map->hero1->trc_Line->push_back(QLine(QPoint(trc_LineJ[i].toObject()["p1X"].toInt(),trc_LineJ[i].toObject()["p1Y"].toInt()),QPoint(trc_LineJ[i].toObject()["p2X"].toInt(),trc_LineJ[i].toObject()["p2Y"].toInt())));
    }
    QJsonArray traceJ = hero1J["trace"].toArray();
    for(int i = 0;i<hero1J["traceSize"].toInt();i++){
        map->hero1->trace->push_back(QLine(QPoint(traceJ[i].toObject()["p1X"].toInt(),traceJ[i].toObject()["p1Y"].toInt()),QPoint(traceJ[i].toObject()["p2X"].toInt(),traceJ[i].toObject()["p2Y"].toInt())));
    }
    if(map->currentMode==Map::mode2){
        QJsonObject hero2J = mapJ["hero2"].toObject();
        map->hero2->anchorNum = hero2J["anchorNum"].toInt();
        map->hero2->scores = hero2J["scores"].toInt();
        map->hero2->currentStyle = static_cast<Player::Style>(hero2J["currentStyle"].toInt());
        for(int i = 0;i<hero2J["highlightBlockSize"].toInt();i++){
            map->hero2->highlightBlock.push_back(QPoint(hero2J["highlightBlock"].toArray()[i].toObject()["X"].toInt(),hero2J["highlightBlock"].toArray()[i].toObject()["Y"].toInt()));
        }
        map->hero2->highlightNum = hero2J["highlightNum"].toInt();
        map->hero2->highlightState = hero2J["highlightState"].toBool();
        map->hero2->pointA = QPoint(hero2J["pointAx"].toInt(),hero2J["pointAy"].toInt());
        map->hero2->pointB = QPoint(hero2J["pointBx"].toInt(),hero2J["pointBy"].toInt());
        map->hero2->pixmap = map->jsonValueToPixmap(hero2J["pixmap"].toString());
        map->hero2->imageLabel->setPixmap(map->hero2->pixmap);
        QJsonArray trc_LineJ2 = hero2J["trc_Line"].toArray();
        for(int i = 0;i<hero2J["trc_LineSize"].toInt();i++){
            map->hero2->trc_Line->push_back(QLine(QPoint(trc_LineJ2[i].toObject()["p1X"].toInt(),trc_LineJ2[i].toObject()["p1Y"].toInt()),QPoint(trc_LineJ2[i].toObject()["p2X"].toInt(),trc_LineJ2[i].toObject()["p2Y"].toInt())));
        }
        QJsonArray traceJ2 = hero2J["trace"].toArray();
        for(int i = 0;i<hero2J["traceSize"].toInt();i++){
            map->hero2->trace->push_back(QLine(QPoint(traceJ2[i].toObject()["p1X"].toInt(),traceJ2[i].toObject()["p1Y"].toInt()),QPoint(traceJ2[i].toObject()["p2X"].toInt(),traceJ2[i].toObject()["p2Y"].toInt())));
        }
    }
    map->setIsKeyPressed1(mapJ["isKeyPressed1"].toBool());
    map->setIsKeyPressed2(mapJ["isKeyPressed2"].toBool());
    map->setPaintMap(mapJ["paintmap"].toBool());
    map->setPaintTrace1(mapJ["painttrace1"].toBool());
    map->setPaintTrace2(mapJ["painttrace2"].toBool());
    map->setPressedKey1(mapJ["pressedKey1"].toInt());
    map->setPressedKey2(mapJ["pressedKey2"].toInt());
    map->setTarget1(mapJ["target1X"].toInt(),mapJ["target1Y"].toInt());
    map->setTarget2(mapJ["target2X"].toInt(),mapJ["target2Y"].toInt());
    map->setTarget3(mapJ["target3X"].toInt(),mapJ["target3Y"].toInt());
    map->setTarget4(mapJ["target4X"].toInt(),mapJ["target4Y"].toInt());
    if(mapJ["timer1"].toObject()["isactive"].toBool()){
        map->timer1 = new QTimer(map);
        map->timer1->setSingleShot(true);
    }
    if(mapJ["timer2"].toObject()["isactive"].toBool()){
        map->timer2 = new QTimer(map);
        map->timer2->setSingleShot(true);
    }
    if(mapJ["timer3"].toObject()["isactive"].toBool()){
        map->timer3 = new QTimer(map);
        map->timer3->setSingleShot(true);
        map->timer3->start(1000);
        connect(map->timer3, &QTimer::timeout,map,&Map::reload);
    }
    map->setType1Count(mapJ["type1Count"].toInt());
    map->setType2Count(mapJ["type2Count"].toInt());
    map->setType3Count(mapJ["type3Count"].toInt());
    //重载TimeCounter
    TimeCounter = new Timer;
    QJsonObject TimeCounterJ = w["TimeCounter"].toObject();
    TimeCounter->move(w["TimeCounterJ"].toObject()["X"].toInt(),w["TimeCounterJ"].toObject()["Y"].toInt());
    TimeCounter->timeLine->resize(TimeCounterJ["timeLine"].toObject()["width"].toInt(),TimeCounterJ["timeLine"].toObject()["height"].toInt());
    TimeCounter->timeLine->setStyleSheet(TimeCounterJ["timeLine"].toObject()["stylesheet"].toString());
    TimeCounter->timeLine->setText(TimeCounterJ["timeLine"].toObject()["Text"].toString());
    TimeCounter->setRemainTime(TimeCounterJ["remainTime"].toInt());
    TimeCounter->setShowHint(TimeCounterJ["showHint"].toBool());
    qDebug()<<TimeCounterJ["showHint"].toBool();
    if(currentMode==MODE1){
        SOLO = new QGraphicsScene(this);
        SOLO->setSceneRect(0, 0, 1500, 800);
        SOLO->addWidget(exitButton);
        SOLO->addWidget(pauseButton);
        SOLO->addWidget(continueButton);
        SOLO->addWidget(TimeCounter);
        SOLO->addWidget(ScoreBoard1);
        SOLO->addWidget(map);
        setScene(SOLO);
    }
    if(currentMode==MODE2){
        //存scoreboard2,battle scene
        QJsonObject ScoreBoard2J = w["ScoreBoard2"].toObject();
        ScoreBoard2 = new ScoreBoard;//ScoreBoard1的board好像能用构造函数自己构造完。
        ScoreBoard2->move(w["scoreBoard2J"].toObject()["X"].toInt(),w["scoreBoard2J"].toObject()["Y"].toInt());
        ScoreBoard2->total_Score = ScoreBoard2J["total_Score"].toInt();
        BATTLE = new QGraphicsScene(this);
        BATTLE->setSceneRect(0, 0, 1500, 800);
        connect(map->hero2,&Player::match,ScoreBoard2,&ScoreBoard::addScore);
        connect(TimeCounter,&Timer::timeOut,ScoreBoard2,&ScoreBoard::gameOver);
        connect(ScoreBoard2,&ScoreBoard::finalScore,map->hero2,&Player::getScore);
        BATTLE->addWidget(exitButton);
        BATTLE->addWidget(pauseButton);
        BATTLE->addWidget(continueButton);
        BATTLE->addWidget(TimeCounter);
        BATTLE->addWidget(ScoreBoard1);
        BATTLE->addWidget(ScoreBoard2);
        BATTLE->addWidget(map);
        setScene(BATTLE);
    }
    //connects affiliated to exitButton
    connect(exitButton, &QToolButton::clicked, this,[=](){
        map->releaseKeyboard();
        map->releaseMouse();
        this->setScene(ABOUT);
    });
    //connects affiliated to TimeCounter
    connect(TimeCounter,&Timer::timeOut,map,&Map::freeze);
    connect(TimeCounter,&Timer::timeOut,map,&Map::getGains);
    connect(TimeCounter,&Timer::bonus,map,&Map::bonus);
    //connects affiliated to map
    connect(map,&Map::gameFinished,map,&Map::getGains);//应该会有多方渠道发出这个信号，目的是让map知道应该展示gains了
    connect(map,&Map::addtime,TimeCounter,&Timer::addTime);
    connect(pauseButton, &QToolButton::clicked, TimeCounter, &Timer::stop);
    connect(pauseButton,&QToolButton::clicked,this,&MainWindow::showPopUpWindow);
    connect(pauseButton, &QToolButton::clicked, map, [=](){
        map->timer3->stop();
    });
    connect(TimeCounter,&Timer::freeze,map,&Map::freeze);
    connect(continueButton, &QToolButton::clicked, TimeCounter, &Timer::Continue);
    connect(TimeCounter,&Timer::melt,map,&Map::melt);
    connect(map->hero1,&Player::match,ScoreBoard1,&ScoreBoard::addScore);
    connect(map,&Map::gameFinished,ScoreBoard1,&ScoreBoard::gameOver);
    connect(map,&Map::gameFinished,map,&Map::freeze);
    connect(TimeCounter,&Timer::timeOut,ScoreBoard1,&ScoreBoard::gameOver);
    connect(map,&Map::gameFinished, TimeCounter, &Timer::stop);
    connect(ScoreBoard1,&ScoreBoard::finalScore,map->hero1,&Player::getScore);
    connect(map,&Map::erase_Couple,map,&Map::hint);
    connect(map,&Map::markHint,TimeCounter,&Timer::SetHint);
    load = true;
    update();
    TimeCounter->update();
    ScoreBoard1->update();
    if(currentMode==MODE2){
        ScoreBoard2->update();
    }
    map->update();
}

void MainWindow::TO_HOME()
{
    setWindowTitle("连连看");
    setRenderHint(QPainter::Antialiasing);
    HOME = new QGraphicsScene(this);
    startButton = new QToolButton();
    startButton->setAutoRaise(true);
    startButton->setStyleSheet("background-color:orange");
    startButton->setText("START GAME!");
    startButton->setFont(QFont("Arial", 30));
    startButton->setFixedSize(300,100);
    startButton->move(600, 355);
    HOME->addWidget(startButton);
    HOME->setSceneRect(0, 0, 1500, 800);
    connect(startButton, &QToolButton::clicked, this, &MainWindow::HOME_LOGIN);
    QGraphicsTextItem *textItem_h = HOME->addText("WELCOME!", QFont("Arial", 60));
    textItem_h->setPos(550, 230);
    setScene(HOME);
}
void MainWindow::HOME_LOGIN()
{
    LOG_IN = new QGraphicsScene(this);
    user = new QLineEdit();
    password = new QLineEdit();
    user->move(600,300);
    password->move(600,400);
    user->setPlaceholderText("username...");
    user->setClearButtonEnabled(true);
    user->setFont(QFont("Arial",20));
    password->setPlaceholderText("password...");
    password->setEchoMode(QLineEdit::Password);
    password->setFont(QFont("Arial",20));
    LOG_IN->addWidget(user);
    LOG_IN->addWidget(password);
    logInButton = new QToolButton();
    logInButton->setAutoRaise(true);
    logInButton->setStyleSheet("background-color:orange");
    logInButton->setText("LOG IN");
    logInButton->setFixedSize(180,40);
    logInButton->setFont(QFont("Arial",20));
    logInButton->move(650,500);
    LOG_IN->addWidget(logInButton);
    LOG_IN->setSceneRect(0,0,1500,1000);
    connect(logInButton, &QToolButton::clicked, this, &MainWindow::TO_ABOUT);
    setScene(LOG_IN);
}
void MainWindow::TO_ABOUT()
{
    qDebug()<<"to about";
    ABOUT = new QGraphicsScene(this);
    ABOUT->setSceneRect(0, 0, 1500, 800);
    quitGameButton = new QToolButton();
    qDebug()<<"1";
    loadGameButton = new QToolButton();
    qDebug()<<"2";
    startGameButton = new QToolButton();
    qDebug()<<"3";
    quitGameButton->setAutoRaise(true);
    quitGameButton->setStyleSheet("background-color:orange");
    quitGameButton->setText("QUIT GAME");
    quitGameButton->setFont(QFont("Arial", 40,QFont::Bold));
    quitGameButton->setFixedSize(500,100);
    quitGameButton->move(500,250);
    loadGameButton->setAutoRaise(true);
    loadGameButton->setStyleSheet("background-color:orange");
    loadGameButton->setText("LOAD GAME");
    qDebug()<<"5";
    loadGameButton->setFont(QFont("Arial", 40,QFont::Bold));
    loadGameButton->setFixedSize(500,100);
    loadGameButton->move(500,360);
    startGameButton->setAutoRaise(true);
    startGameButton->setStyleSheet("background-color:orange");
    startGameButton->setText("NEW GAME");
    startGameButton->setFont(QFont("Arial", 40,QFont::Bold));
    startGameButton->setFixedSize(500,100);
    startGameButton->move(500,470);
    connect(quitGameButton,&QToolButton::clicked,this,[=](){
        if(HOME)
            this->setScene(HOME);
        else
            TO_HOME();
    });
    connect(loadGameButton,&QToolButton::clicked,this,&MainWindow::loadGameWindow);
    connect(startGameButton,&QToolButton::clicked,this,[=](){
        if(MENU)
            this->setScene(MENU);
        else
            LOGIN_MENU();
    });
    ABOUT->addWidget(quitGameButton);
    ABOUT->addWidget(loadGameButton);
    ABOUT->addWidget(startGameButton);
    setScene(ABOUT);
}
void MainWindow::LOGIN_MENU()
{
    MENU = new QGraphicsScene();
    QWidget *widget = new QWidget;
    QVBoxLayout *myLayout = new QVBoxLayout(widget);
    QLabel *text_1  = new QLabel(tr("SET CONFIGUE!"));
    QLabel *text_2  = new QLabel(tr("CHOOSE MODE!"));
    text_1->setFont(QFont("Arial",40,QFont::Bold));
    text_2->setFont(QFont("Arial",40,QFont::Bold));
    QLineEdit *LineEdit = new QLineEdit();
    LineEdit->move(625,230);
    LineEdit->setStyleSheet("background-color: lightblue;");
    LineEdit->setPlaceholderText("map's size M*N...");
    LineEdit->setClearButtonEnabled(true);
    LineEdit->setFont(QFont("Arial",20));
    LineEdit->setEnabled(true);
    LineEdit->setFocusPolicy(Qt::StrongFocus);
    QToolButton* ensureButton = new QToolButton();
    ensureButton->setAutoRaise(true);
    ensureButton->setStyleSheet("background-color:lightblue");
    ensureButton->setText("CONFIGUE");
    ensureButton->setFont(QFont("Arial", 20));
    soloButton = new QToolButton();
    battleButton = new QToolButton();
    soloButton->setAutoRaise(true);
    soloButton->setStyleSheet("background-color:orange");
    soloButton->setText("SOLO");
    soloButton->setFont(QFont("Arial", 40));
    soloButton->setFixedSize(250,100);
    battleButton->setAutoRaise(true);
    battleButton->setStyleSheet("background-color:orange");
    battleButton->setText("BATTLE");
    battleButton->setFont(QFont("Arial", 40));
    battleButton->setFixedSize(250,100);
    myLayout->addWidget(text_1);
    myLayout->addWidget(LineEdit);
    myLayout->addWidget(ensureButton);
    myLayout->addWidget(text_2);
    myLayout->addWidget(soloButton);
    myLayout->addWidget(battleButton);
    myLayout->setSpacing(15);
    connect(ensureButton, &QToolButton::clicked, this,[=](){
        QString mapSize = LineEdit->text();
        if(!(mapSize==" ")){
            qDebug()<<mapSize;
            int pos = mapSize.indexOf('*');
            int tmpx = mapSize.mid(0,pos).toInt()+2;
            int tmpy = mapSize.mid(pos+1).toInt()+2;
            if(!(tmpy<=3&&tmpx<=3))
            {
                connect(soloButton, &QToolButton::clicked, this, [=](){
                    if(SOLO)
                        {
                        qDebug()<<"1";
                        delete map;
                        delete TimeCounter;
                        delete ScoreBoard1;
                        delete exitButton;
                        qDebug()<<"2";
                        delete pauseButton;
                        delete continueButton;
                        qDebug()<<"3";
                        delete SOLO;
                        qDebug()<<"4";
                    }
                    MAP_SIZEX = mapSize.mid(0,pos).toInt()+2;
                    MAP_SIZEY = mapSize.mid(pos+1).toInt()+2;
                    //qDebug()<<MAP_SIZEX<<"  "<<MAP_SIZEY;
                    MENU_SOLO();
                });
                connect(battleButton, &QToolButton::clicked, this, [=](){
                    if(BATTLE)
                    {
                        delete map;
                        delete TimeCounter;
                        delete ScoreBoard1;
                        delete ScoreBoard2;
                        delete exitButton;
                        delete pauseButton;
                        delete continueButton;
                        delete BATTLE;
                    }
                    MAP_SIZEX = mapSize.mid(0,pos).toInt()+2;
                    MAP_SIZEY = mapSize.mid(pos+1).toInt()+2;
                    //qDebug()<<MAP_SIZEX<<"  "<<MAP_SIZEY;
                    MENU_BATTLE();
                });
            }
}
    });
    myLayout->setAlignment(soloButton, Qt::AlignCenter);
    myLayout->setAlignment(battleButton, Qt::AlignCenter);
    myLayout->setAlignment(ensureButton, Qt::AlignCenter);
    widget->move(540,160);
    widget->setStyleSheet("background-color: lightblue;");
    MENU->addWidget(widget);
    MENU->setSceneRect(0,0,1500,1000);
    setScene(MENU);
}
void MainWindow::MENU_SOLO()
{
    map = new Map(); //tips:在这里面自己重新自定义一个map可以让每次返回到新的solo界面的时候仍然能画出map。
    if(MAP_SIZEX<4)
    {
        bool valid = map->testify_Game();
        while(!valid)
        {
            delete map;
            map = new Map();
            valid = map->testify_Game();
            qDebug()<<"valid"<<valid;
        }
    }
    qDebug()<<"here";
    currentMode = MODE1;  // MODE1
    SOLO = new QGraphicsScene(this);
    SOLO->setSceneRect(0, 0, 1500, 800);
    exitButton = new QToolButton();
    SOLO->addWidget(exitButton);
    exitButton->setAutoRaise(true);
    exitButton->setStyleSheet("background-color:orange");
    exitButton->setText("EXIT");
    exitButton->setFont(QFont("Arial", 40,QFont::Bold));
    exitButton->setFixedSize(270,100);
    exitButton->move(1100,90);
    pauseButton = new QToolButton();
    SOLO->addWidget(pauseButton);
    pauseButton->setAutoRaise(true);
    pauseButton->setStyleSheet("background-color:orange");
    pauseButton->setText("PAUSE GAME");
    pauseButton->setFont(QFont("Arial", 20,QFont::Bold));
    pauseButton->setFixedSize(270,100);
    pauseButton->move(1100,200);
    continueButton = new QToolButton();
    SOLO->addWidget(continueButton);
    continueButton->setAutoRaise(true);
    continueButton->setStyleSheet("background-color:orange");
    continueButton->setText("CONTINUE GAME");
    continueButton->setFont(QFont("Arial", 20,QFont::Bold));
    continueButton->setFixedSize(270,100);
    continueButton->move(1100,310);
    SOLO->addWidget(map); //这个还是需要自己在fromJson函数里面实现的
    map->move((width()-WIDTH)/2,(height()-HEIGHT)/2);
    map->currentMode = Map::mode1;
    map->grabKeyboard();
    //tips:
    //在特定情况下，如将小部件添加到 QGraphicsScene 中时，将父级设置为 nullptr 是很常见的做法，因为在这种情况下，QGraphicsScene 会管理这个小部件，而不需要由其他 QWidget 来管理
    //反而如果加了（this）就会让事情变化
    //为什么要传递 nullptr？
    // 避免独立窗口的行为:
    // 当我将父级设置为 nullptr 时，这个 Timer 对象不会自动成为一个独立窗口，而是一个孤立的小部件，
    // 等待被其他容器（如 QGraphicsScene 或 QLayout）管理。如果你传递了一个其他 QWidget 作为父级，这个 Timer 小部件将会嵌套在那个父级小部件内，这通常会影响它在 QGraphicsScene 中的显示效果，或者导致它显示在父小部件内，而不是在 QGraphicsScene 中。
    // 让 QGraphicsScene 管理:
    // 当我将这个 timerWidget 对象添加到 QGraphicsScene 时，QGraphicsScene 会为其创建一个 QGraphicsProxyWidget，并将其显示在场景中。由于 timerWidget 没有父级，QGraphicsScene 可以完全控制它的显示，而不受其他父小部件的影响。
    TimeCounter = new Timer;
    SOLO->addWidget(TimeCounter);
    TimeCounter->move(100,100);
    ScoreBoard1 = new ScoreBoard;
    SOLO->addWidget(ScoreBoard1);
    ScoreBoard1->move(100,500);
    setScene(SOLO);
    connect(TimeCounter,&Timer::timeOut,map,&Map::freeze);
    connect(TimeCounter,&Timer::timeOut,map,&Map::getGains);
    connect(map,&Map::gameFinished,map,&Map::getGains);//应该会有多方渠道发出这个信号，目的是让map知道应该展示gains了
    connect(TimeCounter,&Timer::bonus,map,&Map::bonus);
    connect(map,&Map::addtime,TimeCounter,&Timer::addTime);
    connect(pauseButton, &QToolButton::clicked, TimeCounter, &Timer::stop);
    connect(pauseButton,&QToolButton::clicked,this,&MainWindow::showPopUpWindow);
    connect(pauseButton, &QToolButton::clicked, map, [=](){
        map->timer3->stop();
    });
    connect(TimeCounter,&Timer::freeze,map,&Map::freeze);
    connect(continueButton, &QToolButton::clicked, TimeCounter, &Timer::Continue);
    connect(TimeCounter,&Timer::melt,map,&Map::melt);
    connect(map->hero1,&Player::match,ScoreBoard1,&ScoreBoard::addScore);
    connect(map,&Map::gameFinished,ScoreBoard1,&ScoreBoard::gameOver);
    connect(map,&Map::gameFinished,map,&Map::freeze);
    connect(TimeCounter,&Timer::timeOut,ScoreBoard1,&ScoreBoard::gameOver);
    connect(map,&Map::gameFinished, TimeCounter, &Timer::stop);
    connect(ScoreBoard1,&ScoreBoard::finalScore,map->hero1,&Player::getScore);
    connect(map,&Map::erase_Couple,map,&Map::hint);
    connect(exitButton, &QToolButton::clicked, this,[=](){
        map->releaseKeyboard();
        map->releaseMouse();
        this->setScene(ABOUT);
    });
    connect(map,&Map::markHint,TimeCounter,&Timer::SetHint);
}
void MainWindow::MENU_BATTLE()
{
    map = new Map;
    if(MAP_SIZEX<4)
    {
        bool valid = map->testify_Game();
        while(!valid)
        {
            delete map;
            map = new Map();
            valid = map->testify_Game();
            qDebug()<<"valid"<<valid;
        }
    }
    currentMode = MODE2;
    BATTLE = new QGraphicsScene(this);
    map->currentMode = Map::mode2;
    map->hero1->setStyle(Player::style1);
    map->hero1->setStyle(Player::style2);
    BATTLE->setSceneRect(((width()-WIDTH)/2),((height()-HEIGHT)/2), WIDTH,HEIGHT);
    map->move((width()-WIDTH)/2,(height()-HEIGHT)/2);
    exitButton = new QToolButton();
    exitButton->setAutoRaise(true);
    exitButton->setStyleSheet("background-color:orange");
    exitButton->setText("EXIT");
    exitButton->setFont(QFont("Arial", 40,QFont::Bold));
    exitButton->setFixedSize(270,100);
    exitButton->move(1100,90);
    BATTLE->addWidget(exitButton);
    pauseButton = new QToolButton();
    pauseButton->setAutoRaise(true);
    pauseButton->setStyleSheet("background-color:orange");
    pauseButton->setText("PAUSE GAME");
    pauseButton->setFont(QFont("Arial", 20,QFont::Bold));
    pauseButton->setFixedSize(270,100);
    pauseButton->move(1100,200);
    BATTLE->addWidget(pauseButton);
    continueButton = new QToolButton();
    continueButton->setAutoRaise(true);
    continueButton->setStyleSheet("background-color:orange");
    continueButton->setText("CONTINUE GAME");
    continueButton->setFont(QFont("Arial", 20,QFont::Bold));
    continueButton->setFixedSize(270,100);
    continueButton->move(1100,310);
    BATTLE->addWidget(continueButton);
    BATTLE->addWidget(map);
    TimeCounter = new Timer;
    BATTLE->addWidget(TimeCounter);
    TimeCounter->move(100,100);
    connect(TimeCounter,&Timer::timeOut,map,&Map::freeze);
    connect(TimeCounter,&Timer::bonus,map,&Map::bonus);
    connect(map,&Map::addtime,TimeCounter,&Timer::addTime);
    connect(TimeCounter,&Timer::timeOut,map,&Map::getGains);
    connect(map,&Map::gameFinished,map,&Map::getGains);//应该会有多方渠道发出这个信号，目的是让map知道应该展示gains了
    connect(exitButton, &QToolButton::clicked, this,[=](){
        map->releaseKeyboard();
        map->releaseMouse();
        this->setScene(ABOUT);
    });
    connect(TimeCounter,&Timer::bonus,map,&Map::bonus);
    connect(map,&Map::addtime,TimeCounter,&Timer::addTime);
    connect(continueButton, &QToolButton::clicked, TimeCounter, &Timer::Continue);
    connect(TimeCounter,&Timer::melt,map,&Map::melt);
    map->grabKeyboard();
    connect(pauseButton, &QToolButton::clicked, TimeCounter, &Timer::stop);
    connect(pauseButton,&QToolButton::clicked,this,&MainWindow::showPopUpWindow);
    connect(pauseButton, &QToolButton::clicked, map, [=](){
        map->timer3->stop();
    });
    connect(TimeCounter,&Timer::freeze,map,&Map::freeze);
    ScoreBoard1 = new ScoreBoard;
    BATTLE->addWidget(ScoreBoard1);
    ScoreBoard1->move(100,600);
    connect(map->hero1,&Player::match,ScoreBoard1,&ScoreBoard::addScore);
    connect(ScoreBoard1,&ScoreBoard::finalScore,map->hero1,&Player::getScore);
    connect(TimeCounter,&Timer::timeOut,ScoreBoard1,&ScoreBoard::gameOver);
    ScoreBoard2 = new ScoreBoard;
    BATTLE->addWidget(ScoreBoard2);
    ScoreBoard2->move(1300,600);
    ScoreBoard2->board->setStyleSheet("color:red;background-color:lightgreen");
    connect(map->hero2,&Player::match,ScoreBoard2,&ScoreBoard::addScore);
    connect(TimeCounter,&Timer::timeOut,ScoreBoard2,&ScoreBoard::gameOver);
    connect(ScoreBoard2,&ScoreBoard::finalScore,map->hero2,&Player::getScore);
    connect(map,&Map::markHint,TimeCounter,&Timer::SetHint);
    setScene(BATTLE);
}
void MainWindow::paintGains(QPainter &painter)
{

    //qDebug() <<"paint gains";
    QColor orange(255, 165, 0);
    QString text1 = QString("YOUR SCORE:\n %1").arg(map->hero1->scores);
    QString text2 = QString("HERO1 SCORE:\n %1\n HERO2 SCORE:\n %2\n WINNER:\n %3").arg(map->hero1->scores).arg(map->hero2->scores).arg(map->hero1->scores>map->hero2->scores?map->hero1->scores:map->hero2->scores);
    QString text3 = QString("LOCK!!!");
    painter.setPen(QPen(Qt::NoPen));
    painter.setBrush(QBrush(orange));
    painter.setFont(QFont("Arial",50,QFont::Bold));
    int side = qMin(width(), height());  // 获取窗口的最小边长
    painter.translate((width()-side) / 2.0, (height()-side) / 2.0);
    switch(currentMode){
    case MODE1:
        painter.setPen(QPen(Qt::green));
        painter.drawText(QRectF(64,64,MAP_SIZEX*50,MAP_SIZEY*30),Qt::AlignHCenter | Qt::TextWordWrap,text1);
        break;
    case MODE2:
        painter.setPen(QPen(Qt::green));
        painter.drawText(QRectF(64,64,MAP_SIZEX*50,MAP_SIZEY*50),Qt::AlignHCenter | Qt::TextWordWrap,text2);
    }
    if(map->isLock()){
        painter.setPen(QPen(Qt::red));
        painter.setFont(QFont("Arial",60,QFont::Bold));
        painter.drawText(QRectF(150,100,MAP_SIZEX*50,MAP_SIZEY*50),Qt::AlignHCenter | Qt::TextWordWrap,text3);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
}

void MainWindow::loadGameWindow()
{
    this->open();
    this->JsonToALL();
    if(load)
    {
        load = false;
    }
}
void MainWindow::showPopUpWindow() {
    // 创建一个新的窗口
    QWidget *popupWindow = new QWidget();
    popupWindow->setWindowTitle("save & load");
    // 创建按钮和文本输入框
    QPushButton *popupButton1 = new QPushButton("SAVE GAME", popupWindow);
    QPushButton *popupButton2 = new QPushButton("LOAD GAME", popupWindow);
    popupButton1->setFont(QFont("Arial",20,QFont::Bold));
    popupButton2->setFont(QFont("Arial",20,QFont::Bold));
    // 布局
    QVBoxLayout *layout = new QVBoxLayout(popupWindow);
    layout->addWidget(popupButton1);
    layout->addWidget(popupButton2);
    //按下了save game
    connect(popupButton1,&QPushButton::clicked,this,&MainWindow::save);
    connect(popupButton1,&QPushButton::clicked,map->hero1,&Player::toJson);
    if(currentMode==MODE2)
    {
        connect(popupButton1,&QPushButton::clicked,map->hero2,&Player::toJson);
    }
    connect(popupButton1,&QPushButton::clicked,TimeCounter,&Timer::toJson);
    for(int i = 0;i<MAP_SIZEX;i++)
    {    for(int j = 0;j<MAP_SIZEY;j++)
        {
            connect(popupButton1,&QPushButton::clicked,map->blocks[i][j],&Blocks::toJson);
        }
    }
    connect(popupButton1,&QPushButton::clicked,ScoreBoard1,&ScoreBoard::toJson);
    if(currentMode==MODE2)
        connect(popupButton1,&QPushButton::clicked,ScoreBoard2,&ScoreBoard::toJson);
    connect(popupButton1,&QPushButton::clicked,map,&Map::toJson);
    connect(popupButton1,&QPushButton::clicked,this,&MainWindow::toJson);
    //按下了load game
    connect(popupButton2,&QPushButton::clicked,this,&MainWindow::open);
    connect(popupButton2,&QPushButton::clicked,this,[=](){
        this->JsonToALL();
        if(load)
        {
            load = false;
            delete popupWindow;
        }
        qDebug()<<filePath;
    });
    // 显示窗口
    popupWindow->setLayout(layout);
    popupWindow->setStyleSheet("background-color: lightblue;");
    popupWindow->resize(400, 200);
    popupWindow->show();
}

void MainWindow::open()
{
    filePath = QFileDialog::getOpenFileName(this, "Open");
}
void MainWindow::save()
{
    saveFilePath = QFileDialog::getSaveFileName(this, "Save");
}

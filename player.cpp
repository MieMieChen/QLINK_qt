#include "player.h"
#include <QWidget>
#include "constNum.h"
#include <random>
#include <QRandomGenerator>
#include <QPainter>
#include <QPainter>
static int times = 0;
Player::Player(QWidget *parent):QWidget(parent) {
    // qDebug () << basePath;
    this->setStyle(this->random());
    trc_Line = new std::vector<QLine>;
    trace = new QQueue<QLine>;
}
QJsonObject Player::getJsonData() const
{
    qDebug() << "PlayerData:" << PlayerData;
    return PlayerData;
}
QJsonValue Player::pixmapToJsonValue(const QPixmap &pixmap)const {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");  // 保存 pixmap 数据到缓冲区，使用 PNG 格式
    QString base64String = QString::fromLatin1(byteArray.toBase64().data());
    return QJsonValue(base64String);
}
QPixmap Player::jsonValueToPixmap(const QJsonValue &value) const{
    QByteArray byteArray = QByteArray::fromBase64(value.toString().toLatin1());
    QPixmap pixmap;
    pixmap.loadFromData(byteArray, "PNG");
    return pixmap;
}
void Player::toJson() const
{
    qDebug()<<"player json";
    QJsonArray trc_LineJ,highlightBlockJ,traceJ;
    PlayerData["pixmap"] = pixmapToJsonValue(pixmap);
    PlayerData["pointAx"] = pointA.x();
    PlayerData["pointAy"] = pointA.y();
    PlayerData["pointBx"] = pointB.x();
    PlayerData["pointBy"] = pointB.y();
    PlayerData["currentStyle"] = currentStyle;
    PlayerData["highlightNum"] = highlightNum;
    PlayerData["highlightState"] = highlightState;
    PlayerData["scores"] = scores;
    PlayerData["anchorNum"] = anchorNum;
    for (std::vector<QLine>::const_iterator it = trc_Line->begin();it!=trc_Line->end();it++){
        QJsonObject tmp;
        tmp["p1X"] = it->p1().x();
        tmp["p1Y"] = it->p1().y();
        tmp["p2X"] = it->p2().x();
        tmp["p2Y"] = it->p2().y();
        trc_LineJ.append(tmp);
    }
    for(std::vector<QPoint>::const_iterator it = highlightBlock.begin();it!=highlightBlock.end();it++){
        QJsonObject tmp;
        tmp["X"] = it->x();
        tmp["Y"] = it->y();
        highlightBlockJ.append(tmp);
    }
    while(!trace->empty()){
        QJsonObject tmp;
        tmp["p1X"] = trace->front().p1().x();
        tmp["p1Y"] = trace->front().p1().y();
        tmp["p2X"] = trace->front().p2().x();
        tmp["p2Y"] = trace->front().p2().y();
        traceJ.append(tmp);
    }
    PlayerData["trc_Line"] = trc_LineJ;
    PlayerData["trc_LineSize"] = static_cast<int>(trc_Line->size());
    qDebug() << "trc_LineJ content:" << QJsonDocument( trc_LineJ).toJson(QJsonDocument::Indented);
    PlayerData["highlightBlock"] = highlightBlockJ;
    PlayerData["highlightBlockSize"] = static_cast<int>(highlightBlock.size());
    qDebug() << "highlightBlock:" << QJsonDocument(highlightBlockJ).toJson(QJsonDocument::Indented);
    PlayerData["trace"] = traceJ;
    PlayerData["trace_Size"] = static_cast<int>(trace->size());
}
Player::Style Player::random()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    // 生成1到2之间的均匀分布的整数
    std::uniform_int_distribution<> dis(1, 3);
    if(dis(gen)==1)
        return Player::style1;
    if(dis(gen)==2)
        return Player::style2;
}
void Player::getScore(int value)
{
    scores = value;
    //qDebug() <<value;
}
void Player::setStyle(Player::Style style)
{
    currentStyle = style;
    int scaledWidth = num;
    int scaledHeight = num;
    int originalWidth;
    int originalHeight;
    int cropSize;
    imageLabel = new QLabel(this);
    switch(currentStyle){
    case style1:
        pixmap = *pixmap1; // 确保 pixmap1 已经被正确初始化
        if (!pixmap.isNull()) { // 检查 pixmap 是否有效
            originalWidth = pixmap.width();
            originalHeight = pixmap.height();
            cropSize = qMin(originalWidth, originalHeight);
            QRect cropRect1((originalWidth - cropSize) / 2, (originalHeight - cropSize) / 2, cropSize, cropSize);
            QPixmap croppedImage = pixmap.copy(cropRect1);
            QPixmap resizedImage1 = croppedImage.scaled(scaledWidth, scaledHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            if (!resizedImage1.isNull()) { // 确保 scaledPixmap 有效
                pixmap = resizedImage1;
                if (imageLabel != nullptr) { // 检查 imageLabel 是否为有效指针
                    imageLabel->setPixmap(pixmap); // 设置 pixmap 到 imageLabel
                } else {
                    qDebug() << "imageLabel is null";
                }
            } else {
                qDebug() << "Failed to scale pixmap";
            }
        } else {
            qDebug() << "pixmap1 is null";
        }
        break;
    case style2:
        pixmap = *pixmap2; // 确保 pixmap2 已经被正确初始化
        if (!pixmap.isNull()) { // 检查 pixmap 是否有效
            originalWidth = pixmap.width();
            originalHeight = pixmap.height();
            cropSize = qMin(originalWidth, originalHeight);
            QRect cropRect2((originalWidth - cropSize) / 2, (originalHeight - cropSize) / 2, cropSize, cropSize);
            QPixmap croppedImage = pixmap.copy(cropRect2);
            QPixmap resizedImage2 = croppedImage.scaled(scaledWidth, scaledHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            if (!resizedImage2.isNull()) { // 确保 scaledPixmap 有效
                pixmap = resizedImage2;
                if (imageLabel != nullptr) { // 检查 imageLabel 是否为有效指针
                    imageLabel->setPixmap(pixmap); // 设置 pixmap 到 imageLabel
                } else {
                    qDebug() << "imageLabel is null";
                }
            } else {
                qDebug() << "Failed to scale pixmap";
            }
        } else {
            qDebug() << "pixmap1 is null";
        }
        break;
    }
    this->randomPosition();
}
void Player::paintPlayer(QPainter & painter)
{
    QPoint position(pointB.x(),pointB.y());
    qDebug()<< "paint player" ;
    qDebug()<<pointB.x();
    qDebug()<<pointB.y();
    // painter.fillRect(rect(), Qt::yellow);
    painter.drawPixmap(position,pixmap);
}
void Player::paintEvent(QPaintEvent *event){
    times++;
    QPainter painter(this);
    paintPlayer(painter);
}
void Player::pMove(int dir)
{
    // qDebug() << "i am here !" << std::endl;
    QPoint begin = this->pointB;//出发的地方
    pointA = pointB;//把ponita 记作为上一个位置
    switch (dir){
    case 0://left
        //qDebug()<< "now i am going to move left !";
        emit testifyCrash();
        //应该是返回一个bool 如果是true 可以移动再做后面的操作，如果不可以移动就直接false，然后break了。然后发出一个类似于warning的信号给到用户
        if(!(begin.x()-num<0))
            pointB.rx() = begin.x()-num;
        pointB.ry() = begin.y();
        this->move(pointB.x(),pointB.y());
        //qDebug()<< "now i finish move left!";
        break;
    case 1://right
        //qDebug()<< "now i am going to move right !";
        if(!(begin.x()+num>=WIDTH))
            pointB.rx() = begin.x()+num;
        pointB.ry() = begin.y();
        this->move(pointB.x(),pointB.y());
        //qDebug()<< "now i finish move right !";
        break;
    case 2://down
        //qDebug()<< "now i am going to move up !";
        pointB.rx() = begin.x();
        if(!(begin.y()+num >=HEIGHT))
            pointB.ry() = begin.y()+num;
        this->move(pointB.x(),pointB.y());
        //qDebug()<< "now i finish move up!";
        break;
    case 3://up
        //qDebug()<< "now i am going to move down !";
        pointB.rx() = begin.x();
        if(!(begin.y()-num<0))
            pointB.ry() = begin.y()-num;
        this->move(pointB.x(),pointB.y());
        //sqDebug()<< "now i finish move down!";
        break;
    }
    QPoint a,b;
    QLine line1;
    a.rx() = pointA.x()+num/2;
    a.ry() = pointA.y()+num/2;
    b.rx() = pointB.x()+num/2;
    b.ry() = pointB.y()+num/2;
    line1 = QLine(a,b);
    if(highlightState)
        trc_Line->push_back(line1);
    qDebug()<<pointB.rx()<<pointB.ry();
}

void Player::setPosition(int x, int y)
{
    // imageLabel->setGeometry(x, y, pixmap.width(), pixmap.height());
    pointB.rx() = x;
    pointB.ry() = y;
    update();
}
void Player::randomPosition()
{
    const int step = 64;
    std::vector<QPoint> points;
    for(int i = 0;i<MAP_SIZEY;i++){
        points.emplace_back(0,step*i);
    }
    for(int i = 1;i<MAP_SIZEX;i++){
        points.emplace_back(i*step,0);
    }
    for(int i = 1;i<MAP_SIZEY;i++){
        points.emplace_back(WIDTH-num,i*step);
    }
    for(int i = 1;i<(MAP_SIZEX-1);i++){
        points.emplace_back(i*step,HEIGHT-num);
    }
    int pointindex = QRandomGenerator::global()->bounded(2*(MAP_SIZEX+MAP_SIZEY)-4);
    QPoint tmppoint = points[pointindex];
    this->pointB.rx() = tmppoint.x();
    this->pointB.ry() = tmppoint.y();
}

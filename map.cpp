#include "map.h"
#include <QWidget>
#include <QPainter>
#include "blocks.h"
#include "constNum.h"
#include <QRandomGenerator>
#include <queue>
static int trigger = 0;
Map::Map(QWidget *parent): QWidget(parent){
    WIDTH = MAP_SIZEX*num;
    HEIGHT = MAP_SIZEY*num;
    resize(WIDTH,HEIGHT);
    blocks = new Blocks**[MAP_SIZEX];
    deadBlock = new std::vector<QPoint>;
    for(int i = 0;i<MAP_SIZEX;i++){
        blocks[i]=new Blocks*[MAP_SIZEY];
    }
    for(int i = 0;i<MAP_SIZEY;i++){
        blocks[0][i] = new Blocks(this,true,0);
        blocks[0][i]->setPosition(0,i*num);
        deadBlock->push_back(QPoint(0,i*num));
    }
    for(int i = 1;i<MAP_SIZEX;i++){
        blocks[i][0] = new Blocks(this,true,0);
        blocks[i][0]->setPosition(i*num,0);
        deadBlock->push_back(QPoint(i*num,0));

    }
    for(int i = 1;i< MAP_SIZEY;i++){
        blocks[MAP_SIZEX-1][i] = new Blocks(this,true,0);
        blocks[MAP_SIZEX-1][i]->setPosition((MAP_SIZEX-1)*num,i*num);
        deadBlock->push_back(QPoint(WIDTH-num,i*num));
        }
    for(int i = 1;i<MAP_SIZEX-1;i++){
        blocks[i][MAP_SIZEY-1] = new Blocks(this,true,0);
        blocks[i][MAP_SIZEY-1]->setPosition(i*num,(MAP_SIZEY-1)*num);
        deadBlock->push_back(QPoint(i*num,HEIGHT-num));
    }

    for(int i = 1;i<(MAP_SIZEX-1);i++){
        for(int j = 1;j<(MAP_SIZEY-1);j++){
            blocks[i][j] = new Blocks();
            blocks[i][j]->setPosition(i*num,j*num);
            if(blocks[i][j]->currentType==1)
                type1Count++;
            if(blocks[i][j]->currentType==2)
                type2Count++;
            if(blocks[i][j]->currentType==3)
                type3Count++;
        }
    }
    hero1 = new Player();
    hero2 = new Player();
    while ((hero1->pointB.x() == hero2->pointB.x()) && (hero1->pointB.y() == hero2->pointB.y())) {
        hero1->randomPosition();
        hero2->randomPosition();
    }
    while (hero1->currentStyle == hero2->currentStyle) {
        hero1->setStyle(hero1->random());
    }
    qDebug() <<hero1->currentStyle;
    qDebug() <<hero2->currentStyle;
    //先把边缘的那些blocks放到deadBlocks里面
    timer1 = new QTimer(this);
    timer1->setSingleShot(true);//到时间了就只发送一次timeout然后等待下一次开始计时，而不是一直重复计时
    connect(timer1,&QTimer::timeout,this,&Map::handleLongPress1);
    timer2 = new QTimer(this);
    timer2->setSingleShot(true);//到时间了就只发送一次timeout然后等待下一次开始计时，而不是一直重复计时
    connect(timer2,&QTimer::timeout,this,&Map::handleLongPress2);
}
//用来测试的map定义：
Map::Map(QWidget *parent, int x) : QWidget(parent)
{
    WIDTH = MAP_SIZEX*num;
    HEIGHT = MAP_SIZEY*num;
    resize(WIDTH,HEIGHT);
}

Map::~Map(){
    if(blocks){
        for(int i = 0;i<MAP_SIZEX;i++){
            for(int j = 0;j<MAP_SIZEY;j++){
                delete blocks[i][j];
                blocks[i][j] = NULL;
            }
        }
        for(int i = 0;i<MAP_SIZEX;i++){
            delete[] blocks[i];
            blocks[i] = NULL;
        }
        delete[] blocks;
        blocks = NULL;
    }
    qDebug()<<"delete map";
    delete hero1;
    delete hero2;
    delete deadBlock;
    delete timer1;
    delete timer2;
    delete timer3;
    delete clickPoint;
};
QJsonObject Map::getJsonData() const
{
    return MapData;
}

void Map::toJson()const
{
    qDebug()<<"map json";
    QJsonObject timer1J,timer2J,timer3J;
    QJsonArray deadBlockJ;
    if(timer1&& timer1->isActive()){
        timer1J["remainTime"] = timer1->remainingTime();
        timer1J["isactive"] = timer1->isActive();
    }
    if(timer2&& timer2->isActive()){
        timer2J["remainTime"] = timer2->remainingTime();
        timer2J["isactive"] = timer2->isActive();
    }
    if(timer3&& timer3->isActive()){
        timer3J["remainTime"] = timer3->remainingTime();
        timer3J["isactive"] = timer3->isActive();
    }
    MapData["timerHRemain"] = timerHRemain;
    MapData["currentMode"] = currentMode;
    MapData["timer1"]=timer1J;
    MapData["timer2"]=timer2J;
    MapData["timer3"]=timer3J;
    MapData["paintmap"] = paintmap;
    MapData["painttrace1"] = painttrace1;
    MapData["painttrace2"] = painttrace2;
    MapData["erase"] = erase;
    MapData["bonus1"] = bonus1;
    MapData["bonus2"] = bonus2;
    MapData["bonus3"] = bonus3;
    MapData["bonus4"] = bonus4;
    MapData["isKeyPressed1"] = isKeyPressed1;
    MapData["isKeyPressed2"] = isKeyPressed2;
    MapData["pressedKey1"] = pressedKey1;
    MapData["pressedKey2"] = pressedKey2;
    MapData["Lock"] = Lock;
    MapData["target1X"]=target1.x();
    MapData["target1Y"]=target1.y();
    MapData["target2X"]=target2.x();
    MapData["target2Y"]=target2.y();
    MapData["target3X"]=target3.x();
    MapData["target3Y"]=target3.y();
    MapData["target4X"]=target4.x();
    MapData["target4Y"]=target4.y();
    MapData["type1Count"] = type1Count;
    MapData["type2Count"] = type2Count;
    MapData["type3Count"] = type3Count;
    MapData["gameOver"] = gameOver;
    MapData["paintFinalTrace1"] = paintFinalTrace1;
    MapData["paintFinalTrace2"] = paintFinalTrace2;
    if(clickPoint){
        MapData["clickPointX"] = clickPoint->x();
        MapData["clickPointY"] = clickPoint->y();
    }
    if(deadBlock){
        for(std::vector<QPoint>::iterator it = deadBlock->begin();it!=deadBlock->end();it++)
        {
            QJsonObject tmp;
            tmp["X"] = it->x();
            tmp["Y"] = it->y();
            deadBlockJ.append(tmp);
        }
        MapData["deadBlock"] = deadBlockJ;
        MapData["deadBlockSize"] = static_cast<int>(deadBlock->size());
    }
    if(currentMode==mode2){
        if(timer2&& timer2->isActive())
        timer2J["remainTime"] = timer2->remainingTime();
        if(hero2){
            MapData["hero2"] = hero2->getJsonData();
            //qDebug() << "PlayerData 2content:" << QJsonDocument(hero2->getJsonData()).toJson(QJsonDocument::Indented);
        }
    }
    if(hero1){
        MapData["hero1"] = hero1->getJsonData();
        //qDebug() << "PlayerData content:" << QJsonDocument(hero1->getJsonData()).toJson(QJsonDocument::Indented);
    }
    QJsonArray blocksJ;
    if(blocks){
        for(int i = 0;i<MAP_SIZEX;i++)
        {
            QJsonArray tmp;
            for(int j = 0;j<MAP_SIZEY;j++){
                tmp.append(blocks[i][j]->getJsonData());
            }
            blocksJ.append(tmp);
        }
            MapData["blocks"] = blocksJ;
    }
}

void Map::handleLongPress1()//hero1的处理
{
    //qDebug() <<"long press";
    if(hero1->pointB==target1)//+30s
    {
        qDebug()<<"here";
        bonus1 = false;
        emit addtime();
    }
    if(hero1->pointB==target2)//shuffle
    {
        bonus2 = false;
        shuffle();
    }
    if(hero1->pointB==target3)//hint
    {
        bonus3 = false;
        showHint = true;
        if(timer3)
        {
            timerHRemain += 10;
        }
        else
        {
            timer3 = new QTimer(this);
            timer3->start(1000);
            connect(timer3, &QTimer::timeout,this,&Map::reload);
        }
        emit erase_Couple();
        emit markHint(showHint);
    }
    if(hero1->pointB==target4)//flash
    {
        bonus4 = false;
        flash(hero1,clickPoint);
    }
    update();
}
void Map::handleLongPress2()//hero2的处理
{
    if(hero2->pointB==target1){
        bonus1 = false;
        emit addtime();
    }
    if(hero2->pointB==target2){
        bonus2 = false;
        shuffle();
    }
    if(hero2->pointB==target3){
        bonus3 = false;
        hint();
    }
    update();
}
QPoint Map::getCouple(int x,int y,Blocks ***a)
{
    int **Corner;
    Corner = new int *[MAP_SIZEX];
    for(int i = 0; i < MAP_SIZEX; i++) {
        Corner[i] = new int[MAP_SIZEY];
    }
    for(int i = 0; i < MAP_SIZEX; i++) {
        for(int j = 0; j < MAP_SIZEY; j++) {
            Corner[i][j] = INT_MAX;
        }
    }
    std::queue<Map::Node> Queue;
    Queue.push(Map::Node(x, y, 0, 0));
    Corner[x][y] = 0;
    int direction[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 上、下、左、右
    while(!Queue.empty()) {
        Map::Node current = Queue.front();
        Queue.pop();
        if (a[current.i][current.j]->currentType == a[x][y]->currentType &&
            !(current.i == x && current.j == y) &&current.corners<=2&&
            !a[current.i][current.j]->deadState &&
            !a[x][y]->deadState){
            bool inTriple = tripleLineLink(x,y,current.i,current.j,0);
            if(inTriple){
                for(int i = 0; i < MAP_SIZEX; ++i) {
                    delete[] Corner[i];
                }
                delete[] Corner;
                return QPoint(current.i,current.j);
            }
        }
        for(int p = 0; p < 4; p++) { // 遍历方向数组
            int new_i = current.i + direction[p][0];
            int new_j = current.j + direction[p][1];
            int new_corners = current.corners;
            if( new_j>=0&& new_j<=MAP_SIZEY-1&& new_i>=0&& new_i<=MAP_SIZEX-1)//没出范围
            {
                if (current.dir != 0 && current.dir != p + 1){
                    new_corners += 1;
                }
                if (Corner[new_i][new_j] > new_corners){
                    Corner[new_i][new_j] = new_corners;
                    if (new_corners <= 2&& (a[new_i][new_j]->currentType==a[x][y]->currentType||a[new_i][new_j]->deadState)) {
                        Queue.push(Map::Node(new_i, new_j, new_corners, p + 1)); // 更新dir为p+1
                    }
                }
            }
        }
    }
    for(int i = 0; i < MAP_SIZEX; i++) {
        delete[] Corner[i];
    }
    delete[] Corner;

    return QPoint(-1,-1);
}

void Map::hint()
{
    QPoint p_1,p_2;
    if(blocks[couple1.x()][couple1.y()]->currentType!=blocks[couple2.x()][couple2.y()]->currentType||blocks[couple1.x()][couple1.y()]->deadState||blocks[couple2.x()][couple2.y()]->deadState){
        qDebug() <<"select couples";
        std::queue<std::pair<QPoint,QPoint>> results;
        int direction[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 上、下、左、右
        int nerbor_i = 0,nerbor_j = 0;
        bool exitloop = false;
        for(int i = 0;i<MAP_SIZEX;i++){
            for(int j = 0;j<MAP_SIZEY;j++){
                if((blocks[i][j]->deadState)||blocks[i][j]->coupled)//不断的找到可以发生配对的方块_即不是dead或者还没有couple,dead也有可能没有couple
                    continue;
                else{
                    for(int p = 0;p<4;p++){
                        nerbor_i = direction[p][0]+i;
                        nerbor_j = direction[p][1]+j;
                        if(nerbor_i>=0&&nerbor_j>=0&&nerbor_j<=(MAP_SIZEY-1)&&nerbor_i<=(MAP_SIZEY-1)&&blocks[nerbor_i][nerbor_j]->deadState)//是在边缘的，这样hero可以接触的到
                        {
                            qDebug()<<"in";
                            p_1 = QPoint(i,j);
                            p_2 = getCouple(i,j,blocks);
                            if(p_2==QPoint(-1,-1))
                                continue;
                            else{
                                blocks[p_2.x()][p_2.y()]->coupled = true;
                                blocks[p_1.x()][p_1.y()]->coupled = true;
                                exitloop = true;
                                break;
                            }
                        }
                        if(exitloop)
                            break;
                    }

                }
                if(exitloop)
                    break;
            }
            if(exitloop)
                break;
        }
        couple1 = p_1;
        couple2 = p_2;
        if(p_2==QPoint(-1,-1)){
            showHint = false;
            update();
        }
        update();
    }
    else
        return;
}

bool Map::isValid(int x,int y,Blocks*** a)
{
    return (x>=0&&x<=(MAP_SIZEX-1)&&y>=0&&y<=(MAP_SIZEY-1)&&a[x][y]->deadState);
}


void Map::flash(Player *hero,QPoint *a)
{
    QPropertyAnimation *animation = new QPropertyAnimation(hero, "geometry");
    animation->setDuration(2000); // 动画持续时间为1000毫秒
    animation->setStartValue(QRect(hero->pointB.x(), hero->pointB.y(),64, 64)); // 动画起始位置
    animation->setEndValue(QRect(a->x(), a->y(), 64, 64)); // 动画结束位置
    animation->start(); // 启动动画
    hero->pointB = QPoint(a->x(),a->y());
    update();
}
void Map::setMode(Mode mode)
{
    currentMode = mode;
}
void Map::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    paintMap(painter);
    if(paintFinalTrace1){
        paintFinalTrace(painter,hero1);
        paintFinalTrace1=false;
    }
    if(paintFinalTrace2){
        paintFinalTrace(painter,hero2);
        paintFinalTrace2=false;

    }
    if(hero1->highlightState&&hero1->highlightBlock.size()<=2){
        paintHighlight(painter,hero1);
    }
    if(currentMode==mode2){
        if(hero2->highlightState&&hero2->highlightBlock.size()<=2)
        {
            paintHighlight(painter,hero2);
        }
    }
    if(bonus1||bonus2||bonus3||bonus4){
        //qDebug() <<"paint bonus";
        paintBonus(painter);
    }
    if(showHint){
        paintHints(painter);
    }
    //qDebug()<<"trigger";
    //qDebug()<<gameOver;
    if(gameOver){
        //qDebug()<<gameOver;
        paintGains(painter);
    }
}
void Map::paintHints(QPainter &painter)
{
    qDebug()<<"paint HINTS";
    painter.setOpacity(0.5);
    painter.setPen(QPen(Qt::NoPen));
    painter.setBrush(QBrush(Qt::green));
    painter.drawRect(couple1.x()*num,couple1.y()*num,num,num);
    painter.drawRect(couple2.x()*num,couple2.y()*num,num,num);
}
void Map::paintFinalTrace(QPainter &painter, Player* hero)
{
    QPen pen1;
    pen1 = QPen(Qt::red);
    pen1.setWidth(5);
    painter.setPen(pen1);
    qDebug()<<hero->trace;
    while(!hero->trace->empty()){
        painter.drawLine(hero->trace->front());
        hero->trace->pop_front();
        qDebug() <<"hero->trace->size()"<<hero->trace->size();
    }
}
void Map::paintHighlight(QPainter &painter,Player* hero)
{
    painter.setOpacity(0.5);  // 设置透明度为50%
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::NoBrush));
    if(hero==hero1)
        painter.setBrush(QBrush(Qt::blue));
    else
        painter.setBrush(QBrush(Qt::red));
    for(std::vector<QPoint>::iterator it  = hero->highlightBlock.begin();it!=hero->highlightBlock.end();it++){
        qDebug()<<"highlight coordinates"<<it->x()<<it->y();
        painter.drawRect(it->x(),it->y(),num,num);
    }
    if(hero->highlightBlock.size()==2) //这个说明这次已经是有两个点亮了。应该清空了
{
        while(hero->highlightBlock.size()>0){
            hero->highlightBlock.pop_back();
        }
    }
    if(hero->highlightNum==2){
        hero->highlightNum = 0;
        hero->highlightState = false;
    }

}
void Map::paintBonus(QPainter &painter)
{
    if(bonus1)//+30s
    {
        bonus_1 = new Blocks(this,1);
        painter.drawPixmap(target1,bonus_1->pixmap);
    }
    if(bonus2)//shuffle
    {
        bonus_2 = new Blocks(this,2);
        painter.drawPixmap(target2,bonus_2->pixmap);
    }
    if(bonus3)//hint
    {
        //qDebug()<<"hint paint bonus";
        bonus_3 = new Blocks(this,3);
        painter.drawPixmap(target3,bonus_3->pixmap);
    }
    if(bonus4)//flash
    {
        bonus_4 = new Blocks(this,4);
        painter.drawPixmap(target4,bonus_4->pixmap);
    }
}
void Map::setPaintmap(bool x)
{
    paintmap = x;
}
void Map::setPaintFinalTrace1(bool x)
{
    paintFinalTrace1 = x;
}
void Map::setPaintFinalTrace2(bool x)
{
    paintFinalTrace2 = x;
}
void Map::paintMap(QPainter &painter)
{
    //qDebug()<< "PaintMap event triggered";
    painter.translate(0,0);
    QColor brightyellow(255, 255, 0);
    QBrush brush(brightyellow);
    painter.setBrush(brush);
    QPen pen(Qt::NoPen);
    painter.setPen(pen);
    painter.drawRect(0,0,WIDTH,HEIGHT);
    //qDebug()<<WIDTH<<HEIGHT;
    for(int i = 0;i<MAP_SIZEX;i++){
        for(int j = 0;j<MAP_SIZEY;j++){
            QPoint position(blocks[i][j]->X,blocks[i][j]->Y);
            //qDebug()<<i<<blocks[i][j]->X<<j<<blocks[i][j]->Y;
            if(blocks[i][j]->deadState||blocks[i][j]->currentType==Blocks::type0)
            {
                continue;
            }
            // qDebug() <<"i :"<<i;
            // qDebug() << "j :" <<j;
            // qDebug() << "blocks[i][j].currentType" << blocks[i][j].currentType ;
            // qDebug() <<"picture: " << &blocks[i][j].pixmap;
            painter.drawPixmap(position,blocks[i][j]->pixmap);
        }
    }
    switch (currentMode){
    case mode1:
        hero1->paintPlayer(painter);
        break;
    case mode2:
        hero1->paintPlayer(painter);
        hero2->paintPlayer(painter);
        break;
    }
}
void Map::bonus(int value)
{
    int random_index = QRandomGenerator::global()->bounded(deadBlock->size());
    if(value == 1&&currentMode==mode1){
        target1 = (*deadBlock)[random_index];
        bonus1 = true;
    }
    if(value ==2){   target2 = (*deadBlock)[random_index];
        bonus2 = true;
    }
    if(value ==3&&currentMode==mode1)//hint
    {
        target3 = (*deadBlock)[random_index];
        bonus3 = true;
    }
    if(value ==4&&currentMode==mode1){
        target4 = (*deadBlock)[random_index];
        bonus4 = true;
    }
    update();
}
void Map::shuffle( )
{
    //qDebug()<<" shuffle";
    QPoint p_1(0,0),p_2(0,0);
    int deadBlockCount = deadBlock->size()-(2*(MAP_SIZEX+MAP_SIZEY)-4);
    delete deadBlock;
    deadBlock = new std::vector<QPoint>;
    p_1 = hero1->pointB;
    //删除原来的blocks
    for(int i = 0;i<MAP_SIZEX;i++){
        for(int j = 0;j<MAP_SIZEY;j++){
            delete blocks[i][j];
            blocks[i][j] = NULL;
        }
    }
    for(int i = 0;i<MAP_SIZEX;i++){
        delete[] blocks[i];
        blocks[i] = NULL;
    }
    delete[] blocks;
    blocks = NULL;
    std::vector<int> random_type = randomGenerator(type1Count,type2Count,type3Count,deadBlockCount);
    //qDebug() <<type1Count;
    //qDebug() <<type2Count;
    // qDebug() <<type3Count;
    // qDebug() <<deadBlockCount;
    // qDebug() <<random_type.size();
    blocks = new Blocks**[MAP_SIZEX];
    for(int i = 0;i<MAP_SIZEX;i++){
        blocks[i]=new Blocks*[MAP_SIZEY];
    }
    //边框为dead。
    for(int i = 0;i<MAP_SIZEX;i++)
        for(int i = 0;i<MAP_SIZEY;i++){
            blocks[0][i] = new Blocks(this,true,0);
            blocks[0][i]->setPosition(0,i*num);
        }
    for(int i = 1;i<MAP_SIZEX;i++){
        blocks[i][0] = new Blocks(this,true,0);
        blocks[i][0]->setPosition(i*num,0);
    }
    for(int i = 1;i< MAP_SIZEY;i++){
        blocks[MAP_SIZEX-1][i] = new Blocks(this,true,0);
        blocks[MAP_SIZEX-1][i]->setPosition((MAP_SIZEX-1)*num,i*num);
    }
    for(int i = 1;i<MAP_SIZEX-1;i++){
        blocks[i][MAP_SIZEY-1] = new Blocks(this,true,0);
        blocks[i][MAP_SIZEY-1]->setPosition(i*num,(MAP_SIZEY-1)*num);
    }
    for(int i = 1;i<MAP_SIZEX-1;i++){
        for(int j = 1;j<MAP_SIZEY-1;j++){
            blocks[i][j] = new Blocks(this,false,random_type[(i-1)*(MAP_SIZEY-2)+j-1 ]);
            blocks[i][j]->setPosition(i*num,j*num);
            //qDebug() << i*num <<" " <<j*num;
        }
    }
    std::vector<QPoint> illegal;
    if(currentMode == mode2){
        p_2 = hero2->pointB;
    }
    if(!blocks[p_1.x()/num][p_1.y()/num]->deadState){
        illegal.push_back(QPoint(p_1.x()/num,p_1.y()/num));
    }
    if(currentMode==mode2&&!blocks[p_2.x()/num][p_2.y()/num]->deadState){
        illegal.push_back(QPoint(p_2.x()/num,p_2.y()/num));
    }
    if(bonus1&&!blocks[target1.x()/num][target1.y()/num]->deadState){
        illegal.push_back(QPoint(target1.x()/num,target1.x()/num));
    }
    if(bonus2&&!blocks[target2.x()/num][target2.y()/num]->deadState){
        illegal.push_back(QPoint(target2.x()/num,target2.x()/num));
    }
    if(bonus3&&!blocks[target3.x()/num][target3.y()/num]->deadState){
        illegal.push_back(QPoint(target3.x()/num,target3.x()/num));
    }
    if(bonus4&&!blocks[target4.x()/num][target4.y()/num]->deadState){
        illegal.push_back(QPoint(target4.x()/num,target4.x()/num));
    }
    for(std::vector<QPoint>::iterator it = illegal.begin();it!=illegal.end();it++){
        Blocks::Type tmp = blocks[it->x()][it->y()]->currentType;
        int x = tmp;
        int i,j;
        bool exitloop = false;
        for(i = 1;i<(MAP_SIZEX-1);i++){
            for(j = 1;j<(MAP_SIZEY-1);j++){
                if(random_type[(i-1)*(MAP_SIZEY-2)+j-1]==0)//找到一个被标记为dead的
                {
                    exitloop = true;
                    break;
                }
            }
            if(exitloop)
                break;
        }
        delete blocks[it->x()][it->y()];
        delete blocks[i][j];
        blocks[it->x()][it->y()] = new Blocks(this,true,0);//把hero脚下的标记为dead
        blocks[i][j] = new Blocks(this,false,x);
    }
    for(int p = 0;p<MAP_SIZEX;p++){
        for(int k = 0;k<MAP_SIZEY;k++){
            if(blocks[p][k]->deadState)
                deadBlock->push_back(QPoint(p*num,k*num));
        }
    }
    if(showHint)//避免shuffle之后原本被标记的走失
    {
        //qDebug() <<"from shuffle to hint";
        emit erase_Couple();
    }
    update();
}
std::vector<int> Map::randomGenerator(int a, int b, int c,int d)
{
    std::vector<int> pool;
    pool.reserve(a + b + c +d); // 预分配空间以提高效率
    for (int i = 0; i < a; ++i) {
        pool.push_back(1);
    }
    for (int i = 0; i < b; ++i) {
        pool.push_back(2);
    }
    for (int i = 0; i < c; ++i) {
        pool.push_back(3);
    }
    for (int i = 0; i < d; ++i) {
        pool.push_back(0);
    }
    std::vector<int> result;  // 用于存储随机生成的结果
    result.reserve(a + b + c +d); // 预分配空间以提高效率
    // 随机生成数并存入结果数组
    while (!pool.empty()) {
        int index = rand() % pool.size(); // 从池中随机选择一个索引
        result.push_back(pool[index]);    // 将随机选择的数存入结果数组
        // 从池中移除选择的元素
        pool.erase(pool.begin() + index);
    }
    //qDebug()<<"result generated";
    return result; // 返回结果数组
}
bool Map::testify_Crash(int dir, const Player *hero)
{
    int i,j,x,y;
    i = hero->pointB.x()/num-1;
    j = hero->pointB.y()/num-1;
    bool i_d,state,t_d;//to anchor是指从空地走向目標方块然后选中它，state是判断是否为从刚刚anchor好但还没有消除的方块，run是允许从已经anchor好的方块走向空地
    if(i<(MAP_SIZEX-2)&&i>=0&&j<(MAP_SIZEY-2)&&j>=0){
        if(hero==hero1)
            state = blocks[i][j]->anchorState1;
        if(hero==hero2)
            state = blocks[i][j]->anchorState2;
    }
    QPoint p_1,p_2,p_tmp;//这两个是下一步要走向的方块
    switch(currentMode){
    case mode1://hero就对应的hero1
        switch(dir){
        case 0://左
            x = hero->pointB.x()-num;
            y = hero->pointB.y();
            p_1 = QPoint(x,y);
            if(hero->trc_Line->size()>0){
                p_tmp = QPoint(hero->trc_Line->back().p1().x()-num/2,hero->trc_Line->back().p1().y()-num/2);
                if(p_tmp==p_1)
                {
                    // qDebug() << "repeat";
                    return false;}
            }
            else{
                t_d = std::find(deadBlock->begin(), deadBlock->end(), p_1) != deadBlock->end();
                i_d = std::find(deadBlock->begin(), deadBlock->end(), hero->pointB) != deadBlock->end();
                if((!i_d)&&(!t_d))
                    return true;
                else
                    return false;
            }
            break;
        case 1://右
            x = hero->pointB.x()+num;
            y = hero->pointB.y();
            p_1 = QPoint(x,y);
            if(hero->trc_Line->size()>0){
                p_tmp = QPoint(hero->trc_Line->back().p1().x()-num/2,hero->trc_Line->back().p1().y()-num/2);
                if(p_tmp==p_1){
                    // qDebug() << "repeat";
                    return false;
                }
            }
            else{
                t_d = std::find(deadBlock->begin(), deadBlock->end(), p_1) != deadBlock->end();
                i_d = std::find(deadBlock->begin(), deadBlock->end(), hero->pointB) != deadBlock->end();
                if((!i_d)&&(!t_d))
                    return true;
                else
                    return false;
            }
            break;
        case 2://下
            x = hero->pointB.x();
            y = hero->pointB.y()+num;
            p_1 = QPoint(x,y);
            if(hero->trc_Line->size()>0){
                p_tmp = QPoint(hero->trc_Line->back().p1().x()-num/2,hero->trc_Line->back().p1().y()-num/2);
                if(p_tmp==p_1){
                    // qDebug() << "repeat";
                    return false;}
            }
            else
            {
                t_d = std::find(deadBlock->begin(), deadBlock->end(), p_1) != deadBlock->end();
                i_d = std::find(deadBlock->begin(), deadBlock->end(), hero->pointB) != deadBlock->end();
                if((!i_d)&&(!t_d))
                    return true;
                else
                    return false;
            }
            break;
        case 3://上
            x = hero->pointB.x();
            y = hero->pointB.y()-num;
            p_1 = QPoint(x,y);
            if(hero->trc_Line->size()>0){
                p_tmp = QPoint(hero->trc_Line->back().p1().x()-num/2,hero->trc_Line->back().p1().y()-num/2);
                if(p_tmp==p_1){
                    // qDebug() << "repeat";
                    return false;}
            }
            else{
                t_d = std::find(deadBlock->begin(), deadBlock->end(), p_1) != deadBlock->end();
                i_d = std::find(deadBlock->begin(), deadBlock->end(), hero->pointB) != deadBlock->end();
                if(!i_d&&!t_d)
                    return true;
                else
                    return false;
            }
            break;
        }
    case mode2:
        if(hero==hero1){
            p_2 = hero2->pointB;
        }
        if(hero==hero2){
            p_2 = hero1->pointB;
        }
        switch(dir){
            case 0://左
                x = hero->pointB.x()-num;
                y = hero->pointB.y();
                p_1 = QPoint(x,y);
                t_d = std::find(deadBlock->begin(), deadBlock->end(), p_1) != deadBlock->end();
                i_d = std::find(deadBlock->begin(), deadBlock->end(), hero->pointB) != deadBlock->end();
                if((p_1==p_2)||(!i_d&&!t_d ))
                    return true;
                else
                    return false;
            case 1://右
                x = hero->pointB.x()+num;
                y = hero->pointB.y();
                p_1 = QPoint(x,y);
                t_d = std::find(deadBlock->begin(), deadBlock->end(), p_1) != deadBlock->end();
                i_d = std::find(deadBlock->begin(), deadBlock->end(), hero->pointB) != deadBlock->end();
                if((p_1==p_2)||(!i_d&&!t_d ))
                    return true;
                else
                    return false;
            case 2://下
                x = hero->pointB.x();
                y = hero->pointB.y()+num;
                p_1 = QPoint(x,y);
                t_d = std::find(deadBlock->begin(), deadBlock->end(), p_1) != deadBlock->end();
                i_d = std::find(deadBlock->begin(), deadBlock->end(), hero->pointB) != deadBlock->end();
                if((p_1==p_2)||(!i_d&&!t_d ))
                    return true;
                else
                    return false;
            case 3://上
                x = hero->pointB.x();
                y = hero->pointB.y()-num;
                p_1 = QPoint(x,y);
                t_d = std::find(deadBlock->begin(), deadBlock->end(), p_1) != deadBlock->end();
                i_d = std::find(deadBlock->begin(), deadBlock->end(), hero->pointB) != deadBlock->end();
                if((p_1==p_2)||(!i_d&&!t_d ))
                    return true;
                else
                    return false;
        }
        break;
    }
}
void Map::keyPressEvent(QKeyEvent *event)
{
    bool crash = false;
    if (currentMode == mode1) {  // MODE2
        if (event->key()) {
            switch (event->key()) {
            case Qt::Key_A:
                crash = testify_Crash(0,hero1);
                if(crash){
                    qDebug()<<"crash";
                    break;
                }
                else{
                    hero1->pMove(0);
                    break;
                }
            case Qt::Key_D:
                crash = testify_Crash(1,hero1);
                if(crash){
                    qDebug()<<"crash";
                    break;
                }
                else{
                    hero1->pMove(1);
                    break;
                }
            case Qt::Key_S:
                crash = testify_Crash(2,hero1);
                if(crash){
                    qDebug()<<"crash";

                    break;
                }
                else{
                    hero1->pMove(2);
                    break;
                }
            case Qt::Key_W://up
                crash = testify_Crash(3,hero1);
                if(crash){
                    //qDebug()<<"crash";
                    break;
                }
                else{
                    hero1->pMove(3);
                    break;
                }
            case Qt::Key_E:
                if (!isKeyPressed1) {
                    isKeyPressed1 = true;
                    pressedKey1 = event->key();
                    timer1->start(1000);
                }
                break;
            default:
                qDebug() << "undetected key press";
            }
        }
    }
    if (currentMode == mode2) {  // MODE2
        if (event->key()) {
            switch (event->key()) {
            case Qt::Key_A:
                crash = testify_Crash(0,hero1);
                if(crash){
                    break;
                }
                else{
                    hero1->pMove(0);
                    break;
                }
            case Qt::Key_D:
                crash = testify_Crash(1,hero1);
                if(crash){
                    break;
                }
                else{
                    hero1->pMove(1);
                    break;
                }
            case Qt::Key_S:
                crash = testify_Crash(2,hero1);
                if(crash){
                    break;
                }
                else{
                    hero1->pMove(2);
                    break;
                }
            case Qt::Key_W:
                crash = testify_Crash(3,hero1);
                if(crash){
                    break;
                }
                else{
                    hero1->pMove(3);
                    break;
                }
            case Qt::Key_J:
                crash = testify_Crash(0,hero2);
                if(crash){
                    break;
                }
                else{
                    hero2->pMove(0);
                    break;
                }
            case Qt::Key_L:
                crash = testify_Crash(1,hero2);
                if(crash){
                    break;
                }
                else{
                    hero2->pMove(1);
                    break;
                }
            case Qt::Key_K:
                crash = testify_Crash(2,hero2);
                if(crash){
                    break;
                }
                else{
                    hero2->pMove(2);
                    break;
                }
            case Qt::Key_I:
                crash = testify_Crash(3,hero2);
                if(crash){
                    break;
                }
                else{
                    hero2->pMove(3);
                    break;
                }
            case Qt::Key_E:
                if (!isKeyPressed1) {
                    isKeyPressed1 = true;
                    pressedKey1 = event->key();
                    timer1->start(1000);
                }
                break;
            case Qt::Key_U:
                if (!isKeyPressed1) {
                    isKeyPressed2 = true;
                    pressedKey2 = event->key();
                    timer2->start(1000);
                }
            default:
                qDebug() << "undetected key press";
            }
        }
    }
    update();
}
void Map::keyReleaseEvent(QKeyEvent *event)
{
    //qDebug()<<"release";
    if (isKeyPressed1 && event->key() == pressedKey1) {
        if (timer1->isActive()) {
            // 如果计时器还在运行，说明是短按
            timer1->stop();
            anchor(QPoint(hero1->pointB),hero1);
        }
        isKeyPressed1 = false;
        handleLongPress1();
    }
    if (isKeyPressed2 && event->key() == pressedKey2) {
        if (timer2->isActive()) {
            // 如果计时器还在运行，说明是短按
            timer2->stop();
            anchor(QPoint(hero2->pointB),hero2);
        }
        isKeyPressed2 = false;
        handleLongPress2();
    }
}
void Map::highLight(QPoint X,Player *hero)
{
    QPoint position(hero->pointB.x(),hero->pointB.y());
    //把对应的X给高亮了就行！
    for (auto it = deadBlock->begin(); it != deadBlock->end(); ++it) {
        if(*it == position){
            qDebug() << "cannot hightlight a dead block!!!";
        }
    }
    hero->highlightBlock.push_back(X);
    hero->highlightNum++;
    if(hero->highlightNum<=2){
        hero->highlightState = true;
    }
}

void Map::anchor(QPoint X,Player *hero)
{
    int i = X.x()/num;
    int j = X.y()/num;
    if(i<0||i>(MAP_SIZEX-1)||j<0||j>(MAP_SIZEY-1)||blocks[i][j]->deadState==true){
        qDebug()<<"wrong anchor! out of range!";
        return;
    }
    if(hero==hero1){
        if(blocks[i][j]->anchorState1)
            return;
        blocks[i][j]->anchorState1 = true;
    }
    if(hero==hero2){
        if(blocks[i][j]->anchorState2)
            return;
        blocks[i][j]->anchorState2 = true;
    }
    hero->anchorNum++;
    if((hero->anchorNum>=1)&&(hero->anchorNum<=2)){
        if(hero==hero1)
            painttrace1 = true;
        if(hero==hero2)
            painttrace2 = true;
    }
    highLight(X,hero);
    if(hero->anchorNum==2){
        bool Erase = testify_Erase(hero);
        hero->anchorNum = 0;
        pause(100);
        bool gameState = testify_Game();
        int remain = 10*10-deadBlock->size();
        qDebug()<<gameState;
        if(!gameState){
            if(remain==0)
                emit gameFinished();//都清空
            else{
                emit gameFinished();
                Lock = true;//锁局
            }
        }
    }
    update();
}
bool Map::testify(int x, int y, Blocks*** a) {
    int **Corner;
    Corner = new int *[MAP_SIZEX];
    for(int i = 0; i < MAP_SIZEX; i++) {
        Corner[i] = new int[MAP_SIZEY];
    }
    for(int i = 0; i < MAP_SIZEX; i++) {
        for(int j = 0; j < MAP_SIZEY; j++) {
            Corner[i][j] = INT_MAX;
        }
    }
    std::queue<Map::Node> Queue;
    Queue.push(Map::Node(x, y, 0, 0));
    Corner[x][y] = 0;
    int direction[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 上、下、左、右
    while(!Queue.empty()) {
        Map::Node current = Queue.front();
        Queue.pop();
        if (a[current.i][current.j]->currentType == a[x][y]->currentType &&
            !(current.i == x && current.j == y) &&current.corners<=2&&
            !a[current.i][current.j]->deadState &&
            !a[x][y]->deadState){
            bool inTriple = tripleLineLink(x,y,current.i,current.j,0);
            if(inTriple){
                for(int i = 0; i < MAP_SIZEX; ++i) {
                    delete[] Corner[i];
                }
                delete[] Corner;
                return true;
            }
        }
        for(int p = 0; p < 4; p++) { // 遍历方向数组
            int new_i = current.i + direction[p][0];
            int new_j = current.j + direction[p][1];
            int new_corners = current.corners;
            if( new_j>=0&& new_j<=(MAP_SIZEY-1)&& new_i>=0&& new_i<=(MAP_SIZEX-1))//没出范围
            {
                if (current.dir != 0 && current.dir != p + 1){
                    new_corners += 1;
                }
                if (Corner[new_i][new_j] > new_corners){
                    Corner[new_i][new_j] = new_corners;
                    if (new_corners <= 2&& (a[new_i][new_j]->currentType==a[x][y]->currentType||a[new_i][new_j]->deadState)) {
                        Queue.push(Map::Node(new_i, new_j, new_corners, p + 1)); // 更新dir为p+1
                    }
                }
            }
        }
    }
    for(int i = 0; i < MAP_SIZEX; i++) {
        delete[] Corner[i];
    }
    delete[] Corner;
    return false;
}
bool Map::oneLineLink(int i_1,int j_1,int i_2,int j_2,int playerId)
{
    int max,min;
    if(!blocks[i_1][j_1]->deadState&&!blocks[i_2][j_2]->deadState)
        return false;
    if(i_1!=i_2&&j_1!=j_2)
        return false;
    else if(i_1==i_2){
        max = qMax(j_1,j_2);
        min = qMin(j_1,j_2);
        for(int i = min+1;i<max;i++){
            if(!blocks[i_1][i]->deadState)
                return false;
        }
        if(playerId == 1){
            hero1->trace->push_back(QLine(QPoint(i_1*num+num/2,j_1*num+num/2),QPoint(i_2*num+num/2,j_2*num+num/2)));
        }
        if(playerId == 2){
            hero2->trace->push_back(QLine(QPoint(i_1*num+num/2,j_1*num+num/2),QPoint(i_2*num+num/2,j_2*num+num/2)));
            qDebug()<<"finish push";
        }
        return true;
    }
    else if(j_1==j_2){
        max = qMax(i_1,i_2);
        min = qMin(i_1,i_2);
        for(int i = min+1;i<max;i++){
            if(!blocks[i][j_1]->deadState)
                return false;
        }
        if(playerId == 1){
            hero1->trace->push_back(QLine(QPoint(i_1*num+num/2,j_1*num+num/2),QPoint(i_2*num+num/2,j_2*num+num/2)));
        }
        if(playerId == 2){
            hero2->trace->push_back(QLine(QPoint(i_1*num+num/2,j_1*num+num/2),QPoint(i_2*num+num/2,j_2*num+num/2)));
        }
        return true;
    }
    return false;
}
bool Map::doubleLineLink(int i_1,int j_1,int i_2,int j_2,int playerId)
{
    bool check1v = oneLineLink(i_1,j_1,i_1,j_2,playerId);
    bool check1h = oneLineLink(i_1,j_2,i_2,j_2,playerId);
    if(check1v&&check1h&&blocks[i_1][j_2]->deadState){
        return true;
    }
    else{
        if(playerId == 1){
            while(hero1->trace->size()>1){
                hero1->trace->pop_back();
            }
        }
        if(playerId == 2){
            while(hero2->trace->size()>1){
                hero2->trace->pop_back();
            }
        }
        bool check2v = oneLineLink(i_2,j_1,i_2,j_2,playerId);
        bool check2h = oneLineLink(i_1,j_1,i_2,j_1,playerId);
        if(check2v&&check2h&&blocks[i_2][j_1]->deadState)
            return true;
        else{
            if(playerId == 1){
                while(hero1->trace->size()>1){
                    hero1->trace->pop_back();
                }
            }
            if(playerId == 2){
                while(hero2->trace->size()>1){
                    hero2->trace->pop_back();
                }
            }
            return false;
        }
    }
}
bool Map::tripleLineLink(int i_1,int j_1,int i_2,int j_2,int playerId)//playerId == 0就说明是不用考虑finaltrace的情况
{

    qDebug()<<"test trace";
    int direction[4][2]={{-1,0},{1,0},{0,1},{0,-1}};
    int new_i,new_j;
    for(int i =0;i<4;i++)
    {

        new_i = i_1+direction[i][0];
        new_j = j_1+direction[i][1];
        while(new_i<=(MAP_SIZEX-1)&&new_i>=0&&new_j<=(MAP_SIZEY-1)&&new_j>=0){
            if(blocks[new_i][new_j]->deadState){
                if(playerId == 1){
                    hero1->trace = new QQueue<QLine>;
                    hero1->trace->push_back(QLine(QPoint(i_1*num+num/2,j_1*num+num/2),QPoint(new_i*num+num/2,new_j*num+num/2)));
                }
                if(playerId == 2){
                    hero2->trace = new QQueue<QLine>;
                    qDebug()<<hero2->trace;
                    hero2->trace->push_back(QLine(QPoint(i_1*num+num/2,j_1*num+num/2),QPoint(new_i*num+num/2,new_j*num+num/2)));
                }
                bool link = doubleLineLink(new_i,new_j,i_2,j_2,playerId);
                if(link)
                    return true;
            }
            else
                break;
            new_i +=direction[i][0];
            new_j +=direction[i][1];
        }
    }
}
bool Map::testify_Erase(Player *hero)
//连续双击同一个色块是不被允许的
{
    int i_1 = hero->highlightBlock[0].x()/num;
    int j_1 = hero->highlightBlock[0].y()/num;
    int i_2 = hero->highlightBlock[1].x()/num;
    int j_2 = hero->highlightBlock[1].y()/num;
    if(hero==hero1){
        blocks[i_1][j_1]->anchorState1 = false;
        blocks[i_2][j_2]->anchorState1 = false;
        painttrace1 = false; //这次的update就不要再画轨迹了，所以再把轨迹清空
    }
    if(hero==hero2){
        blocks[i_1][j_1]->anchorState2 = false;
        blocks[i_2][j_2]->anchorState2 = false;
        painttrace2 = false; //这次的update就不要再画轨迹了，所以再把轨迹清空
    }
    if((i_1==i_2)&&(j_1==j_2)){
        erase = false;
    }
    if(blocks[i_1][j_1]->currentType!=blocks[i_2][j_2]->currentType){
        erase = false;
        update();
    }
    else{
        bool testify;
        if(hero==hero1)
            testify = tripleLineLink(i_1,j_1,i_2,j_2,1);
        if(hero==hero2){
            testify = tripleLineLink(i_1,j_1,i_2,j_2,2);
        }
        if(!testify){
            qDebug() << "can not be erase";
            erase = false;
            update();
        }
        else{
            erase = true;
            if(hero == hero1){
                paintFinalTrace1 = true;
            }
            if(hero == hero2){
                paintFinalTrace2 = true;
            }
            qDebug() << paintFinalTrace2;
            blocks[i_1][j_1]->deadState = true;
            blocks[i_2][j_2]->deadState = true;
            if(blocks[i_1][j_1]->currentType==1)
                type1Count-=2;
            if(blocks[i_1][j_1]->currentType==2)
                type2Count-=2;
            if(blocks[i_1][j_1]->currentType==3)
                type3Count-=2;
            deadBlock->push_back(QPoint(hero->highlightBlock[0].x(),hero->highlightBlock[0].y()));
            deadBlock->push_back(QPoint(hero->highlightBlock[1].x(),hero->highlightBlock[1].y()));
            if(showHint){
                qDebug()<<"hint anothor";
                emit erase_Couple();
            }
            update();
            emit hero->match();
        }
    }
    delete hero->trc_Line;
    hero->trc_Line = new std::vector<QLine>;
    return erase;
}
bool Map::testify_Angle(const QLine &a,const QLine &b)
{
    //qDebug() <<"testify_Angle";
    QPoint p_1,p_2;
    //qDebug() << "a.p2().x():"<< a.p2().x();
   //qDebug() << "a.p2().y():"<< a.p2().y();
    p_1.rx() = a.x2()-a.x1();
    p_1.ry() = a.y2()-a.y1();
    p_2.rx() = b.x2()-b.x1();
    p_2.ry() = b.y2()-b.y1();
    int dotProduct = p_1.x()*p_2.x()+p_1.y()*p_2.y();
    //qDebug() << "dotProduct"<< dotProduct;
    return dotProduct==0;
}
bool Map::testify_Game()
//true是仍旧可以进行游戏，false是不能进行游戏了
//这个只用来看是否有解
{
    qDebug() <<"testify game";
    int counts = 0;
    bool solution1 = false,solution2 = false,exitloop = false;
    switch(currentMode){
    case mode1:
        for(int i = 0;i<MAP_SIZEX;i++){
            for(int j = 0;j<MAP_SIZEY;j++){
                //qDebug()<<"check";
                if(!blocks[i][j]->deadState){
                    //qDebug()<<"go to test";
                    solution1 = testify(i,j,blocks);
                    if(solution1){
                        exitloop = true; //找到一对有解的就行了
                        break;
                    }
                }
                else
                    continue;
            }
            if(exitloop)
                break;
        }
        return solution1;
    case mode2:
        for(int i = 0;i<MAP_SIZEX;i++){
            for(int j = 0;j<MAP_SIZEY;j++){
                if(!blocks[i][j]->deadState){
                    solution1 = testify(i,j,blocks);
                    if(solution1){
                        counts++;
                        if(counts==2){
                            exitloop = true;//保证俩hero都能有解
                            solution2 = true;
                            break;
                        }
                    }
                }
                else
                    continue;
            }
            if(exitloop)
                break;
        }
        return solution2;
        break;
    }
}

void Map::freeze()
{
    //qDebug() <<"freeze";
    this->releaseKeyboard();
}
void Map::melt()
{
    this->setFocus();
    this->grabKeyboard();
}
void Map::mousePressEvent(QMouseEvent *event)
{
    //qDebug() <<"mouse click";
    QPoint locate;
    if(event->button()==Qt::LeftButton){
        locate = event->pos();//要确定一下locate到底是对应到什么位置。
    }
    int x = locate.x()/num;
    int y = locate.y()/num;
    clickPoint = new QPoint(x*num,y*num);
    //qDebug() << clickPoint->x();
    //qDebug() << clickPoint->y();
}
void Map::getGains()
{
    //修改state
    //qDebug()<<"here";
    gameOver = true;
    update();
}
void Map::paintGains(QPainter &painter)
{
    //qDebug() <<"paint gains";
    QColor orange(255, 165, 0);
    QString heroA = "HERO1";
    QString heroB = "HERO2";
    QString winner = (hero1->scores > hero2->scores) ? heroA : heroB;
    QString text1 = QString("YOUR SCORE:\n %1").arg(hero1->scores);
    QString text2 = QString("HERO1 SCORE: %1\n HERO2 SCORE: %2\n WINNER: %3").arg(hero1->scores).arg(hero2->scores).arg(winner);
    QString text3 = QString("LOCK!!");
    painter.setPen(QPen(Qt::NoPen));
    painter.setBrush(QBrush(orange));
    painter.setFont(QFont("Arial",MAP_SIZEX*5,QFont::Bold));
    switch(currentMode){
    case mode1:
        // painter.drawRect(170,200,300,200);
        painter.setPen(QPen(Qt::green));
        painter.drawText(QRectF(num/2,num/2,MAP_SIZEX*50,MAP_SIZEY*30),Qt::AlignHCenter | Qt::TextWordWrap,text1);
        break;
    case mode2:
        painter.setPen(QPen(Qt::green));
        painter.drawText(QRectF(num/2,num/2,MAP_SIZEX*50,MAP_SIZEY*50),Qt::AlignHCenter | Qt::TextWordWrap,text2);
    }
    if(Lock){
        painter.setPen(QPen(Qt::red));
        painter.setFont(QFont("Arial",MAP_SIZEX*5,QFont::Bold));
        painter.drawText(QRectF((MAP_SIZEX/2-1)*num,2*num,MAP_SIZEX*50,MAP_SIZEY*50),Qt::AlignHCenter | Qt::TextWordWrap,text3);
    }
}
bool Map::isGameOver()
{
    return gameOver;
}
bool Map::isLock()
{
    return Lock;
}

void Map::setGameOver(bool x)
{
    gameOver =x;
}
void Map::setLock(bool x)
{
    Lock = x;
}
void Map::setPaintMap(bool x)
{
    paintmap = x;
}
void Map::setPaintTrace1(bool x)
{
    painttrace1 = x;
}
void Map::setPaintTrace2(bool x)
{
    painttrace2 = x;
}
void Map::setErase(bool x)
{
    erase = x;
}
void Map::setBonus1(bool x)
{
    bonus1 = x;
}
void Map::setBonus2(bool x)
{
    bonus2 = x;
}
void Map::setBonus3(bool x)
{
    bonus3 = x;
}
void Map::setBonus4(bool x){
    bonus4 = x;
}
void Map::setIsKeyPressed1(bool x)
{
    isKeyPressed1 = x;
}
void Map::setIsKeyPressed2(bool x)
{
    isKeyPressed2 = x;
}
void Map::setPressedKey1(int x)
{
    pressedKey1 = x;
}
void Map::setPressedKey2(int x)
{
    pressedKey2 = x;
}
void Map::setTarget1(int x,int y)
{
    target1 = QPoint(x,y);
}
void Map::setTarget2(int x,int y)
{
    target2 = QPoint(x,y);
}
void Map::setTarget3(int x,int y)
{
    target3 = QPoint(x,y);
}
void Map::setTarget4(int x,int y)
{
    target4 = QPoint(x,y);
}
void Map::setType1Count(int x)
{
    type1Count = x;
}
void Map::setType2Count(int x)
{
    type2Count = x;
}
void Map::setType3Count(int x)
{
    type3Count = x;
}
void Map::setClickPoint(int x,int y)
{
    clickPoint  = new QPoint(x,y);
}
void Map::setCouple1(int x,int y)
{
    couple1 = QPoint(x,y);
}
void Map::setCouple2(int x,int y)
{
    couple2 = QPoint(x,y);
}
void Map::setShowHint(bool x)
{
    showHint = x;
}
void Map::setRemainTime(int x)
{
    timerHRemain = x;
}

QPixmap Map::jsonValueToPixmap(const QJsonValue &value) const{
    QByteArray byteArray = QByteArray::fromBase64(value.toString().toLatin1());
    QPixmap pixmap;
    pixmap.loadFromData(byteArray, "PNG");
    return pixmap;
}
void Map::reload()
{
    timerHRemain--;
    qDebug()<<timerHRemain;
    if(timerHRemain==1){
        showHint = false;
        delete timer3;
        timer3 = NULL;
        timerHRemain = 10;
        emit markHint(showHint);
    }
    update();
}

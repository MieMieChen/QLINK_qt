#include "simpletest.h"
#include "mainwindow.h"
int size = num;
SimpleTest::SimpleTest() {

}


//应该要有各种各样的测试可能性，或许需要再加几个测试用例捏
void SimpleTest::testcase()
{
    qDebug()<<"test";
    MAP_SIZEX = MAP_SIZE;
    MAP_SIZEY = MAP_SIZE;
    Map *map = new Map(nullptr,0); //一个的格子大小是10
    map->blocks  = new Blocks**[MAP_SIZEX];
    map->deadBlock = new std::vector<QPoint>;
    for(int i = 0;i<MAP_SIZEX;i++){
        map->blocks[i]=new Blocks*[MAP_SIZEY];
    }
    for(int i = 0;i<MAP_SIZEX;i++){
        map->blocks[0][i] = new Blocks(map,true,0);
        map->blocks[0][i]->setPosition(0,i*size);
        map->deadBlock->push_back(QPoint(0,i*size));
    }
    for(int i = 1;i<MAP_SIZEX;i++){
        map->blocks[i][0] = new Blocks(map,true,0);
        map->blocks[i][0]->setPosition(i*size,0);
        map->blocks[MAP_SIZEX-1][i] = new Blocks(map,true,0);
        map->blocks[MAP_SIZEX-1][i]->setPosition(MAP_SIZEX-1,i*size);
        map->deadBlock->push_back(QPoint(i*size,0));
        map->deadBlock->push_back(QPoint(40,i*size));
    }
    for(int i = 1;i<MAP_SIZEY-1;i++){
        map->blocks[i][MAP_SIZEY-1] = new Blocks(map,true,0);
        map->blocks[i][MAP_SIZEY-1]->setPosition(i*size,(MAP_SIZEY-1)*size);
        map->deadBlock->push_back(QPoint(i*size,40));
    }
    map->blocks[1][1] = new Blocks(map,false,1);
    map->blocks[1][3] = new Blocks(map,false,1);
    map->blocks[2][2] = new Blocks(map,false,0);
    map->blocks[1][2] = new Blocks(map,false,0);
    map->blocks[2][3] = new Blocks(map,false,2);
    map->blocks[3][1] = new Blocks(map,false,2);
    map->blocks[3][3] = new Blocks(map,false,1);
    map->blocks[2][1] = new Blocks(map,false,2);
    map->blocks[3][2] = new Blocks(map,false,0);
    bool test1 = map->tripleLineLink(1,1,1,3,0); //水平一条线
    qDebug()<<"test1"<<test1;
    bool test2 = map->tripleLineLink(2,1,3,1,0);//向右开口
    qDebug()<<"test2"<<test2;
    bool test3 = map->tripleLineLink(1,1,3,3,0);//正Z
    qDebug()<<"test3"<<test3;
    bool test4 = map->tripleLineLink(3,1,2,3,0);//反Z
    qDebug()<<"test4"<<test4;
    bool test5 = map->tripleLineLink(1,3,3,3,0);//向左开口
    qDebug()<<"test5"<<test5;
    delete map->blocks[2][3];
    map->blocks[2][3] = new Blocks(map,false,0);
    bool test6 = map->tripleLineLink(1,3,3,3,0);//竖直一条线
    qDebug()<<"test6"<<test6;
    delete map->blocks[1][2];
    delete map->blocks[3][1];
    delete map->blocks[2][3];
    delete map->blocks[3][2];
    delete map->blocks[3][3];
    map->blocks[1][2] = new Blocks(map,false,2);
    map->blocks[2][3] = new Blocks(map,false,1);
    map->blocks[3][2] = new Blocks(map,false,1);
    map->blocks[3][1] = new Blocks(map,false,1);
    map->blocks[3][3] = new Blocks(map,false,2);
    bool test7 = map->tripleLineLink(1,2,2,1,0);//左上开口一角
    qDebug()<<"test7"<<test7;
    bool test8 = map->tripleLineLink(2,3,3,2,0);//右下开口一角
    qDebug()<<"test8"<<test8;
    bool test9 = map->tripleLineLink(1,1,1,3,0);//向下开口
    qDebug()<<"test9"<<test9;
    delete map->blocks[3][1];
    map->blocks[3][1] = new Blocks(map,false,2);
    bool test10 = map->tripleLineLink(3,1,3,3,0);//向上开口
    qDebug()<<"test10"<<test10;
    QVERIFY(test1 == true);
    QVERIFY(test2 == true);
    QVERIFY(test3 == true);
    QVERIFY(test4 == true);
    QVERIFY(test5 == true);
    QVERIFY(test6 == true);
    QVERIFY(test7 == true);
    QVERIFY(test8 == true);
    QVERIFY(test9 == true);
    QVERIFY(test10 == true);
}
//测试的时候把下面的部分解除注释
// QTEST_MAIN(SimpleTest)
//tips:QTEST_MAIN会封装一个main函数，并且调用指定的TestString这个测试类中的测试函数。使用QTEST_MAIN的时候不能有其他main函数，会冲突

#include "constNum.h"
//一定要设全局变量，，，改一个数真的要死了，，，，，
int MAP_SIZEX,MAP_SIZEY;
//测试的时候把下面四行注释掉，还要记得注释掉main函数
int WIDTH= MAP_SIZEX*num;  // 定义并初始化全局变量：map的宽度 游戏的时候的大小
int HEIGHT = MAP_SIZEY*num; // 定义并初始化全局变量：map的长度
int MAP_SIZE = 10; //测试的时候为5*5，游戏的时候为10*10
int BLOCKS_SIZE = 8;//测试的时候为3*3，游戏的时候为8*8

int num = 64;//每一个block的边长
int GAMETIME = 60;//计时的时间

//测试的时候解除下面的注释：
// int WIDTH= 50;  // 定义并初始化全局变量：map的宽度 测试时候的大小
// int HEIGHT = 50; // 定义并初始化全局变量：map的长度
// int MAP_SIZE = 5; //测试的时候为5*5，游戏的时候为10*10
// int BLOCKS_SIZE = 3;//测试的时候为3*3，游戏的时候为8*8

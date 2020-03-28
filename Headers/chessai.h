#ifndef CHESSAI_H
#define CHESSAI_H

#include <QDebug>
#include <QPoint>
#include <QMap>

#define C_NONE 0//棋子：黑子,白子,无子
#define C_BLACK 1
#define C_WHITE 2

#define RIGHT 0//方向：向右、向上、右上、左上
#define UP 1
#define UPRIGHT 2
#define UPLEFT 3

//各个棋型的代号 /权重
#define OTHER 0//其他棋型
#define WIN 1//1000000,白赢
#define LOSE 2//-10000000
#define FLEX4 3//50000,白活4
#define flex4 4//-100000
#define BLOCK4 5//400
#define block4 6//-100000
#define FLEX3 7//400
#define flex3 8//-8000
#define BLOCK3 9//20
#define block3 10//-40
#define FLEX2 11//20
#define flex2 12//-40
#define BLOCK2 13//1
#define block2 14//-2
#define FLEX1 15//1
#define flex1 16//-2

enum gameMode{PLAYER,AI};
enum gameStatus{UNDERWAY,FINISH};
enum gameTurn{T_BLACK,T_WHITE};//轮到谁下
enum gameResult{R_BLACK,R_WHITE,R_DRAW};//黑棋赢，白棋赢，和棋

struct EVALUATION{
    int score;
    gameResult result;
    int STAT[8];//储存部分棋形的个数,下标WIN=1为白连5,LOSE=2为黑连5,FLEX4=3为白活4,BLOCK4=5为白冲4,FLEX3=7为白活3
};
struct POINTS{//最佳落子位置,[0]分数最高,[19]分数最低
    QPoint pos[20];
    int score[20];//此处落子的局势分数
};
struct DECISION{
    QPoint pos;//位置
    int eval;//对分数的评估
};

class chessAi
{
public:
    int chesses[15][15];//棋盘
    DECISION decision;//储存极大极小搜索得到的要走的位置

     int nodeNum=0;

private:
    int tuple6type[4][4][4][4][4][4];//棋型辨识数组,0无子,1黑子,2白子,3边界
    POINTS points;//最佳落子位置

public://贪心算法部分
    chessAi();
    int calcOnePosGreedy(int board[15][15],int row,int col,int C_ME);//计算某一个位置对于me的分数(贪心)
    int tupleScoreGreedy(int black,int white,int C_ME);//计算黑白子给定的五元组对于me的分数(贪心算法)
    QPoint getXY(int row,int col,int dir,int rel);//对于给定点、方向和偏移量，得到实际坐标
    bool checkBound(int x,int y);//检查边界
    QPoint findBestMoveGreedy(int C_ME);//贪心算法只看一步,效果还不错,但是目光短浅

public://博弈树搜索部分
    void init_tuple6type();//对棋型判断数组赋初值
    POINTS seekPoints(int board[15][15]);//生成对于白子的最佳20个落子位置及分数
    void copyBoard(int A[15][15],int B[15][15]);//将A棋盘复制到B棋盘
    void reverseBoard(int A[15][15],int B[15][15]);//将A棋盘黑白子颠倒结果传给B棋盘
    EVALUATION evaluate(int board[15][15],bool needPrint=false);//对棋局board的黑子的局势估值函数,还可以判断输赢
    int analyse(int board[15][15],int depth,int alpha,int beta);//博弈树极大极小搜索加ab剪枝

    bool analyse_kill(int board[15][15],int depth);//计算杀棋,若找到杀棋则返回true
    QList<QPoint> seek_kill_points(int board[15][15]);//找白棋的连5,活4,冲4,活3的杀棋位置

};

#endif // CHESSAI_H

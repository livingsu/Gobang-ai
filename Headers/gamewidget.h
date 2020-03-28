#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMap>
#include <QRandomGenerator>
#include <QTimer>
#include <QTime>
#include <sys/time.h>
#include <QDebug>
#include <iostream>
#include "chessai.h"
using namespace std;


namespace Ui {
class gameWidget;
}

class gameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit gameWidget(QWidget *parent = nullptr);
    ~gameWidget()override;
    void initializeGame();
    void setGameMode(gameMode m){mode=m;}
    bool chessOneByPlayer();
    void oneChessMove(int row,int col);//给出要走子的地址，走一步
    bool isDeadGame();//判断是否是僵局
    bool isLegalMove(int row,int col);//判断下子位置是否合法

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;//实际落子

signals:
    void returnSignal();

public slots:
    void returnPush();
    void chessOneByAi();

private:
    Ui::gameWidget *ui;
    QPoint chessboard[15][15];//第一个下标是列，第二个是行
    chessAi ai;//棋盘放在ai当中
    int cursorRow;//光标位置
    int cursorCol;

    gameMode mode;
    gameTurn turn;
    gameStatus status;

    int score_black;
    int score_write;

};

#endif // GAMEWIDGET_H

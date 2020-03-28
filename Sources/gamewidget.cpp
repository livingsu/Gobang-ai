#include "gamewidget.h"
#include "ui_gamewidget.h"

gameWidget::gameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameWidget)
{
    ui->setupUi(this);

    for(int i=0;i<15;++i){//棋盘左上角点为(20,20),每格间距为30
        for(int j=0;j<15;++j){
            chessboard[i][j].setX(20+30*i);
            chessboard[i][j].setY(20+30*j);
        }
    }
    connect(this->ui->btn_return,SIGNAL(clicked(bool)),this,SLOT(returnPush()));

    setMouseTracking(true);//不用点击鼠标也一直追踪
    initializeGame();

}
gameWidget::~gameWidget()
{
    delete ui;
}
void gameWidget::returnPush(){
    emit returnSignal();
    this->hide();
    initializeGame();
}

void gameWidget::initializeGame(){
    qDebug()<<"游戏重新初始化";
    if(mode==PLAYER)qDebug()<<"玩家模式";
    else qDebug()<<"人机模式";

    for(int i=0;i<15;++i)
        for(int j=0;j<15;++j)
            ai.chesses[i][j]=C_NONE;

    status=UNDERWAY;
    turn=T_BLACK;
    cursorRow=-1;
    cursorCol=-1;
}

void gameWidget::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //画棋盘
    painter.setPen(Qt::black);
    for(int i=0;i<15;++i){
        painter.drawLine(chessboard[0][i],chessboard[14][i]);
        painter.drawLine(chessboard[i][0],chessboard[i][14]);
    }
    if(turn==T_BLACK)painter.setBrush(Qt::black);
    else painter.setBrush(Qt::white);
    //画鼠标光标
    if(cursorRow!=-1&&cursorCol!=-1){
        //8为光标边长
        QRect rec(chessboard[cursorCol][cursorRow].x()-8/2,chessboard[cursorCol][cursorRow].y()-8/2,8,8);
        painter.drawRect(rec);
    }

    //画棋子
    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            if(ai.chesses[i][j]!=C_NONE){
                if(ai.chesses[i][j]==C_BLACK)painter.setBrush(Qt::black);
                else painter.setBrush(Qt::white);
                painter.drawEllipse(chessboard[j][i].x()-20/2,chessboard[j][i].y()-20/2,20,20);

            }
        }
    }
}

void gameWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->x()>=5&&event->x()<=455&&event->y()>=5&&event->y()<=455){//5=20-15,455=20+14*30+15
        setCursor(Qt::BlankCursor);
        for(int i=0;i<15;++i)
            for(int j=0;j<15;++j){
                float x=event->x(),y=event->y();
                //判断鼠标落在哪一个点附近(正方形范围)
                if((x>=(chessboard[i][j].x()-15))&&(x<(chessboard[i][j].x()+15))&&
                   (y>=(chessboard[i][j].y()-15))&&(y<(chessboard[i][j].y()+15))){
                    cursorRow=j;
                    cursorCol=i;
                    if(ai.chesses[cursorRow][cursorCol]!=C_NONE)
                        setCursor(Qt::ForbiddenCursor);

                    //展示图标坐标
                    QString str="坐标:";
                    str+=QString::number(j);
                    str+=",";
                    str+=QString::number(i);
                    if(turn==T_BLACK)ui->lb_black_position->setText(str);
                    else ui->lb_white_position->setText(str);
                    break;
                }
            }
    }
    else setCursor(Qt::ArrowCursor);
    update();
}

void gameWidget::mouseReleaseEvent(QMouseEvent *event){
    if(mode==PLAYER){
        if(chessOneByPlayer()){
            if(status==FINISH)initializeGame();
        }
    }else{
        if(chessOneByPlayer()){
            if(status==UNDERWAY){
                chessOneByAi();
                if(status==FINISH)initializeGame();
            }
            else initializeGame();
        }
    }
}

bool gameWidget::chessOneByPlayer(){
    if(ai.chesses[cursorRow][cursorCol]==C_NONE){
        qDebug()<<"player chess";
        oneChessMove(cursorRow,cursorCol);

       // qDebug()<<"局势得分:"<<ai.evaluate(ai.chesses).score;

        return true;
    }
    return false;
}

bool gameWidget::isDeadGame(){
    int chessNum=0;
    for(int i=0;i<16;++i)
        for(int j=0;j<16;++j)
            if(ai.chesses[i][j]!=C_NONE)chessNum++;
    if(chessNum==16*16)return true;
    else return false;
}
bool gameWidget::isLegalMove(int row, int col){
    if(ai.chesses[row][col]==C_NONE)return true;
    else return false;
}

void gameWidget::oneChessMove(int row, int col){
    qDebug()<<row<<","<<col;

    if(turn==T_BLACK){
        turn=T_WHITE;
        ai.chesses[row][col]=C_BLACK;
    }
    else{
        turn=T_BLACK;
        ai.chesses[row][col]=C_WHITE;
    }

    gameResult result=ai.evaluate(ai.chesses).result;

    QMessageBox msg;
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Yes);
    if(result!=R_DRAW){
        status=FINISH;
        if(result==R_BLACK){
            qDebug()<<"黑棋赢";
            msg.setText("黑棋赢");
            score_black++;
        }
        else {
            qDebug()<<"白棋赢";
            msg.setText("白棋赢");
            score_write++;
        }
        msg.exec();

        ui->lcd_black->display(score_black);
        ui->lcd_write->display(score_write);
    }
    else if(isDeadGame()){
        status=FINISH;
        msg.setText("平局");
        msg.exec();
    }
    update();
}

void gameWidget::chessOneByAi(){
    qDebug()<<"ai chess";

    //QPoint p=ai.findBestMove(T_BLACK);

    struct timeval tpstart,tpend;
    float timeuse;//ai计算耗时
    gettimeofday(&tpstart,NULL);

    //QPoint p=ai.findBestMoveGreedy(C_BLACK);
    ai.nodeNum=0;

    if(!ai.analyse_kill(ai.chesses,16)){
        qDebug()<<"没找到杀棋";
       ai.analyse(ai.chesses,6,-INT_MAX,INT_MAX);

    }else{
        qDebug()<<"找到了杀棋";
    }

    QPoint p=ai.decision.pos;

    qDebug()<<"ai落子:"<<p.x()<<","<<p.y();

    if(isLegalMove(p.x(),p.y()))
        oneChessMove(p.x(),p.y());
    else qDebug()<<"ai下标不合法！";

    qDebug()<<"ai所求局势得分:"<<ai.evaluate(ai.chesses).score;

    gettimeofday(&tpend,NULL);
    timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000000.0;
    qDebug()<<timeuse<<"s";

    QString text="ai计算耗时:"+QString::number(timeuse)+"s";
    this->ui->lb_timeuse->setText(text);

    text="ai叶结点数:"+QString::number(ai.nodeNum);
    this->ui->lb_nodeNum->setText(text);

    text="ai局面估分:"+QString::number(ai.evaluate(ai.chesses).score);
    this->ui->lb_eval->setText(text);
}

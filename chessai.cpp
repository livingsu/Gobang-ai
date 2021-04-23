#include "chessai.h"

chessAi::chessAi()
{
    init_tuple6type();
    qDebug()<<"初始化ai";
}

bool chessAi::checkBound(int x,int y){
    if(x>=0&&x<15&&y>=0&&y<15)return true;
    else return false;
}
QPoint chessAi::getXY(int row, int col, int dir, int rel){
    QPoint p;
    if(dir==RIGHT){
        p.setX(row);
        p.setY(col+rel);
    }else if(dir==UP){
        p.setX(row-rel);
        p.setY(col);
    }else if(dir==UPRIGHT){
        p.setX(row-rel);
        p.setY(col+rel);
    }else if(dir==UPLEFT){
        p.setX(row-rel);
        p.setY(col-rel);
    }
    return p;
}
int chessAi::calcOnePosGreedy(int board[15][15],int row, int col, int C_ME){
    int sum=0;
    for(int i=0;i<4;++i){//四个方向
        for(int j=0;j<5;++j){//每个方向上最多5个五元组
            QPoint start=getXY(row,col,RIGHT+i,j-4);//五元组顶点位置
            QPoint end=getXY(start.x(),start.y(),RIGHT+i,4);//五元组最远位置
            if(checkBound(start.x(),start.y())&&checkBound(end.x(),end.y())){//若五元组下标均合法
                int blackChess=0;
                int whiteChess=0;
                for(int k=0;k<5;++k){//对五元组5个位置考察
                    QPoint tmp=getXY(start.x(),start.y(),RIGHT+i,k);
                    if(board[tmp.x()][tmp.y()]==C_BLACK)blackChess++;
                    if(board[tmp.x()][tmp.y()]==C_WHITE)whiteChess++;
                }
                sum+=tupleScoreGreedy(blackChess,whiteChess,C_ME);
            }
        }
    }
    return sum;
}
int chessAi::tupleScoreGreedy(int black, int white, int C_ME){
    //连5
    if(C_ME==C_BLACK&&black==5)return 9999999;
    if(C_ME==C_WHITE&&white==5)return 9999999;
    //全空
    if(black==0&&white==0)return 7;
    //polluted
    else if(black>=1&&white>=1)return 0;
    else if(C_ME==C_BLACK){
        if(black==1&&white==0)return 35;
        else if(black==2&&white==0)return 800;
        else if(black==3&&white==0)return 15000;
        else if(black==4&&white==0)return 800000;
        else if(black==0&&white==1)return 15;
        else if(black==0&&white==2)return 400;
        else if(black==0&&white==3)return 1800;
        else return 100000;
    }else{
        if(black==1&&white==0)return 15;
        else if(black==2&&white==0)return 400;
        else if(black==3&&white==0)return 1800;
        else if(black==4&&white==0)return 100000;
        else if(black==0&&white==1)return 35;
        else if(black==0&&white==2)return 800;
        else if(black==0&&white==3)return 15000;
        else return 800000;
    }
}
QPoint chessAi::findBestMoveGreedy(int C_ME){
    int bestScore=0;
    int bestRow=0,bestCol=0;
    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            if(chesses[i][j]==C_NONE){//空的位置
                int score=calcOnePosGreedy(chesses,i,j,C_ME);
                if(bestScore<score){
                    bestScore=score;
                    bestRow=i;
                    bestCol=j;
                }
            }
        }
    }
    QPoint p(bestRow,bestCol);
    return p;
}
void chessAi::init_tuple6type(){
    memset(tuple6type,0,sizeof (tuple6type));//全部设为0
    //白连5,ai赢
    tuple6type[2][2][2][2][2][2]=WIN;
    tuple6type[2][2][2][2][2][0]=WIN;
    tuple6type[0][2][2][2][2][2]=WIN;
    tuple6type[2][2][2][2][2][1]=WIN;
    tuple6type[1][2][2][2][2][2]=WIN;
    tuple6type[3][2][2][2][2][2]=WIN;//边界考虑
    tuple6type[2][2][2][2][2][3]=WIN;
    //黑连5,ai输
    tuple6type[1][1][1][1][1][1]=LOSE;
    tuple6type[1][1][1][1][1][0]=LOSE;
    tuple6type[0][1][1][1][1][1]=LOSE;
    tuple6type[1][1][1][1][1][2]=LOSE;
    tuple6type[2][1][1][1][1][1]=LOSE;
    tuple6type[3][1][1][1][1][1]=LOSE;
    tuple6type[1][1][1][1][1][3]=LOSE;
    //白活4
    tuple6type[0][2][2][2][2][0]=FLEX4;
    //黑活4
    tuple6type[0][1][1][1][1][0]=flex4;
    //白活3
    tuple6type[0][2][2][2][0][0]=FLEX3;
    tuple6type[0][0][2][2][2][0]=FLEX3;
    tuple6type[0][2][0][2][2][0]=FLEX3;
    tuple6type[0][2][2][0][2][0]=FLEX3;
    //黑活3
    tuple6type[0][1][1][1][0][0]=flex3;
    tuple6type[0][0][1][1][1][0]=flex3;
    tuple6type[0][1][0][1][1][0]=flex3;
    tuple6type[0][1][1][0][1][0]=flex3;
    //白活2
    tuple6type[0][2][2][0][0][0]=FLEX2;
    tuple6type[0][2][0][2][0][0]=FLEX2;
    tuple6type[0][2][0][0][2][0]=FLEX2;
    tuple6type[0][0][2][2][0][0]=FLEX2;
    tuple6type[0][0][2][0][2][0]=FLEX2;
    tuple6type[0][0][0][2][2][0]=FLEX2;
    //黑活2
    tuple6type[0][1][1][0][0][0]=flex2;
    tuple6type[0][1][0][1][0][0]=flex2;
    tuple6type[0][1][0][0][1][0]=flex2;
    tuple6type[0][0][1][1][0][0]=flex2;
    tuple6type[0][0][1][0][1][0]=flex2;
    tuple6type[0][0][0][1][1][0]=flex2;
    //白活1
    tuple6type[0][2][0][0][0][0]=FLEX1;
    tuple6type[0][0][2][0][0][0]=FLEX1;
    tuple6type[0][0][0][2][0][0]=FLEX1;
    tuple6type[0][0][0][0][2][0]=FLEX1;
    //黑活1
    tuple6type[0][1][0][0][0][0]=flex1;
    tuple6type[0][0][1][0][0][0]=flex1;
    tuple6type[0][0][0][1][0][0]=flex1;
    tuple6type[0][0][0][0][1][0]=flex1;

    int p1,p2,p3,p4,p5,p6,x,y,ix,iy;//x:左5中黑个数,y:左5中白个数,ix:右5中黑个数,iy:右5中白个数
    for(p1=0;p1<4;++p1){
        for(p2=0;p2<3;++p2){
            for(p3=0;p3<3;++p3){
                for(p4=0;p4<3;++p4){
                    for(p5=0;p5<3;++p5){
                        for(p6=0;p6<4;++p6){
                            x=y=ix=iy=0;

                            if(p1==1)x++;
                            else if(p1==2)y++;

                            if(p2==1){x++;ix++;}
                            else if(p2==2){y++;iy++;}

                            if(p3==1){x++;ix++;}
                            else if(p3==2){y++;iy++;}

                            if(p4==1){x++;ix++;}
                            else if(p4==2){y++;iy++;}

                            if(p5==1){x++;ix++;}
                            else if(p5==2){y++;iy++;}

                            if(p6==1)ix++;
                            else if(p6==2)iy++;

                            if(p1==3||p6==3){//有边界
                                if(p1==3&&p6!=3){//左边界
                                    //白冲4
                                    if(ix==0&&iy==4){//若右边有空位是活4也没关系，因为活4权重远大于冲4，再加上冲4权重变化可以不计
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK4;
                                    }
                                    //黑冲4
                                    if(ix==4&&iy==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block4;
                                    }
                                    //白眠3
                                    if(ix==0&&iy==3){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK3;
                                    }
                                    //黑眠3
                                    if(ix==3&&iy==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block3;
                                    }
                                    //白眠2
                                    if(ix==0&&iy==2){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK2;
                                    }
                                    //黑眠2
                                    if(ix==2&&iy==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block2;
                                    }                                  
                                }else if(p6==3&&p1!=3){//右边界
                                    //白冲4
                                    if(x==0&&y==4){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK4;
                                    }
                                    //黑冲4
                                    if(x==4&&y==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block4;
                                    }      
                                    //黑眠3
                                    if(x==3&&y==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK3;
                                    }
                                    //白眠3
                                    if(x==0&&y==3){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block3;
                                    }
                                    //黑眠2
                                    if(x==2&&y==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK2;
                                    }
                                    //白眠2
                                    if(x==0&&y==2){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block2;
                                    }
                                }
                            }else{//无边界
                                //白冲4
                                if((x==0&&y==4)||(ix==0&&iy==4)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK4;
                                }
                                //黑冲4
                                if((x==4&&y==0)||(ix==4&&iy==0)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=block4;
                                }
                                //白眠3
                                if((x==0&&y==3)||(ix==0&&iy==3)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK3;
                                }
                                //黑眠3
                                if((x==3&&y==0)||(ix==3&&iy==0)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=block3;
                                }
                                //白眠2
                                if((x==0&&y==2)||(ix==0&&iy==2)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK2;
                                }
                                //黑眠2
                                if((x==2&&y==0)||(ix==2&&iy==0)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=block2;
                                }                               
                            }
                        }
                    }
                }
            }
        }
    }
}

EVALUATION chessAi::evaluate(int board[15][15],bool needPrint){
    //各棋型权重
    int weight[17]={0,1000000,-10000000,50000,-100000,400,-100000,400,-8000,20,-50,20,-50,1,-3,1,-3};

    int i,j,type;
    int stat[4][17];//统计4个方向上每种棋型的个数
    memset(stat,0,sizeof(stat));


    int A[17][17];//包括边界的虚拟大棋盘,board[i][j]=A[i-1][j-1],3表示边界
    for(int i=0;i<17;++i)A[i][0]=3;
    for(int i=0;i<17;++i)A[i][16]=3;
    for(int j=0;j<17;++j)A[0][j]=3;
    for(int j=0;j<17;++j)A[16][j]=3;
    for(int i=0;i<15;++i)
        for(int j=0;j<15;++j)
            A[i+1][j+1]=board[i][j];

    //判断横向棋型
    for(i=1;i<=15;++i){
        for(j=0;j<12;++j){
            type=tuple6type[A[i][j]][A[i][j+1]][A[i][j+2]][A[i][j+3]][A[i][j+4]][A[i][j+5]];
            stat[0][type]++;
        }
    }
    //判断竖向棋型
    for(j=1;j<=15;++j){
        for(i=0;i<12;++i){
            type=tuple6type[A[i][j]][A[i+1][j]][A[i+2][j]][A[i+3][j]][A[i+4][j]][A[i+5][j]];
            stat[1][type]++;
        }
    }
    //判断左上至右下棋型
    for(i=0;i<12;++i){
        for(j=0;j<12;++j){
            type=tuple6type[A[i][j]][A[i+1][j+1]][A[i+2][j+2]][A[i+3][j+3]][A[i+4][j+4]][A[i+5][j+5]];
            stat[2][type]++;
        }
    }
    //判断右上至左下棋型
    for(i=0;i<12;++i){
        for(j=5;j<17;++j){
            type=tuple6type[A[i][j]][A[i+1][j-1]][A[i+2][j-2]][A[i+3][j-3]][A[i+4][j-4]][A[i+5][j-5]];
            stat[3][type]++;
        }
    }

    EVALUATION eval;
    memset(eval.STAT,0,sizeof (eval.STAT));

    int score=0;
    for(i=1;i<17;++i){
        score+=(stat[0][i]+stat[1][i]+stat[2][i]+stat[3][i])*weight[i];//初步计分

        int count=stat[0][i]+stat[1][i]+stat[2][i]+stat[3][i];//统计所有方向上部分棋型的个数
        if(i==WIN)eval.STAT[WIN]=count;
        else if(i==LOSE)eval.STAT[LOSE]=count;
        else if(i==FLEX4)eval.STAT[FLEX4]=count;
        else if(i==BLOCK4)eval.STAT[BLOCK4]=count;
        else if(i==FLEX3)eval.STAT[FLEX3]=count;
    }

   /* if(needPrint==true){
        qDebug()<<"局面棋型判断";
        for(int i=0;i<17;++i)
            qDebug()<<i<<":"<<STAT[i];
    }
*/
    eval.result=R_DRAW;

   //白赢
    if(eval.STAT[WIN]>0)eval.result=R_WHITE;
    //黑赢
    else if(eval.STAT[LOSE]>0)eval.result=R_BLACK;

    eval.score=score;
    return eval;
}

POINTS chessAi::seekPoints(int board[15][15]){
    bool B[15][15];//标记数组
    int worth[15][15];
    POINTS best_points;

    memset(B,0,sizeof (B));
    for(int i=0;i<15;++i){//每个非空点附近8个方向延伸3个深度,若不越界则标记为可走
        for(int j=0;j<15;++j){
            if(board[i][j]!=C_NONE){
                for(int k=-3;k<=3;++k){
                    if(i+k>=0&&i+k<15){
                        B[i+k][j]=true;
                        if(j+k>=0&&j+k<15)B[i+k][j+k]=true;
                        if(j-k>=0&&j-k<15)B[i+k][j-k]=true;
                    }
                    if(j+k>=0&&j+k<15)B[i][j+k]=true;
                }
            }
        }
    }

    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            worth[i][j]=-INT_MAX;
            if(board[i][j]==C_NONE&&B[i][j]==true){
                //board[i][j]=C_BLACK;
                worth[i][j]=calcOnePosGreedy(board,i,j,C_WHITE);
                //worth[i][j]=evaluate(board).score;
                //board[i][j]=C_NONE;
            }
        }
    }

    int w;
    for(int k=0;k<20;++k){
        w=-INT_MAX;
        for(int i=0;i<15;++i){
            for(int j=0;j<15;++j){
                if(worth[i][j]>w){
                    w=worth[i][j];
                    QPoint tmp(i,j);
                    best_points.pos[k]=tmp;
                }
            }
        }

        int x=best_points.pos[k].x(),y=best_points.pos[k].y();
        board[x][y]=C_WHITE;
        best_points.score[k]=evaluate(board).score;
        board[x][y]=C_NONE;

        worth[best_points.pos[k].x()][best_points.pos[k].y()]=-INT_MAX;//清除掉上一点,计算下一点的位置和分数
    }
    return best_points;
}

void chessAi::copyBoard(int (*A)[15], int (*B)[15]){
    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            if(A[i][j]==C_NONE)B[i][j]=C_NONE;
            else if(A[i][j]==C_BLACK)B[i][j]=C_BLACK;
            else B[i][j]=C_WHITE;
        }
    }
}
void chessAi::reverseBoard(int (*A)[15], int (*B)[15]){
    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            if(A[i][j]==C_NONE)B[i][j]=C_NONE;
            else if(A[i][j]==C_BLACK)B[i][j]=C_WHITE;
            else B[i][j]=C_BLACK;
        }
    }
}

int chessAi::analyse(int (*board)[15], int depth,int alpha, int beta){
    EVALUATION EVAL=evaluate(board);
    if(depth==0||EVAL.result!=R_DRAW){//抵达最深层/如果模拟落子可以分出输赢，那么直接返回结果，不需要再搜索
        nodeNum+=1;
        if(depth==0){
            POINTS P;
            P=seekPoints(board);

            return P.score[0];//返回最佳位置对应的最高分
        }else return EVAL.score;
    }else if(depth%2==0){//max层,我方(白)决策
        //qDebug()<<"白方决策！";

        POINTS P=seekPoints(board);

        for(int i=0;i<10;++i){
            //qDebug()<<"白方模拟下"<<P.pos[i].x()<<","<<P.pos[i].y();
            int sameBoard[15][15];
            copyBoard(board,sameBoard);

            sameBoard[P.pos[i].x()][P.pos[i].y()]=C_WHITE;//模拟己方落子,不能用board,否则可能改变board的信息
            int a=analyse(sameBoard,depth-1,alpha,beta);
            if(a>alpha){
                alpha=a;
                if(depth==6){
                    qDebug()<<"set decision:"<<P.pos[i].x()<<P.pos[i].y();

                    decision.pos.setX(P.pos[i].x());
                    decision.pos.setY(P.pos[i].y());
                    decision.eval=a;
                }
            }
            if(beta<=alpha)break;//剪枝
        }
        return alpha;
    }else{//min层,敌方(黑)决策
        //qDebug()<<"黑方决策！";

        int rBoard[15][15];
        reverseBoard(board,rBoard);
        POINTS P=seekPoints(rBoard);//找对于黑子的最佳位置,需要将棋盘不同颜色反转,因为seekPoint是求白色方的最佳位置

        for(int i=0;i<10;++i){
            //qDebug()<<"黑方模拟下"<<P.pos[i].x()<<","<<P.pos[i].y();

            int sameBoard[15][15];
            copyBoard(board,sameBoard);

            sameBoard[P.pos[i].x()][P.pos[i].y()]=C_BLACK;//模拟敌方落子
            int a=analyse(sameBoard,depth-1,alpha,beta);
            if(a<beta)beta=a;
            if(beta<=alpha)break;//剪枝
        }
        return beta;
    }
}

bool chessAi::analyse_kill(int (*board)[15], int depth){
    EVALUATION EVAL=evaluate(board);
    if(depth==0||EVAL.result!=R_DRAW){
        if(depth==0){//若抵达最深层,走一步对白棋的最好位置,若白棋还没赢则返回false
            POINTS P;
            P=seekPoints(board);            
            board[P.pos[0].x()][P.pos[0].y()]=C_WHITE;

            gameResult result=evaluate(board).result;
            if(result==R_WHITE)return true;
            else return false;
        }else if(EVAL.result==R_WHITE)return true;//找到白棋杀棋
        else return false;//白棋输
    }else if(depth%2==0){//max层,我方(白)决策
        if(depth==16||depth==14){//最开始4层选所有能走的10个点
            POINTS P=seekPoints(board);
            for(int i=0;i<10;++i){
                int sameBoard[15][15];
                copyBoard(board,sameBoard);

                sameBoard[P.pos[i].x()][P.pos[i].y()]=C_WHITE;//模拟己方落子

                if(analyse_kill(sameBoard,depth-1)){
                    if(depth==16){
                        qDebug()<<"kill set decision:"<<P.pos[i].x()<<P.pos[i].y();

                        decision.pos.setX(P.pos[i].x());
                        decision.pos.setY(P.pos[i].y());
                        decision.eval=INT_MAX;//测试一下..
                    }
                    return true;
                }
            }
            return false;
        }else{//后面只选杀棋点
            QList<QPoint> pointList=seek_kill_points(board);//产生杀棋点

            if(pointList.length()==0)return false;//没有杀棋点
            for(auto i:pointList){
                //qDebug()<<"白方模拟下"<<P.pos[i].x()<<","<<P.pos[i].y();
                int sameBoard[15][15];
                copyBoard(board,sameBoard);

                sameBoard[i.x()][i.y()]=C_WHITE;//模拟己方落子

                if(analyse_kill(sameBoard,depth-1)){
                    /*if(depth==16){
                        qDebug()<<"kill set decision:"<<i.x()<<i.y();

                        decision.pos.setX(i.x());
                        decision.pos.setY(i.y());
                        decision.eval=INT_MAX;//测试一下..
                    }*/
                    return true;
                }
            }
            return false;
        }
    }else{//min层,敌方(黑)决策,只下对自己最好的棋
        int rBoard[15][15];
        reverseBoard(board,rBoard);
        POINTS P=seekPoints(rBoard);//找对于黑子的最佳位置,需要将棋盘不同颜色反转,因为seekPoint是求白色方的最佳位置

        int sameBoard[15][15];
        copyBoard(board,sameBoard);
        sameBoard[P.pos[0].x()][P.pos[0].y()]=C_BLACK;//模拟敌方落子:只走最好的一步
        //无需剪枝
        return analyse_kill(sameBoard,depth-1);
    }
}

QList<QPoint> chessAi::seek_kill_points(int (*board)[15]){
    QList<QPoint> pointList;

    POINTS P=seekPoints(board);//一般来说,能冲4或者活3的必在评分前20的点内

    int sameBoard[15][15];
    copyBoard(board,sameBoard);

    for(int i=0;i<20;++i){
        sameBoard[P.pos[i].x()][P.pos[i].y()]=C_WHITE;//模拟落子
        if(evaluate(sameBoard).STAT[WIN]>0){//产生连5
            pointList.append(P.pos[i]);
        }else if(evaluate(sameBoard).STAT[FLEX4]>evaluate(board).STAT[FLEX4]){//产生新活4
            pointList.append(P.pos[i]);
        }else if(evaluate(sameBoard).STAT[BLOCK4]>evaluate(board).STAT[BLOCK4]){//产生新冲4
            pointList.append(P.pos[i]);
        }else if(evaluate(sameBoard).STAT[FLEX3]>evaluate(board).STAT[FLEX3]){//产生新活3
            pointList.append(P.pos[i]);
        }
        sameBoard[P.pos[i].x()][P.pos[i].y()]=C_NONE;//还原落子
    }
    return pointList;
}


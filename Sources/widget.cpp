#include "widget.h"
#include "gamewidget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    game=new gameWidget();
    game->hide();
    connect(this->ui->btn_player,SIGNAL(clicked(bool)),this,SLOT(playerButtonPush()));
    connect(this->ui->btn_ai,SIGNAL(clicked(bool)),this,SLOT(aiButtonPush()));
    connect(game,&gameWidget::returnSignal,this,&Widget::show);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::playerButtonPush(){
    this->hide();
    game->show();
    game->setGameMode(PLAYER);
}

void Widget::aiButtonPush(){
    this->hide();
    game->show();
    game->setGameMode(AI);
}

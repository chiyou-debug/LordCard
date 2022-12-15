#include "buttongroup.h"
#include "ui_buttongroup.h"

ButtonGroup::ButtonGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);

    connect(ui->startGame, &MyButton::clicked, this, &ButtonGroup::startGame);

    connect(ui->pass, &MyButton::clicked, this, &ButtonGroup::pass);
    connect(ui->playHand1, &MyButton::clicked, this, &ButtonGroup::playHand);
    connect(ui->playHand2, &MyButton::clicked, this, &ButtonGroup::playHand);

    connect(ui->giveup, &MyButton::clicked, this, [=](){
        emit betPoint(0);
    });
    connect(ui->bet1, &MyButton::clicked, this, [=](){
        emit betPoint(1);
    });
    connect(ui->bet2, &MyButton::clicked, this, [=](){
        emit betPoint(2);
    });
    connect(ui->bet3, &MyButton::clicked, this, [=](){
        emit betPoint(3);
    });
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::initButtons()
{
    // 开始游戏
    ui->startGame->setImage(":/images/start-1.png", ":/images/start-2.png");

    // 再来一局
    // 不要
    ui->pass->setImage(":/images/pass_btn-1.png", ":/images/pass_btn-2.png");
    // 出牌
    ui->playHand1->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-2.png");
    ui->playHand2->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-2.png");
    // 不抢
    ui->giveup->setImage(":/images/buqiang-1.png", ":/images/buqiang-2.png");
    // 1-3分
    ui->bet1->setImage(":/images/1fen-1.png", ":/images/1fen-2.png");
    ui->bet2->setImage(":/images/2fen-1.png", ":/images/2fen-2.png");
    ui->bet3->setImage(":/images/3fen-1.png", ":/images/3fen-2.png");

    QVector<MyButton*> btns;
    btns << ui->startGame << ui->pass << ui->playHand1
         << ui->playHand2 << ui->giveup << ui->bet1 << ui->bet2 << ui->bet3;
    for(int i=0; i<btns.size(); ++i)
    {
        btns[i]->setFixedSize(90, 45);
    }
}

void ButtonGroup::selectPanel(Panel type, int bet)
{
    ui->stackedWidget->setCurrentIndex(type);
    if(type != CallLord)
    {
        return;
    }
    if(bet == 2)
    {
        ui->bet1->setVisible(false);
        ui->bet2->setVisible(false);
    }
    else if(bet == 1)
    {
        ui->bet1->setVisible(false);
    }
    else if(bet == 0)
    {
        ui->bet1->setVisible(true);
        ui->bet2->setVisible(true);
        ui->bet1->setVisible(true);
    }
}

#include "animationwindow.h"

#include <QPainter>
#include <QDebug>
#include <QTimer>

AnimationWindow::AnimationWindow(QWidget *parent) : QWidget(parent)
{
    m_x = 0;
}

void AnimationWindow::showPlane()
{
    // 第一张
    m_x = width();
    m_image.load(":/img/plane_1.png");
    setMinimumHeight(m_image.height());
    update();

    int step = width() / 5;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
         static int dist = 0;   // 距离
         static int times = 0;  // 次数
         dist += 5;
         // 是否要停止定时器
         if(dist >= step)
         {
             // 换下一张图片
             times ++;
             dist = 0;
             QString name = QString(":/images/plane_%1.png").arg(times % 5 + 1);
             qDebug() << name;
             m_image.load(name);
         }
         if(m_x <= -110)
         {
             timer->stop();
             timer->deleteLater();
             dist = times = 0;
             emit animationOver();
         }
         m_x -= 5;
         update();
    });
    timer->start(15);
}


void AnimationWindow::showBomb()
{
    m_x = 0;
    m_index = 0;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        m_index ++;
        if(m_index > 12)
        {
            timer->stop();
            timer->deleteLater();
            m_index = 12;
            emit animationOver();
        }
        QString name = QString(":/images/bomb_%1.png").arg(m_index);
        m_image.load(name);

        update();
    });
    timer->start(60);
}

void AnimationWindow::showJokerBomb()
{
    m_x = 0;
    m_index = 0;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        m_index ++;
        if(m_index > 8)
        {
            timer->stop();
            timer->deleteLater();
            m_index = 8;
            emit animationOver();
        }
        QString name = QString(":/images/joker_bomb_%1.png").arg(m_index);
        m_image.load(name);

        update();
    });
    timer->start(60);
}

void AnimationWindow::showSequence(Type type)
{
    QString name = type == Pair ? ":/images/liandui.png" : ":/images/shunzi.png";
    m_image.load(name);
    update();
    QTimer::singleShot(2000, this, &AnimationWindow::animationOver);
}

void AnimationWindow::showBetScore(int bet)
{
    if(bet ==1)
    {
        m_image.load(":/images/score1.png");
    }
    else if(bet == 2)
    {
        m_image.load(":/images/score2.png");
    }
    else if(bet == 3)
    {
        m_image.load(":/images/score3.png");
    }
    update();
    QTimer::singleShot(2000, this, &AnimationWindow::animationOver);
}

void AnimationWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawPixmap(m_x, 0, m_image.width(), m_image.height(), m_image);
}

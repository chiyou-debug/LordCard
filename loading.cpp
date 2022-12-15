#include "loading.h"

#include <QPainter>
#include <QTimer>
#include <QDebug>
#include "gamepanel.h"

Loading::Loading(QWidget *parent) : QWidget(parent)
{
    m_bk.load(":/images/loading.png");
    QPixmap pixmap = QPixmap(":/images/progress.png");
    m_progress = pixmap;
    setFixedSize(m_bk.size());

    // 去边框
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    // 设置背景透明
    setAttribute(Qt::WA_TranslucentBackground);

    // 创建定时器
    QTimer *timer = new QTimer(this);
    int step = 5;  // 步长, 每次增加10个像素
    connect(timer, &QTimer::timeout, this, [=]()
    {
        // 步长增加
        m_progress = pixmap.copy(0, 0, m_dist, pixmap.height());
        update();
        if(m_dist >= pixmap.width())
        {
            timer->stop();
            timer->deleteLater();
            GamePanel* panel = new GamePanel;
            panel->show();
            close();
        }
        m_dist += step;
    });
    timer->start(15);
}

void Loading::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawPixmap(rect(), m_bk);
    p.drawPixmap(62, 417, m_progress.width(), m_progress.height(), m_progress);
}

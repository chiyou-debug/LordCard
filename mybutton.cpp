#include "mybutton.h"

#include <QMouseEvent>
#include <QPainter>

MyButton::MyButton(QWidget *parent) : QPushButton(parent)
{

}

void MyButton::setImage(QString normal, QString pressed)
{
    m_normal.load(normal);
    m_pressed.load(pressed);
    m_curImag = m_normal;
    update();
}

void MyButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_curImag = m_pressed;
        update();
    }

    QPushButton::mousePressEvent(e);
}

void MyButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_curImag = m_normal;
        update();
    }

    QPushButton::mouseReleaseEvent(e);
}

void MyButton::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_curImag);
}

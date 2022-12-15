#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include "player.h"

class CardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent = nullptr);
    explicit CardPanel(QPixmap& front, QPixmap& back, Card &c, QWidget *parent = nullptr);

    // 设置/获取图片
    void setImage(QPixmap& front, QPixmap& back);
    QPixmap getImage(); // 得到正面图片

    // 设置/获取卡牌的所有者
    void setOwner(Player* player);
    Player* getOwner();

    // 设置/获取卡牌属性信息
    void setCard(Card& card);
    Card getCard();

    // 设置/获取扑克牌是正面还是反面
    void setFrontSide(bool flag);
    bool isFrontSide();

    // 设置/获取扑克牌是否是选中状态
    void setSelected(bool flag);
    bool isSelected();

    // 模拟鼠标点击了窗口
    void clicked();

protected:
    // 窗口重绘事件
    void paintEvent(QPaintEvent* ev);
    // 鼠标点击事件
    void mousePressEvent(QMouseEvent* ev);

signals:
    void cardSelected(Qt::MouseButton button);

private:
    QPixmap m_front;    // 保存正面图片
    QPixmap m_back;     // 保存背面图片
    bool m_isFront;     // 记录当前是正面还是反面
    bool m_isSelect;    // 记录当前是否被选中
    Player* m_owner;    // 记录当前这张牌的所有者
    Card m_card;        // 存储当前这张牌的属性信息

};

#endif // CARDPANEL_H

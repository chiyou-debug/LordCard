#ifndef ROBOT_H
#define ROBOT_H

#include "player.h"

class Robot : public Player
{
    Q_OBJECT
public:
    using Player::Player;   // 继承构造函数
    explicit Robot(QObject *parent = nullptr);

    void prepareCallLord() override;
    void preparePlayHand() override;
    void thinkCallLord() override;
    void thinkPlayHand() override;

signals:

};

#endif // ROBOT_H

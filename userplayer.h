#ifndef USERPLAYER_H
#define USERPLAYER_H

#include "player.h"

class UserPlayer : public Player
{
    Q_OBJECT
public:
    using Player::Player;   // 继承构造函数
    explicit UserPlayer(QObject *parent = nullptr);

    virtual void prepareCallLord() override;
    virtual void preparePlayHand() override;
    virtual void thinkCallLord() override;
    virtual void thinkPlayHand() override;

signals:
    void startCountDown();
};

#endif // USERPLAYER_H

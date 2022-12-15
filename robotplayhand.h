#ifndef ROBOTPLAYHAND_H
#define ROBOTPLAYHAND_H

#include <QThread>
#include "player.h"

class RobotPlayHand : public QThread
{
    Q_OBJECT
public:
    explicit RobotPlayHand(Player* player, QObject *parent = nullptr);

protected:
    void run() override;

private:
    Player* m_player;
};

#endif // ROBOTPLAYHAND_H

#include "userplayer.h"

UserPlayer::UserPlayer(QObject *parent) : Player(parent)
{
    m_type = Player::User;
}

void UserPlayer::prepareCallLord()
{
    thinkCallLord();
}

void UserPlayer::preparePlayHand()
{
    thinkPlayHand();
}

void UserPlayer::thinkCallLord()
{

}

void UserPlayer::thinkPlayHand()
{
    emit startCountDown();
}

#include "player.h"
#include <QRandomGenerator>

Player::Player(QObject *parent) : QObject(parent)
{
    m_score = 0;
    m_isWin = false;
    m_sex = (Sex)QRandomGenerator::global()->bounded(2);
}

// 委托构造函数
Player::Player(QString name, QObject *parent) : Player(parent)
{
    setName(name);
}

void Player::setName(QString& name)
{
    m_name = name;
}

QString Player::getName()
{
    return m_name;
}

void Player::setPrevPlayer(Player *player)
{
    m_prevPlayer = player;
}

void Player::setNextPlayer(Player *player)
{
    m_nextPlayer = player;
}

Player *Player::getPrevPlayer()
{
    return m_prevPlayer;
}

Player *Player::getNextPlayer()
{
    return m_nextPlayer;
}

void Player::setScore(int score)
{
    m_score = score;
}

int Player::getScore()
{
    return m_score;
}

void Player::setRole(Role role)
{
    m_role = role;
}

Player::Role Player::getRole()
{
    return m_role;
}

void Player::setType(Type t)
{
    m_type = t;
}

Player::Type Player::getType()
{
    return m_type;
}

void Player::grabLordBet(int point)
{
    // 处理动作在GameControl类里边
    emit notifyGrabLordBet(this, point);
}

void Player::playHand(Cards &cards)
{
    m_cards.remove(cards);
    // 处理动作在GameControl类里边
    emit notifyPlayHand(this, cards);
}

void Player::clearCards()
{
    m_cards.clear();
}

void Player::storeDispatchCard(Card &card)
{
    // 添加到set容器里边
    Cards cards;
    cards.add(card);
    storeDispatchCards(cards);
}

void Player::storeDispatchCards(Cards &cs)
{
    m_cards.add(cs);
    emit notifyPickCards(this, cs);
}

Cards Player::getCards()
{
    return m_cards;
}

void Player::thinkCallLord()
{

}

void Player::thinkPlayHand()
{

}

void Player::prepareCallLord()
{

}

void Player::preparePlayHand()
{

}

Player *Player::getPendPlayer()
{
    return m_pendPlayer;
}

Cards Player::getPendCards()
{
    return m_pendCards;
}

void Player::storePendingInfo(Player *player, Cards &card)
{
     m_pendPlayer = player;
     m_pendCards = card;
}

Player::Sex Player::getSex()
{
    return m_sex;
}

void Player::setDirection(Direction direct)
{
    m_direct = direct;
}

Player::Direction Player::getDirection()
{
    return m_direct;
}

bool Player::isWin()
{
    return m_isWin;
}

void Player::setWin(bool bl)
{
    m_isWin = bl;
}

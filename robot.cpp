#include "robot.h"
#include "robotgraplord.h"
#include "robotplayhand.h"
#include "strategy.h"

Robot::Robot(QObject *parent) : Player(parent)
{
    m_type = Player::Robot;
}

void Robot::prepareCallLord()
{
    RobotGrapLord* subThread = new RobotGrapLord(this);
    subThread->start();
}

void Robot::preparePlayHand()
{
    RobotPlayHand* subThread = new RobotPlayHand(this);
    subThread->start();
}

void Robot::thinkCallLord()
{
    int weight = 0;

    Strategy st(this, m_cards);
    weight += st.getRangeCards(Card::Card_SJ, Card::Card_BJ).cardCount() * 6;
    weight += m_cards.pointCount(Card::Card_2) * 3;

    QVector<Cards> optSeq = st.pickOptimalSeqSingles();
    weight += optSeq.size() * 5;

    Cards left = m_cards;
    left.remove(optSeq);
    Strategy stLeft(this, left);

    QVector<Cards> bombs = stLeft.findCardsByCount(4);
    weight += bombs.size() * 5;

    QVector<Cards> triples = stLeft.findCardsByCount(3);
    weight += triples.size() * 4;

    QVector<Cards> pairs = stLeft.findCardsByCount(2);
    weight += pairs.size() * 1;

    if (weight >= 22)
    {
        grabLordBet(3);
    }
    else if (weight < 22 && weight >= 18)
    {
        grabLordBet(2);
    }
    else if (weight > 18 && weight >= 10)
    {
        grabLordBet(1);
    }
    else
    {
        grabLordBet(0);
    }
}

void Robot::thinkPlayHand()
{
    Strategy strategy(this, m_cards);
    Cards cards = strategy.makeStrategy();
    playHand(cards);
}

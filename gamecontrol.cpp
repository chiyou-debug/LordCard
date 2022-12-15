#include "gamecontrol.h"
#include "playhand.h"

#include <QTimer>
#include <QDebug>

GameControl::GameControl(QObject *parent) : QObject(parent)
{
    m_pendPlayer = nullptr;
    m_pendCards = Cards();
    m_currBet = 0;
}

void GameControl::initAllCards()
{
    m_allCards.clear();
    for(int p=Card::Card_Begin+1; p<Card::Card_SJ; ++p)
    {
        for(int s=Card::Suit_Begin+1; s<Card::Suit_End; ++s)
        {
            Card c((Card::CardPoint)p, (Card::CardSuit)s);
            m_allCards.add(c);
        }
    }
    // 添加大小王
    m_allCards.add(Card(Card::Card_SJ, Card::Suit_Begin));
    m_allCards.add(Card(Card::Card_BJ, Card::Suit_Begin));
}

void GameControl::playerInit()
{
    // 实例化玩家对象 && 设置玩家名字
    m_leftRobot = new Robot("机器人A", this);
    m_rightRobot = new Robot("机器人B", this);
    m_user = new UserPlayer("我自己", this);

    // 设置玩家头像显示的位置: 左侧 or 右侧
    m_leftRobot->setDirection(Player::Left);
    m_rightRobot->setDirection(Player::Right);
    m_user->setDirection(Player::Right);

    // 设置出牌顺序 --> 逆时针, 左侧是上家, 右侧是下家
    // 左侧机器人
    m_leftRobot->setPrevPlayer(m_rightRobot);
    m_leftRobot->setNextPlayer(m_user);
    // 非机器人玩家
    m_user->setPrevPlayer(m_leftRobot);
    m_user->setNextPlayer(m_rightRobot);
    // 右侧机器人
    m_rightRobot->setPrevPlayer(m_user);
    m_rightRobot->setNextPlayer(m_leftRobot);

    // 三个玩家进行对接
    // 抢地主
    connect(m_leftRobot, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabLordBet);
    connect(m_rightRobot, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabLordBet);
    connect(m_user, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabLordBet);

    // 出牌
    connect(m_leftRobot, &UserPlayer::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_rightRobot, &UserPlayer::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_user, &UserPlayer::notifyPlayHand, this, &GameControl::onPlayHand);

    // 发牌
    connect(m_leftRobot, &UserPlayer::notifyPickCards, this, &GameControl::notifyPickCards);
    connect(m_rightRobot, &UserPlayer::notifyPickCards, this, &GameControl::notifyPickCards);
    connect(m_user, &UserPlayer::notifyPickCards, this, &GameControl::notifyPickCards);

    // 传递待处理的玩家和扑克牌数据
    connect(this, &GameControl::pendingInfo, m_leftRobot, &Robot::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_rightRobot, &Robot::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_user, &UserPlayer::storePendingInfo);

    // 指定默认的当前玩家
    m_curPlayer = m_user;
}

Robot *GameControl::getLeftRobot()
{
    return m_leftRobot;
}

Robot *GameControl::getRightRobot()
{
    return m_rightRobot;
}

UserPlayer *GameControl::getUserPlayer()
{
    return m_user;
}

Player *GameControl::getCurrentPlayer()
{
    return m_curPlayer;
}

void GameControl::setCurrentPlayer(Player *player)
{
    m_curPlayer = player;
}

Player *GameControl::getPendPlayer()
{
    return m_pendPlayer;
}

Cards GameControl::getPendCards()
{
    return m_pendCards;
}

void GameControl::clearPlayerScore()
{
    m_leftRobot->setScore(0);
    m_rightRobot->setScore(0);
    m_user->setScore(0);
}

void GameControl::resetCardsData()
{
    initAllCards();

    m_leftRobot->clearCards();
    m_rightRobot->clearCards();
    m_user->clearCards();

    // 数据重置, 还没有出牌的玩家, 更没有打出任何扑克牌
    m_pendPlayer = nullptr;
    m_pendCards.clear();
    // 传递信息, 用于其它对象的初始化
    emit pendingInfo(m_pendPlayer, m_pendCards);
}

Card GameControl::takeOneCard()
{
    return m_allCards.takeRandomCard();
}

Cards GameControl::getSurplusCards()
{
    return m_allCards;
}

void GameControl::startLordCard()
{
    emit playerStatusChanged(m_curPlayer, ThinkingForCallLord);
    m_curPlayer->prepareCallLord();
}

void GameControl::becomeLord(Player *player, int bet)
{
    m_currBet = bet;
    player->setRole(Player::Lord);
    player->getPrevPlayer()->setRole(Player::Farmer);
    player->getNextPlayer()->setRole(Player::Farmer);

    // 设置地主为当前玩家
    m_curPlayer = player;
    // 拿到剩下的三张底牌
    player->storeDispatchCards(m_allCards);
    m_allCards.clear();

    // 延时等待一下再开始出牌的过程 --> 使用定时器
    QTimer::singleShot(1000, this, [=](){
        // 游戏状态发生变化 -> 出牌状态
        emit gameStatusChanged(PlayingHand);
        // 玩家状态发生变化 -> 考虑出牌状态
        emit playerStatusChanged(m_curPlayer, ThinkingForPlayHand);
        // 玩家出牌
        m_curPlayer->preparePlayHand();
    });
}

int GameControl::getPlayerMaxBet()
{
    return m_betRecord.bet;
}

void GameControl::onGrabLordBet(Player *player, int score)
{    
    qDebug() << "current score: " << score;
    qDebug() << "m_betRecord.bet: " << m_betRecord.bet;
    // 如果分数为0, 或者 分数不比上家大则视为无效分数, 不抢地主
    if(score == 0 || m_betRecord.bet >= score)
    {
        emit notifyGrabLordBet(player, 0, false);
        qDebug() << "emit notifyGrabLordBet(player, 0, false)";
    }
    else if(score > 0 && m_betRecord.bet == 0)
    {
        // 第一次叫地主
        emit notifyGrabLordBet(player, score, true);
        qDebug() << "emit notifyGrabLordBet(player, score, true)";
    }
    else
    {
        // 不是第一次, 开始抢地主
        emit notifyGrabLordBet(player, score, false);
        qDebug() << "emit notifyGrabLordBet(player, score, false)";
    }

    // 抢地主下注为3分，抢地主结束
    if(score == 3)
    {
        // 直接当地主
        becomeLord(player, score);
        // 本轮数据清空
        m_betRecord.reset();
        return;
    }


    // 没有人叫3分, 比较分数, 如果分数高, 将该玩家和分数存储起来
    if(m_betRecord.bet < score)
    {
        m_betRecord.bet = score;        // 更新分数
        m_betRecord.player = player;    // 更新玩家
    }
    m_betRecord.times++;

    // 每个人都叫过一次地主, 做最后的处理
    if(m_betRecord.times == 3)
    {
        if(m_betRecord.bet == 0)
        {
            // 都放弃叫地主, 重新发牌
            emit gameStatusChanged(DispatchCard);
        }
        else
        {
            becomeLord(m_betRecord.player, m_betRecord.bet);
        }
        m_betRecord.reset();
        return;
    }

    // 让下一个玩家叫分
    Player* nextPlayer = player->getNextPlayer();
    m_curPlayer = nextPlayer;
    // 发信号 -> 玩家状态发送了变化(考虑叫地主)
    emit playerStatusChanged(m_curPlayer, ThinkingForCallLord);
    // 玩家叫地主
    m_curPlayer->prepareCallLord();
}

// 玩家出牌
void GameControl::onPlayHand(Player *player, Cards &cards)
{
    // 该信号的处理者是 GamePanel 类的对象
    qDebug() << m_curPlayer << player;
    emit notifyPlayHand(m_curPlayer, cards);

    if(!cards.isEmpty())
    {
        // 如果打出的牌不为空, 记录出牌人和打出的牌
        m_pendCards = cards;
        m_pendPlayer = player;
        // 发通知信号
        emit pendingInfo(player, cards);
    }

    // 如果有炸, 低分翻倍
    PlayHand::HandType type = PlayHand(cards).getType();
    if(type == PlayHand::Hand_Bomb || type == PlayHand::Hand_Bomb_Jokers)
    {
        m_currBet = m_currBet * 2;
    }

    // player已经把牌出完了, 计算得分
    if(player->getCards().isEmpty())
    {
        Player* prev = player->getPrevPlayer();
        Player* next = player->getNextPlayer();
        if(player->getRole() == Player::Lord)
        {
            // 当前玩家是地主, 并且赢了
            player->setScore(player->getScore() + 2 * m_currBet);
            prev->setScore(prev->getScore() - m_currBet);
            next->setScore(next->getScore() - m_currBet);
            player->setWin(true);
            prev->setWin(false);
            prev->setWin(false);
        }
        else
        {
            player->setWin(true);
            player->setScore(player->getScore() + m_currBet);
            if(prev->getRole() == Player::Lord)
            {
                prev->setScore(prev->getScore() - 2 * m_currBet);
                next->setScore(next->getScore() + m_currBet);
                prev->setWin(false);
                next->setWin(true);
            }
            else
            {
                prev->setScore(prev->getScore() + m_currBet);
                next->setScore(next->getScore() - 2 * m_currBet);
                prev->setWin(true);
                next->setWin(false);

            }
        }
        // 玩家状态变化了
        emit playerStatusChanged(player, GameControl::Winning);
        return;
    }
    // 出完牌, 轮到下一个玩家
    m_curPlayer = player->getNextPlayer();
    emit playerStatusChanged(m_curPlayer, GameControl::ThinkingForPlayHand);
    m_curPlayer->preparePlayHand();
}

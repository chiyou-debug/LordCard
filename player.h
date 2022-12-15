#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "cards.h"

class Player : public QObject
{
    Q_OBJECT
public:
    enum Role   //角色
    {
        Lord,   //地主
        Farmer  //农民
    };

    enum Sex{Man, Woman};
    enum Direction{Left, Right};    // 人物头像显示的方向

    enum Type
    {
        Unknow,     // 不确定
        User,       // 非电脑玩家
        Robot       // 左侧电脑玩家
    };

    explicit Player(QObject *parent = nullptr);
    explicit Player(QString name, QObject *parent = nullptr);

    // 设置/获取当前玩家的名字
    void setName(QString &name);
    QString getName();

    // 设置/获取当前玩家的上一个/下一个玩家
    void setPrevPlayer(Player* player);
    void setNextPlayer(Player* player);
    Player* getPrevPlayer();
    Player* getNextPlayer();


    // 设置/获取玩家的得分
    void setScore(int score);
    int getScore();

    // 设置/获取玩家的角色
    void setRole(Role role);
    Role getRole();

    // 设置/获取玩家类型
    void setType(Type t);
    Type getType();

    // 抢地主下注
    void grabLordBet(int point);
    // 出牌
    void playHand(Cards& cards);

    // 清空玩家手里的扑克牌
    void clearCards();
    // 存储分发的扑克牌
    void storeDispatchCard(Card& card);
    void storeDispatchCards(Cards& cards);
    // 得到玩家手中的牌
    Cards getCards();

    // 得到刚出完牌的玩家对象
    Player* getPendPlayer();
    // 得到玩家刚打出的牌
    Cards getPendCards();
    // 记录
    void storePendingInfo(Player* player, Cards& card);

    // 得到玩家的性别
    Sex getSex();

    // 人物头像的显示方式
    void setDirection(Direction direct);
    Direction getDirection();

    // 判断当前玩家是否赢了游戏
    bool isWin();
    // 设置玩家最后的游戏状态
    void setWin(bool bl = true);

    // 虚函数
    // 考虑叫地主
    virtual void thinkCallLord();
    // 考虑出牌
    virtual void thinkPlayHand();
    // 开始叫地主
    virtual void prepareCallLord();
    // 开始出牌
    virtual void preparePlayHand();

signals:
    // 通知已抢地主下注
    void notifyGrabLordBet(Player* player, int score);
    // 通知已出牌
    void notifyPlayHand(Player* player, Cards& card);
    // 通知已拿到了分发的牌
    void notifyPickCards(Player* player, Cards& card);

protected:
    QString m_name;         // 记录当前玩家的名字
    Player* m_prevPlayer;   // 记录上一个玩家对象
    Player* m_nextPlayer;   // 记录下一个玩家对象

    Player* m_pendPlayer;   // 刚出完牌的用户
    Cards m_pendCards;      // 出牌的用户刚打出的牌

    int m_score;            // 存储玩家的得分
    Cards m_cards;          // 存储玩家手里的扑克牌
    Role m_role;            // 当前玩家的角色
    Type m_type;            // 当前玩家的类型

    Sex m_sex;              // 玩家的性别
    Direction m_direct;     // 人物头像的显示方式: 左侧 or 右侧
    bool m_isWin;           // 记录当前玩家是否赢了游戏
};

#endif // PLAYER_H

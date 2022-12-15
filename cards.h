#ifndef CARDS_H
#define CARDS_H

#include <QObject>
#include <QSet>


// 扑克牌的属性结构体
struct Card
{
    // 扑克牌的点数
    enum CardPoint
    {
        Card_Begin,
        Card_3,
        Card_4,
        Card_5,
        Card_6,
        Card_7,
        Card_8,
        Card_9,
        Card_10,
        Card_J,
        Card_Q,
        Card_K,
        Card_A,
        Card_2,
        Card_SJ,    // 小王 Small Joker
        Card_BJ,    // 大王 Big Joker
        Card_End
    };

    // 扑克牌的牌面属性
    enum CardSuit
    {
        Suit_Begin,
        Diamond,    // 方块
        Club,       // 梅花
        Heart,      // 红桃
        Spade,      // 黑桃
        Suit_End
    };

    Card(){}
    Card(CardPoint p, CardSuit s) : point(p), suit(s){}
    CardPoint point;    // 扑克牌的点数
    CardSuit suit;      // 扑克牌的牌面属性
};

// 定义新的别名
using CardList = QVector<Card>;

/*
 * 全局函数写在头文件中，头文件被其他文件使用导致重定义, 解决方案:
 * 1. 声明写在.h中，函数实现写在.cpp里
 * 2. 如果非要写在.h中，可以声明成内联函数
*/
// 此容器实现方式是基于哈希表，而不是红黑树，若使用自定义类必须提供对应的hash函数：
inline bool operator ==(const Card& left, const Card& right)
{
    return (left.point == right.point && left.suit == right.suit);
}

inline uint qHash(const Card& card)
{
    return card.point * 100 + card.suit;
}

inline bool lessSort(const Card& c1, const Card& c2)
{
    if (c1.point == c2.point)
    {
        return c1.suit < c2.suit;
    }
    else
    {
        return c1.point < c2.point;
    }
}

inline bool greaterSort(const Card& c1, const Card& c2)
{
    if (c1.point == c2.point)
    {
        return c1.suit > c2.suit;
    }
    else
    {
        return c1.point > c2.point;
    }
}

inline bool operator <(const Card& left, const Card& right)
{
    return lessSort(left, right);
}

// 该类用于处理玩家手里的扑克牌
class Cards
{
public:
    // 扑克牌的排序方式: 升序, 降序, 不排序
    enum SortType{Asc, Desc, NoSort};
    explicit Cards();

    // 添加扑克牌
    void add(const Card& card);
    void add(const Cards& cards);
    void add(const QVector<Cards>& array);

    // 方便一次性添加多个对象
    Cards& operator <<(const Cards& cards);
    Cards& operator <<(const Card& card);
    Cards& operator <<(const QVector<Cards>& array);

    // 移除扑克牌
    void remove(const Card& card);
    void remove(const Cards& cards);
    void remove(const QVector<Cards>& array);
    // 判断手里是否有这张牌
    bool contains(const Card& card);
    bool contains(const Cards& cards);
    // 随机发一张牌
    Card takeRandomCard();

    // 判断玩家手里是否还有牌
    bool isEmpty();
    // 清空玩家手里的牌
    void clear();

    // 获得扑克牌的张数
    int cardCount();
    // 计算指定点数的扑克牌的张数
    int pointCount(Card::CardPoint pt);
    // 返回手里这副牌的最小点数
    Card::CardPoint minPoint();
    // 返回手里这副牌的最大点数
    Card::CardPoint maxPoint();

    // 类型转换 QSet -> CardList
    CardList toCardList(SortType type = Asc);

private:
    QSet<Card> m_cards;     // 存储玩家手里的扑克牌/整副扑克牌
};

#endif // CARDS_H

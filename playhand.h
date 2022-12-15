#pragma once

#include "cards.h"
#include <QVector>

//出牌类
class PlayHand
{
public:
    // 出牌组合或者方式
    enum HandType
    {
        Hand_Unknown,               // 未知
        Hand_Pass,                  // 过

        Hand_Single,                // 单
        Hand_Pair,                  // 对

        Hand_Triple,                // 三个
        Hand_Triple_Single,         // 三带一
        Hand_Triple_Pair,           // 三带二

        Hand_Plane,                 // 飞机，555_666
        Hand_Plane_Two_Single,      // 飞机带单，555_666_3_4
        Hand_Plane_Two_Pair,        // 飞机带双，555_666_33_44

        Hand_Seq_Pair,              // 连对，33_44_55(_66...)
        Hand_Seq_Single,            // 顺子，34567(8...)

        Hand_Bomb,                  // 炸弹
        Hand_Bomb_Single,           // 炸弹带一个
        Hand_Bomb_Pair,             // 炸弹带一对
        Hand_Bomb_Two_Single,       // 炸弹带两单

        Hand_Bomb_Jokers,           // 王炸
        Hand_Bomb_Jokers_Single,    // 王炸带一个
        Hand_Bomb_Jokers_Pair,      // 王炸带一对
        Hand_Bomb_Jokers_Two_Single	// 王炸带两单
    };

    PlayHand();
    PlayHand(HandType type, Card::CardPoint base, int extra);
    PlayHand(Cards& cards);

    bool operator ==(const PlayHand& hand);

    void classify(Cards& cards);            // 分类
    void judgeHand();                       // 判断情况

    bool canBeat(const PlayHand& other);    // 判断是否能赢
    HandType getType();                     // 出牌组合或者方式
    Card::CardPoint getBasePoint();         // 获取牌的基本点数
    int getExtra();

protected:
    HandType m_handType;                    // 组合方式
    Card::CardPoint m_basePoint;            //牌面点数
	int m_extra;

    QVector<Card::CardPoint> m_oneCard;     // 一张牌的点数集合
    QVector<Card::CardPoint> m_twoCards;    // 二张牌的点数集合
    QVector<Card::CardPoint> m_threeCards;  // 三张牌的点数集合
    QVector<Card::CardPoint> m_fourCards;   // 四张牌的点数集合
};

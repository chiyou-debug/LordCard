#pragma once

#include <QVector>
#include "cards.h"
#include "player.h"
#include "playhand.h"

class Strategy
{
public:
	Strategy(Player* player, const Cards& cards);
	~Strategy();

    Cards makeStrategy();

public:
    // 找出count张点数为point的牌
    Cards findSamePointCards(Card::CardPoint point, int count);
    // 找出所有张数为count的所有牌数组
    QVector<Cards> findCardsByCount(int count);
    // 根据点数范围找牌
    Cards getRangeCards(Card::CardPoint beginPoint, Card::CardPoint endPoint);
    // 找到第一个能构成顺子的牌集合
    Cards getFirstSeqSingleRange();
    // 在cards中挑出一个顺子，seqInherited记录逐步拨除顺子，allSeqRecord记录所能分离出的顺子
    void pickSeqSingles(QVector<QVector<Cards> >& allSeqRecord, const QVector<Cards>& seqInherited, Cards cards);
    // 挑选出一个最优/最理想的顺子
    QVector<Cards> pickOptimalSeqSingles();
    // 按牌型找到若干手牌，beat为true表示要大过hand
    QVector<Cards> findHand(PlayHand hand, bool beat);
    // 主动出牌
    Cards playFirst();
    // 大过指定牌型
    Cards playBeatHand(PlayHand hand);
    // 能大过时，判断是打还是放行
    bool whetherToBeat(Cards& myCards);
	
protected:
	Player* m_player;
	Cards m_cards;
};

#include "playhand.h"
#include <QMap>
#include <QDebug>

PlayHand::PlayHand()
{

}

PlayHand::PlayHand(HandType type, Card::CardPoint base, int extra)//构造函数
{
	m_handType = type;
	m_basePoint = base;
	m_extra = extra;
}

PlayHand::PlayHand(Cards& cards)
{
    classify(cards);
    judgeHand();
}

void PlayHand::classify(Cards& cards)
{
    CardList cardList = cards.toCardList();

    int* cardRecord = new int[Card::Card_End];
    memset(cardRecord, 0, sizeof(int) * Card::Card_End);

	CardList::const_iterator it = cardList.constBegin();
	for (; it != cardList.constEnd(); it++)
	{
		cardRecord[(int)it->point]++;
	}

	m_oneCard.clear();
	m_twoCards.clear();
	m_threeCards.clear();
	m_fourCards.clear();

    for (int i = 0; i < Card::Card_End; i++)
	{
		if (cardRecord[i] == 1)
		{
            m_oneCard.push_back((Card::CardPoint)i);
		}
		else if (cardRecord[i] == 2)
		{
            m_twoCards.push_back((Card::CardPoint)i);
		}
		else if (cardRecord[i] == 3)
		{
            m_threeCards.push_back((Card::CardPoint)i);
		}
		else if (cardRecord[i] == 4)
		{
            m_fourCards.push_back((Card::CardPoint)i);
		}
	}

	delete[] cardRecord;
}

void PlayHand::judgeHand()
{
	m_handType = Hand_Unknown;
    m_basePoint = Card::Card_Begin;
	m_extra = 0;

	if (m_oneCard.isEmpty() &&
		m_twoCards.isEmpty() &&
		m_threeCards.isEmpty() &&
		m_fourCards.isEmpty())
	{
		m_handType = Hand_Pass;
	}
	else if (m_oneCard.size() == 1 && 
		m_twoCards.isEmpty() && 
		m_threeCards.isEmpty() && 
		m_fourCards.isEmpty())		// 单牌
	{
		m_handType = Hand_Single;
		m_basePoint = m_oneCard[0];
	}
	else if (m_oneCard.isEmpty() &&
		m_twoCards.size() == 1 &&
		m_threeCards.isEmpty() &&
		m_fourCards.isEmpty())		// 对
	{
		m_handType = Hand_Pair;
		m_basePoint = m_twoCards[0];
	}
	else if (m_oneCard.isEmpty() &&
		m_twoCards.isEmpty() &&
		m_threeCards.size() == 1 &&
		m_fourCards.isEmpty())		// 三个
	{
		m_handType = Hand_Triple;
		m_basePoint = m_threeCards[0];
	}
	else if (m_oneCard.size() == 1 &&
		m_twoCards.isEmpty() &&
		m_threeCards.size() == 1 &&
		m_fourCards.isEmpty())		// 三带一
	{
		m_handType = Hand_Triple_Single;
		m_basePoint = m_threeCards[0];
	}
	else if (m_oneCard.isEmpty() &&
		m_twoCards.size() == 1 &&
		m_threeCards.size() == 1 &&
		m_fourCards.isEmpty())		// 三带二
	{
		m_handType = Hand_Triple_Pair;
		m_basePoint = m_threeCards[0];
	}
	else if (m_oneCard.isEmpty() &&
		m_twoCards.isEmpty() &&
		m_threeCards.size() == 2 &&
		m_fourCards.isEmpty())
	{
        std::sort(m_threeCards.begin(), m_threeCards.end());
		if (m_threeCards[1] - m_threeCards[0] == 1 &&
            m_threeCards[1] < Card::Card_2)		// 飞机
		{
			m_handType = Hand_Plane;
			m_basePoint = m_threeCards[0];
		}
	}
	else if (m_oneCard.size() == 2 &&
		m_twoCards.isEmpty() &&
		m_threeCards.size() == 2 &&
		m_fourCards.isEmpty())
	{
        std::sort(m_threeCards.begin(), m_threeCards.end());
        std::sort(m_oneCard.begin(), m_oneCard.end());

		if (m_threeCards[1] - m_threeCards[0] == 1 &&
            m_oneCard[0] != Card::Card_SJ &&
            m_oneCard[1] != Card::Card_BJ &&
            m_threeCards[1] < Card::Card_2)		// 飞机带两单，注意两单不是双王
		{
			m_handType = Hand_Plane_Two_Single;
			m_basePoint = m_threeCards[0];
		}
	}
	else if (m_oneCard.isEmpty() &&
		m_twoCards.size() == 2 &&
		m_threeCards.size() == 2 &&
		m_fourCards.isEmpty())
	{
        std::sort(m_threeCards.begin(), m_threeCards.end());
		if (m_threeCards[1] - m_threeCards[0] == 1 &&
            m_threeCards[1] < Card::Card_2)		// 飞机带两对
		{
			m_handType = Hand_Plane_Two_Pair;
			m_basePoint = m_threeCards[0];
		}
	}
	else if (m_oneCard.isEmpty() &&
		m_twoCards.size() >= 3 &&
		m_threeCards.isEmpty() &&
		m_fourCards.isEmpty())
	{
        std::sort(m_twoCards.begin(), m_twoCards.end());
        if (m_twoCards.first() >= Card::Card_3 &&
            m_twoCards.last() < Card::Card_2 &&
			m_twoCards.last() - m_twoCards.first() == (m_twoCards.size() - 1))	// 连对
		{
			m_handType = Hand_Seq_Pair;
			m_basePoint = m_twoCards[0];
			m_extra = m_twoCards.size();
		}
	}
    else if (m_oneCard.size() >= 5 &&
		m_twoCards.isEmpty() &&
		m_threeCards.isEmpty() &&
		m_fourCards.isEmpty())
	{
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_oneCard.first() >= Card::Card_3 &&
            m_oneCard.last() < Card::Card_2 &&
			m_oneCard.last() - m_oneCard.first() == (m_oneCard.size() - 1))		// 顺子
		{
			m_handType = Hand_Seq_Single;
			m_basePoint = m_oneCard[0];
			m_extra = m_oneCard.size();
		}
	}
	else if (m_oneCard.isEmpty() &&
		m_twoCards.isEmpty() &&
		m_threeCards.isEmpty() &&
		m_fourCards.size() == 1)		// 炸弹
	{
		m_handType = Hand_Bomb;
		m_basePoint = m_fourCards[0];
	}
	else if (m_oneCard.size() == 1 &&
		m_twoCards.isEmpty() &&
		m_threeCards.isEmpty() &&
		m_fourCards.size() == 1)		// 炸弹带一个
	{
		m_handType = Hand_Bomb_Single;
		m_basePoint = m_fourCards[0];
	}
	else if (m_oneCard.isEmpty() &&
		m_twoCards.size() == 1 &&
		m_threeCards.isEmpty() &&
		m_fourCards.size() == 1)		// 炸弹带一对
	{
		m_handType = Hand_Bomb_Pair;
		m_basePoint = m_fourCards[0];
	}
	else if (m_oneCard.size() == 2 &&
		m_twoCards.isEmpty() &&
		m_threeCards.isEmpty() &&
		m_fourCards.size() == 1)
	{
        std::sort(m_oneCard.begin(), m_oneCard.end());

        if (m_oneCard[0] != Card::Card_SJ &&
            m_oneCard[1] != Card::Card_BJ)		//  炸弹带两单，两单不是双王
		{
			m_handType = Hand_Bomb_Two_Single;
			m_basePoint = m_fourCards[0];
		}
	}
	else if (m_oneCard.size() == 2 &&
		m_twoCards.isEmpty() &&
		m_threeCards.isEmpty() &&
		m_fourCards.isEmpty())
	{
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_oneCard[0] == Card::Card_SJ && m_oneCard[1] == Card::Card_BJ)		// 王炸
		{
			m_handType = Hand_Bomb_Jokers;
		}
	}
	else if (m_oneCard.size() == 3 &&
		m_twoCards.isEmpty() &&
		m_threeCards.isEmpty() &&
		m_fourCards.isEmpty())
	{
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_oneCard[1] == Card::Card_SJ && m_oneCard[2] == Card::Card_BJ)		// 王炸带一个
		{
			m_handType = Hand_Bomb_Jokers_Single;
		}
	}
	else if (m_oneCard.size() == 2 &&
		m_twoCards.size() == 1 &&
		m_threeCards.isEmpty() &&
		m_fourCards.isEmpty())
	{
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_oneCard[0] == Card::Card_SJ && m_oneCard[1] == Card::Card_BJ)		// 王炸带一对
		{
			m_handType = Hand_Bomb_Jokers_Single;
		}
	}
	else if (m_oneCard.size() == 4 &&
		m_twoCards.isEmpty() &&
		m_threeCards.isEmpty() &&
		m_fourCards.isEmpty())
	{
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_oneCard[2] == Card::Card_SJ && m_oneCard[3] == Card::Card_BJ)		// 王炸带两个
		{
			m_handType = Hand_Bomb_Jokers_Two_Single;
		}
	}

}

bool PlayHand::operator ==(const PlayHand& hand)
{
	return (m_handType == hand.m_handType &&
		m_basePoint == hand.m_basePoint &&
		m_extra == hand.m_extra);
}

PlayHand::HandType PlayHand::getType()
{
    return m_handType;
}

Card::CardPoint PlayHand::getBasePoint()
{
	return m_basePoint;
}

int PlayHand::getExtra()
{
	return m_extra;
}

bool PlayHand::canBeat(const PlayHand& other)
{
	if (m_handType == Hand_Unknown) return false;

	// 王炸无敌
	if (m_handType == Hand_Bomb_Jokers) return true;

	if (other.m_handType == Hand_Pass) return true;

	// 炸弹可炸普通牌
	if (other.m_handType >= Hand_Single && 
		other.m_handType <= Hand_Seq_Single &&
		m_handType == Hand_Bomb)
	{
		return true;
	}

	if (m_handType == other.m_handType)
	{
		if (m_handType == Hand_Seq_Pair || m_handType == Hand_Seq_Single)
		{
			return (m_basePoint > other.m_basePoint && m_extra == other.m_extra);
		}
		else
		{
			return (m_basePoint > other.m_basePoint);
		}
	}

    return false;
}


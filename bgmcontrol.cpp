#include "bgmcontrol.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTimer>

BGMControl::BGMControl(QObject *parent) : QObject(parent)
{
    for(int i=0; i<5; ++i)
    {
        QMediaPlayer* player = new QMediaPlayer(this);
        QMediaPlaylist* list = new QMediaPlaylist(this);
        // 设置播放列表属性
        if(i<2 || i==4)
        {
            list->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        }
        else if(i == 2)
        {
            list->setPlaybackMode(QMediaPlaylist::Loop);
        }
        player->setPlaylist(list);
        player->setVolume(100);
        m_players.push_back(player);
        m_lists.push_back(list);
    }

    initPlayList();
}

void BGMControl::playCardMusic(Cards cards, bool isfirst, MusicType role)
{
    int index = role == Man ? 0 : 1;
    QMediaPlaylist* list = m_lists[index];

    PlayHand hand(cards);
    PlayHand::HandType type = hand.getType();

    int number = 0;
    Card::CardPoint pt = Card::Card_Begin;
    if(type == PlayHand::Hand_Single || type == PlayHand::Hand_Pair || type == PlayHand::Hand_Triple)
    {
        pt = cards.takeRandomCard().point;
    }

    // 判断牌型
    switch(type)
    {
    case PlayHand::Hand_Single:
        number = pt - 1;
        break;
    case PlayHand::Hand_Pair:
        number = pt - 1 + 15;
        break;
    case PlayHand::Hand_Triple:                     // 3张牌相同
        number = pt - 1 + 15 + 13;
        break;
    case PlayHand::Hand_Triple_Single:              // 三带一
        number = ThreeBindOne;
        break;
    case PlayHand::Hand_Triple_Pair:                // 三带二
        number = ThreeBindPair;
        break;
    case PlayHand::Hand_Plane:                      // 飞机
    case PlayHand::Hand_Plane_Two_Single:           // 飞机带单
    case PlayHand::Hand_Plane_Two_Pair:             // 飞机带双
        number = Plane;
        break;
    case PlayHand::Hand_Seq_Pair:                   // 连对
        number = SequencePair;
        break;
    case PlayHand::Hand_Seq_Single:                 // 顺子
        number = Sequence;
        break;
    case PlayHand::Hand_Bomb:                       // 炸弹
        number = Bomb;
        break;
    case PlayHand::Hand_Bomb_Pair:                  // 炸弹带一对
    case PlayHand::Hand_Bomb_Jokers_Pair:           // 王炸带一对
    case PlayHand::Hand_Bomb_Two_Single:            // 炸弹带两单
    case PlayHand::Hand_Bomb_Jokers_Two_Single:     // 王炸带两单
        number = FourBindTow;
        break;
    case PlayHand::Hand_Bomb_Jokers:                // 王炸
        number = JokerBomb;
        break;
    default:
        break;
    }
    if(!isfirst && (number == ThreeBindOne || number == ThreeBindPair ||
                    number == SequencePair || number == Sequence))
    {
        list->setCurrentIndex(MoreBiger1 + QRandomGenerator::global()->bounded(2));
    }
    else
    {
        list->setCurrentIndex(number);
    }
    if(number == Bomb || number == JokerBomb)
    {
        playAssistMusic(BombVoice);
    }
    if(number == Plane)
    {
        playAssistMusic(PlaneVoice);
    }

    m_players[index]->play();
}

void BGMControl::startBGM(int volume)
{
    m_lists[2]->setCurrentIndex(0);
    m_players[2]->setVolume(volume);
    m_players[2]->play();
}

void BGMControl::stopBGM()
{
    m_players[2]->stop();
}

void BGMControl::playPassMusic(BGMControl::MusicType role)
{
    int index = role == Man ? 0 : 1;
    int random = QRandomGenerator::global()->bounded(4);
    m_lists[index]->setCurrentIndex(random+Pass1);
    m_players[index]->play();
}

void BGMControl::playRobLordMusic(int point, bool isfirst, MusicType role)
{
    qDebug() << "bgmControl class - " << "point: " << point << ", isfirst: " << isfirst;
    int index = role == Man ? 0 : 1;
    if(isfirst && point > 0)
    {
        // 叫地主
        m_lists[index]->setCurrentIndex(Order);
    }
    else if(point == 0)
    {
        if(isfirst)
        {
            // 不叫
            m_lists[index]->setCurrentIndex(NoOrder);
        }
        else
        {
            // 不抢
            m_lists[index]->setCurrentIndex(NoRob);
        }
    }
    else if(point == 2)
    {
        m_lists[index]->setCurrentIndex(Rob1);
    }
    else if(point == 3)
    {
        m_lists[index]->setCurrentIndex(Rob2);
    }
    m_players[index]->play();
}

void BGMControl::playAssistMusic(BGMControl::AssistMusic music, bool isloop)
{
    QMediaPlaylist::PlaybackMode mode = isloop ? QMediaPlaylist::CurrentItemInLoop : QMediaPlaylist::CurrentItemOnce;
    m_lists[3]->setCurrentIndex(music);
    m_lists[3]->setPlaybackMode(mode);
    qDebug() << m_lists[3]->mediaCount();
    qDebug() << "music: " << music << ", mode: " << mode;
    m_players[3]->play();
}

void BGMControl::stopAssistMusic()
{
    m_players[3]->stop();
}

void BGMControl::playEndingMusic(bool isWin)
{
    if(isWin)
    {
        // 赢了
        m_lists[4]->setCurrentIndex(0);
    }
    else
    {
        // 输了
        m_lists[4]->setCurrentIndex(1);
    }
    m_players[4]->play();
}

void BGMControl::initPlayList()
{
    QStringList keylist;
    keylist << "Man" << "Woman" << "BGM" << "Other" << "Ending";

    // 打开并读配置文件
    QFile file(":/conf/playList.json");
    file.open(QFile::ReadOnly);
    QByteArray json = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();

    // 遍历角色
    for(int i=0; i<keylist.size(); ++i)
    {
        QString prefix = keylist.at(i);
        QJsonArray array = obj.value(prefix).toArray();
        QMediaPlaylist* subList = m_lists[i];
        // 遍历文件名并取出
        for(int j=0; j<array.size(); ++j)
        {
            subList->addMedia(QUrl(array.at(j).toString()));
        }
    }

    file.close();
}

void BGMControl::playCardMusic(CardType type, MusicType role)
{
    int index = role == Man ? 0 : 1;
    QMediaPlaylist* list = m_lists[index];
    if(m_players[index]->state() != QMediaPlayer::StoppedState)
    {
        QTimer::singleShot(1200, this, [=]()
        {
            list->setCurrentIndex(type);
            m_players[index]->play();
        });
    }
    else
    {
        list->setCurrentIndex(type);
        m_players[index]->play();
    }
}

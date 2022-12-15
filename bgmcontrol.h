#ifndef BGMCONTROL_H
#define BGMCONTROL_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "playhand.h"

class BGMControl : public QObject
{
    Q_OBJECT
public:
    enum MusicType{Man, Woman, BGM, Other};
    enum CardType
    {
        // 单张牌
        Three,
        Foue,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        Ace,
        Tow,
        SmallJoker,
        BigJoker,
        // 两张牌
        Three_Double,
        Foue_Double,
        Five_Double,
        Six_Double,
        Seven_Double,
        Eight_Double,
        Nine_Double,
        Ten_Double,
        Jack_Double,
        Queen_Double,
        King_Double,
        Ace_Double ,
        Tow_Double,
        // 三张牌
        Three_Triple,
        Foue_Triple,
        Five_Triple,
        Six_Triple,
        Seven_Triple,
        Eight_Triple,
        Nine_Triple,
        Ten_Triple,
        Jack_Triple,
        Queen_Triple,
        King_Triple,
        Ace_Triple,
        Tow_Triple,
        // 其他组合
        Plane,          // 飞机
        SequencePair,   // 连对
        ThreeBindOne,   // 三带一
        ThreeBindPair,  // 三带一对
        Sequence,       // 顺子
        FourBindTow,    // 四带二(单张)
        FourBind2Pair,  // 四带两对
        Bomb,           // 炸弹
        JokerBomb,      // 王炸
        Pass1,          // 过
        Pass2,
        Pass3,
        Pass4,
        MoreBiger1,     // 大你
        MoreBiger2,
        Biggest,        // 压死
        // 抢地主
        NoOrder,        // 不叫
        NoRob,          // 不抢
        Order,          // 叫地主
        Rob1,           // 抢地主
        Rob2,
        Last1,          // 只剩1张牌
        Last2           // 只剩2张牌
    };

    enum AssistMusic
    {
        Dispatch,       // 发牌
        SelectCard,     // 选牌
        PlaneVoice,     // 飞机
        BombVoice,      // 炸弹
        Alert,          // 提醒(还剩2张牌的时候提示)
        Win,            // 赢了
        Fail            // 输了
    };

    explicit BGMControl(QObject *parent = nullptr);
    // 初始化播放列表
    void initPlayList();
    // 播放出牌音乐
    void playCardMusic(CardType type, MusicType role = Man);
    void playCardMusic(Cards cards, bool isfirst = true, MusicType role = Man);
    // 播放背景音乐
    void startBGM(int volume = 88);
    // 停止播放背景音乐
    void stopBGM();
    // 播放不出牌提示音乐
    void playPassMusic(MusicType role = Man);
    // 播放抢地主音乐
    void playRobLordMusic(int point, bool isfirst, MusicType role = Man);
    // 播放辅助音乐
    void playAssistMusic(AssistMusic music, bool isloop = false);
    // 停止播放辅助音乐 -> 循环播放的时候才需要停止
    void stopAssistMusic();
    // 播放结尾音乐
    void playEndingMusic(bool isWin);

private:
    QVector<QMediaPlayer*> m_players;    // 播放器对象: man, woman, bgm, other, 结束音乐
    QVector<QMediaPlaylist*> m_lists;    // 播放列表: man-list, woman-list, bgm-list, other-list, ending-list
};

#endif // BGMCONTROL_H

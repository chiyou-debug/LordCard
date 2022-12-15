#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QDialog>
#include <QMap>
#include <QTimer>
#include <QLabel>
#include "gamecontrol.h"
#include "cardpanel.h"
#include "cards.h"
#include "bgmcontrol.h"
#include "animationwindow.h"
#include "countdown.h"

namespace Ui {
class GamePanel;
}

class GamePanel : public QDialog
{
    Q_OBJECT

public:
    enum AnimationType{ShunZi, LianDui, Plane, JokerBomb, Bomb, Bet};
    explicit GamePanel(QWidget *parent = nullptr);
    ~GamePanel();

    // 更新玩家的分数
    void updatePlayerScore();
    // 初始化玩家窗口环境
    void initPlayerContext();
    // 绘制窗口游戏场景
    void drawGameScene();
    // 更新用户区扑克牌
    void updatePlayerCards(Player* player);
    // 初始化按钮组按钮
    void initButtonGroup();
    // 游戏状态的处理
    void gameStatusProcess(GameControl::GameStatus status);
    // 开始发牌
    void startPickCard();
    // 显示发牌动画
    void showDispatchAnimation(Player* player, int step);
    // 隐藏玩家打出的最后一张牌
    void hidePlayerDropCard(Player* player);
    // 初始化游戏动画窗口
    void initAnimation();
    // 显示游戏动画
    void showAnimation(AnimationType type, int bet = 0);
    // 加载角色图像
    QPixmap loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role);
    // 显示分数面板
    void showScorePanelAnimation();
    // 初始化倒计时窗口
    void initCountDown();
    // 开始出牌倒计时
    void startCountDown();

public slots:
    // 发牌
    void onDispatchCard();
    // 不出牌, 跳过本轮
    void onUserPass();
    // 出牌
    void onUserPlayHand();
    // 处理发牌过程中得到的扑克牌
    void onDisposCard(Player* player, Cards& card);
    // 玩家状态发送变化
    void onPlayerStatusChanged(Player* player, GameControl::PlayerStatus s);
    // 处理扑克牌被选中之后的处理动作
    void onCardSelected(Qt::MouseButton btn);
    // 处理玩家打出的牌
    void onDisposePlayHand(Player* player, Cards& cards);
    // 处理玩家抢地主
    void onGrabLordBet(Player* player, int bet, bool isfirst);

protected:
    void paintEvent(QPaintEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);

private:
    // 裁剪图片
    void cropImage(QPixmap& pix, int x, int y, Card c);
    // 初始化存储扑克牌图片的map容器
    void initCardsMap();

private:
    Ui::GamePanel *ui;
    enum CardAlign
    {
        Horizontal,     // 扑克牌水平放置
        Vertical        // 扑克牌垂直放置
    };

    struct PlayerContext
    {
        QRect cardsRect;        // 玩家扑克牌显示区域
        QRect playHandRect;     // 出牌区域
        CardAlign align;        // 对齐方式
        bool isFrontSide;       // 是否是正面
        QLabel* info;           // 出牌过程中提示信息, 比如: 不要(不出牌)
        QLabel* roleImg;        // 角色图片
        Cards lastCards;        // 玩家手里剩余的扑克牌
    };

    QPixmap m_bkImg;                            // 存储背景图片
    GameControl* m_gameCtl;
    QSize m_cardSize;                           // 每张扑克牌的大小(宽度+高度)
    QPixmap m_cardBackImg;                      // 存储扑克牌的背面图像
    QMap<Card, CardPanel*> m_cardMap;           // 每张卡牌对应的一个面板窗口
    QMap<Player*, PlayerContext> m_contextMap;  // 玩家和玩家手中的牌在窗口中的相关信息
    CardPanel* m_baseCard;                      // 发牌底牌
    CardPanel* m_moveCard;                      // 发牌时的移动牌
    QVector<CardPanel*> m_last3Cards;           // 剩下的3张底牌
    QTimer* m_pickCardTimer;                    // 发牌计时器
    QPoint m_baseCardPos;                       // 发牌的位置
    GameControl::GameStatus m_gameStatus;       // 游戏状态
    QSet<CardPanel*> m_selectCards;             // 用户选择的要打出的牌
    QVector<Player*> m_playerList;              // 存储玩家对象
    BGMControl* m_bgm;                          // 游戏音乐控制
    AnimationWindow* m_animation;               // 动画窗口-炸弹/王炸/飞机
    QHash<CardPanel*, QRect> m_userCards;       // 记录非机器人玩家手中的牌在窗口中的位置
    CardPanel* m_curSelCard = nullptr;          // 当前被选中的牌(鼠标框选过程中)
    QRect m_cardsRect;                          // 非机器人玩家剩余的扑克牌显示区域
    CountDown* m_countDown;                     // 倒计时窗口
};

#endif // GAMEPANEL_H

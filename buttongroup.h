#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT

public:
    enum Panel{Start, PlayCard, PassOrPlay, CallLord, Empty};
    explicit ButtonGroup(QWidget *parent = nullptr);
    ~ButtonGroup();

    // 初始化按钮
    void initButtons();

    void selectPanel(Panel type, int bet = 0);

signals:
    // 开始游戏
    void startGame();
    // 不要
    void pass();
    // 出牌
    void playHand();
    // 放弃叫地主: point=0
    // 叫地主1,2,3分: point=1,2,3
    void betPoint(int point);

private:
    Ui::ButtonGroup *ui;
};

#endif // BUTTONGROUP_H

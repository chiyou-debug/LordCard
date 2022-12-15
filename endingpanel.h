#ifndef ENDINGPANEL_H
#define ENDINGPANEL_H

#include "scorepanel.h"

#include <QLabel>
#include <QPushButton>
#include <QWidget>

class EndingPanel : public QWidget
{
    Q_OBJECT
public:
    explicit EndingPanel(bool isLord, bool isWin, QWidget *parent = nullptr);

    void setPlayScore(int left, int right, int me);

protected:
    void paintEvent(QPaintEvent *event);

signals:
    void continueGame();

private:
    QPixmap m_bk;
    QLabel* m_title;
    QPushButton *m_continue;
    ScorePanel* m_score;
};

#endif // ENDINGPANEL_H

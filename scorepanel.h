#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QLabel>
#include <QWidget>

namespace Ui {
class ScorePanel;
}

class ScorePanel : public QWidget
{
    Q_OBJECT

public:
    enum FontColor{Black, Red};
    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();

    // 设置分数
    void setScores(int left, int me, int right);
    // 设置字体颜色
    void setMyFontColor(FontColor color);
    // 设置字体大小
    void setMyFontSize(int point);

private:
    Ui::ScorePanel *ui;
};

#endif // SCOREPANEL_H

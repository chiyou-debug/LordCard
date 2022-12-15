#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QWidget>
#include <QTimer>

class CountDown : public QWidget
{
    Q_OBJECT
public:
    explicit CountDown(QWidget *parent = nullptr);

    void showCountDown();
    void stopCountDown();

protected:
    void paintEvent(QPaintEvent* ev);

signals:
    void notMuchTime();
    void timeout();

private:
    int m_count;
    QTimer* m_timer;
    QPixmap m_clock;
    QPixmap m_number;
};

#endif // COUNTDOWN_H

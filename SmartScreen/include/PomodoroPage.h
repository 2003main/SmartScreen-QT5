#pragma once
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

class PomodoroPage : public QWidget {
    Q_OBJECT
public:
    explicit PomodoroPage(QWidget *parent = nullptr);

    enum State { IDLE, WORKING, BREAK };

public slots:
    void startPause();
    void reset();
    void tick();

signals:
    void stateChanged(State s);

private:
    void setupUI();
    void updateDisplay();
    void applyStyle();
    QString formatTime(int seconds);

    QLabel      *m_timeLabel;
    QLabel      *m_stateLabel;
    QLabel      *m_cycleLabel;
    QPushButton *m_startBtn;
    QPushButton *m_resetBtn;
    QProgressBar *m_progress;
    QTimer      *m_timer;

    State   m_state      = IDLE;
    int     m_remaining  = 25 * 60;  // 默认25分钟
    int     m_workTime   = 25 * 60;
    int     m_breakTime  = 5  * 60;
    int     m_cycles     = 0;
    int     m_totalTime  = 25 * 60;
};

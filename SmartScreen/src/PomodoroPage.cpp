#include "PomodoroPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

PomodoroPage::PomodoroPage(QWidget *parent)
    : QWidget(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, &PomodoroPage::tick);
    setupUI();
    applyStyle();
    updateDisplay();
}

void PomodoroPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    m_stateLabel = new QLabel("准备开始");
    m_stateLabel->setObjectName("stateLabel");
    m_stateLabel->setAlignment(Qt::AlignCenter);

    m_timeLabel = new QLabel("25:00");
    m_timeLabel->setObjectName("timeLabel");
    m_timeLabel->setAlignment(Qt::AlignCenter);

    m_progress = new QProgressBar;
    m_progress->setObjectName("pomodoroProgress");
    m_progress->setRange(0, 100);
    m_progress->setValue(0);
    m_progress->setTextVisible(false);
    m_progress->setFixedHeight(8);
    m_progress->setMaximumWidth(400);

    m_cycleLabel = new QLabel("已完成专注: 0 个");
    m_cycleLabel->setObjectName("cycleLabel");
    m_cycleLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    m_startBtn = new QPushButton("▶ 开始");
    m_resetBtn = new QPushButton("↺ 重置");
    m_startBtn->setObjectName("startBtn");
    m_resetBtn->setObjectName("resetBtn");
    m_startBtn->setFixedSize(120, 44);
    m_resetBtn->setFixedSize(120, 44);

    btnLayout->addStretch();
    btnLayout->addWidget(m_startBtn);
    btnLayout->addSpacing(20);
    btnLayout->addWidget(m_resetBtn);
    btnLayout->addStretch();

    layout->addStretch();
    layout->addWidget(m_stateLabel);
    layout->addWidget(m_timeLabel);
    layout->addWidget(m_progress, 0, Qt::AlignCenter);
    layout->addWidget(m_cycleLabel);
    layout->addLayout(btnLayout);
    layout->addStretch();

    QLabel *hint = new QLabel("快捷键: 空格=开始/暂停  Esc=重置");
    hint->setObjectName("hintLabel");
    hint->setAlignment(Qt::AlignCenter);
    layout->addWidget(hint);

    connect(m_startBtn, &QPushButton::clicked, this, &PomodoroPage::startPause);
    connect(m_resetBtn, &QPushButton::clicked, this, &PomodoroPage::reset);
}

void PomodoroPage::applyStyle()
{
    setStyleSheet(R"(
        #stateLabel {
            font-size: 18px;
            color: #a0a0b0;
            letter-spacing: 2px;
        }
        #timeLabel {
            font-size: 72px;
            font-weight: bold;
            color: #e94560;
            letter-spacing: 4px;
        }
        QProgressBar#pomodoroProgress {
            background: #16213e;
            border-radius: 4px;
            border: none;
        }
        QProgressBar#pomodoroProgress::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #e94560, stop:1 #ff6b9d);
            border-radius: 4px;
        }
        #cycleLabel {
            font-size: 14px;
            color: #606080;
        }
        #startBtn {
            background-color: #e94560;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 15px;
            font-weight: bold;
        }
        #startBtn:hover { background-color: #ff6b9d; }
        #startBtn:pressed { background-color: #c73652; }
        #resetBtn {
            background-color: #16213e;
            color: #a0a0b0;
            border: 1px solid #0f3460;
            border-radius: 8px;
            font-size: 15px;
        }
        #resetBtn:hover { background-color: #0f3460; color: white; }
        #hintLabel { font-size: 11px; color: #404060; }
    )");
}

void PomodoroPage::startPause()
{
    if (m_state == IDLE || m_state == BREAK) {
        m_state = WORKING;
        m_remaining = m_workTime;
        m_totalTime = m_workTime;
        m_timer->start();
        m_startBtn->setText("⏸ 暂停");
        m_stateLabel->setText("🍅 专注中...");
    } else if (m_state == WORKING) {
        if (m_timer->isActive()) {
            m_timer->stop();
            m_startBtn->setText("▶ 继续");
            m_stateLabel->setText("⏸ 已暂停");
        } else {
            m_timer->start();
            m_startBtn->setText("⏸ 暂停");
            m_stateLabel->setText("🍅 专注中...");
        }
    }
    updateDisplay();
}

void PomodoroPage::reset()
{
    m_timer->stop();
    m_state     = IDLE;
    m_remaining = m_workTime;
    m_totalTime = m_workTime;
    m_startBtn->setText("▶ 开始");
    m_stateLabel->setText("准备开始");
    updateDisplay();
}

void PomodoroPage::tick()
{
    if (m_remaining > 0) {
        m_remaining--;
        updateDisplay();
    } else {
        m_timer->stop();
        if (m_state == WORKING) {
            m_cycles++;
            m_cycleLabel->setText(QString("已完成专注: %1 个").arg(m_cycles));
            m_state     = BREAK;
            m_remaining = m_breakTime;
            m_totalTime = m_breakTime;
            m_stateLabel->setText("☕ 休息时间！");
            m_startBtn->setText("▶ 开始休息");
            // 自动开始休息
            m_timer->start();
        } else {
            // 休息结束
            m_state     = IDLE;
            m_remaining = m_workTime;
            m_totalTime = m_workTime;
            m_stateLabel->setText("✅ 休息结束，准备下一轮");
            m_startBtn->setText("▶ 开始");
        }
        updateDisplay();
    }
}

void PomodoroPage::updateDisplay()
{
    m_timeLabel->setText(formatTime(m_remaining));
    int pct = (m_totalTime > 0)
        ? static_cast<int>((1.0 - (double)m_remaining / m_totalTime) * 100)
        : 0;
    m_progress->setValue(pct);
}

QString PomodoroPage::formatTime(int seconds)
{
    int m = seconds / 60;
    int s = seconds % 60;
    return QString("%1:%2")
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0'));
}

#include "MainWindow.h"
#include "PomodoroPage.h"
#include "WeatherPage.h"
#include "SettingsPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QShortcut>
#include <QKeySequence>
#include <QWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_pageManager = new PageManager(this);
    setupUI();
    setupStyle();
    setupShortcuts();

    connect(m_pageManager, &PageManager::pageChanged,
            this, &MainWindow::onSwitchPage);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 顶部导航栏
    QWidget *navbar = new QWidget;
    navbar->setFixedHeight(48);
    navbar->setObjectName("navbar");

    QHBoxLayout *navLayout = new QHBoxLayout(navbar);
    navLayout->setContentsMargins(10, 0, 10, 0);

    QLabel *titleLabel = new QLabel("⏱ SmartScreen");
    titleLabel->setObjectName("navTitle");

    QPushButton *btnPomodoro = new QPushButton("🍅 专注");
    QPushButton *btnWeather  = new QPushButton("🌤 天气");
    QPushButton *btnSettings = new QPushButton("⚙ 设置");

    btnPomodoro->setObjectName("navBtn");
    btnWeather->setObjectName("navBtn");
    btnSettings->setObjectName("navBtn");

    navLayout->addWidget(titleLabel);
    navLayout->addStretch();
    navLayout->addWidget(btnPomodoro);
    navLayout->addWidget(btnWeather);
    navLayout->addWidget(btnSettings);

    connect(btnPomodoro, &QPushButton::clicked, [this](){
        m_pageManager->switchTo(PageManager::POMODORO);
    });
    connect(btnWeather, &QPushButton::clicked, [this](){
        m_pageManager->switchTo(PageManager::WEATHER);
    });
    connect(btnSettings, &QPushButton::clicked, [this](){
        m_pageManager->switchTo(PageManager::SETTINGS);
    });

    // 页面堆叠
    m_stack = new QStackedWidget;
    m_pomodoroPage = new PomodoroPage;
    m_weatherPage  = new WeatherPage;
    m_settingsPage = new SettingsPage;

    m_stack->addWidget(m_pomodoroPage);  // index 0
    m_stack->addWidget(m_weatherPage);   // index 1
    m_stack->addWidget(m_settingsPage);  // index 2

    mainLayout->addWidget(navbar);
    mainLayout->addWidget(m_stack);

    // Settings信号转发
    connect(m_settingsPage, &SettingsPage::workTimeChanged,
            m_pomodoroPage, [this](int min){
        m_pomodoroPage->reset();
    });
}

void MainWindow::setupStyle()
{
    setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #1a1a2e;
            color: #e0e0e0;
        }
        #navbar {
            background-color: #16213e;
            border-bottom: 1px solid #0f3460;
        }
        #navTitle {
            color: #e94560;
            font-size: 16px;
            font-weight: bold;
        }
        #navBtn {
            background: transparent;
            color: #a0a0b0;
            border: none;
            padding: 6px 14px;
            font-size: 13px;
            border-radius: 4px;
        }
        #navBtn:hover {
            background-color: #0f3460;
            color: #e0e0e0;
        }
        #navBtn:pressed {
            background-color: #e94560;
            color: white;
        }
    )");
}

void MainWindow::setupShortcuts()
{
    // 快捷键切换页面
    QShortcut *sc1 = new QShortcut(QKeySequence("F1"), this);
    QShortcut *sc2 = new QShortcut(QKeySequence("F2"), this);
    QShortcut *sc3 = new QShortcut(QKeySequence("F3"), this);
    QShortcut *scSpace = new QShortcut(QKeySequence("Space"), this);

    connect(sc1, &QShortcut::activated, [this](){ m_pageManager->switchTo(PageManager::POMODORO); });
    connect(sc2, &QShortcut::activated, [this](){ m_pageManager->switchTo(PageManager::WEATHER); });
    connect(sc3, &QShortcut::activated, [this](){ m_pageManager->switchTo(PageManager::SETTINGS); });
    // 空格键：在番茄钟页面时启动/暂停
    connect(scSpace, &QShortcut::activated, [this](){
        if (m_pageManager->currentPage() == PageManager::POMODORO)
            m_pomodoroPage->startPause();
    });
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // 嵌入式物理按键支持（根据T113实际GPIO按键映射修改）
    switch (event->key()) {
    case Qt::Key_Left:
        m_pageManager->switchTo(PageManager::POMODORO); break;
    case Qt::Key_Right:
        m_pageManager->switchTo(PageManager::WEATHER);  break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (m_pageManager->currentPage() == PageManager::POMODORO)
            m_pomodoroPage->startPause();
        break;
    case Qt::Key_Escape:
        m_pomodoroPage->reset(); break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::onSwitchPage(int index)
{
    m_stack->setCurrentIndex(index);
}

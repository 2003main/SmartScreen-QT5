#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QKeyEvent>
#include "PageManager.h"

class PomodoroPage;
class WeatherPage;
class SettingsPage;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onSwitchPage(int index);

private:
    void setupUI();
    void setupStyle();
    void setupShortcuts();

    QStackedWidget  *m_stack;
    PomodoroPage    *m_pomodoroPage;
    WeatherPage     *m_weatherPage;
    SettingsPage    *m_settingsPage;
    PageManager     *m_pageManager;
};

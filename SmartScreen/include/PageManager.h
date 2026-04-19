#pragma once
#include <QObject>

class PageManager : public QObject {
    Q_OBJECT
public:
    enum Page { POMODORO = 0, WEATHER = 1, SETTINGS = 2 };
    explicit PageManager(QObject *parent = nullptr);
    void switchTo(Page page);
    Page currentPage() const { return m_current; }

signals:
    void pageChanged(int index);

private:
    Page m_current = POMODORO;
};

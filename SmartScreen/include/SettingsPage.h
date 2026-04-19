#pragma once
#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class SettingsPage : public QWidget {
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = nullptr);

signals:
    void workTimeChanged(int minutes);
    void breakTimeChanged(int minutes);
    void cityChanged(const QString &city);
    void apiKeyChanged(const QString &key);

private slots:
    void onApply();

private:
    void setupUI();
    void applyStyle();

    QSpinBox    *m_workSpin;
    QSpinBox    *m_breakSpin;
    QLineEdit   *m_cityEdit;
    QLineEdit   *m_apiKeyEdit;
    QPushButton *m_applyBtn;
};

#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "WeatherManager.h"

class WeatherPage : public QWidget {
    Q_OBJECT
public:
    explicit WeatherPage(QWidget *parent = nullptr);

public slots:
    void onWeatherUpdated(const WeatherData &data);
    void onFetchError(const QString &error);
    void onRefreshClicked();
    void onCityChanged();

private:
    void setupUI();
    void applyStyle();
    void showLoading();

    QLabel      *m_cityLabel;
    QLabel      *m_tempLabel;
    QLabel      *m_descLabel;
    QLabel      *m_humidityLabel;
    QLabel      *m_windLabel;
    QLabel      *m_iconLabel;
    QLabel      *m_updateTimeLabel;
    QLineEdit   *m_cityInput;
    QPushButton *m_refreshBtn;

    WeatherManager *m_manager;
};

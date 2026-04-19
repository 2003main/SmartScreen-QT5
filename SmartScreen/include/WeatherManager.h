#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QTimer>

struct WeatherData {
    QString city;
    QString description;
    double  temperature = 0.0;
    double  humidity    = 0.0;
    double  windSpeed   = 0.0;
    QString icon;
    bool    valid       = false;
};

class WeatherManager : public QObject {
    Q_OBJECT
public:
    explicit WeatherManager(QObject *parent = nullptr);
    void fetchWeather(const QString &city);
    void setApiKey(const QString &key) { m_apiKey = key; }
    void setCity(const QString &city)  { m_city = city; }
    void startAutoRefresh(int intervalMinutes = 10);
    void stopAutoRefresh();

signals:
    void weatherUpdated(const WeatherData &data);
    void fetchError(const QString &error);

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void onAutoRefresh();

private:
    WeatherData parseResponse(const QJsonObject &obj);

    QNetworkAccessManager *m_nam;
    QTimer                *m_refreshTimer;
    QString                m_apiKey;
    QString                m_city = "Nanchang"; // 默认南昌
};

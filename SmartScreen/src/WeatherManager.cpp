#include "WeatherManager.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUrl>
#include <QDebug>

WeatherManager::WeatherManager(QObject *parent)
    : QObject(parent)
{
    m_nam = new QNetworkAccessManager(this);
    m_refreshTimer = new QTimer(this);
    connect(m_nam, &QNetworkAccessManager::finished,
            this, &WeatherManager::onReplyFinished);
    connect(m_refreshTimer, &QTimer::timeout,
            this, &WeatherManager::onAutoRefresh);
}

void WeatherManager::fetchWeather(const QString &city)
{
    m_city = city;
    // 使用 OpenWeatherMap API（免费）
    // 需要在 SettingsPage 填入 API Key
    // https://openweathermap.org/api 注册获取免费key
    QString url = QString(
        "http://api.openweathermap.org/data/2.5/weather"
        "?q=%1&appid=%2&units=metric&lang=zh_cn"
    ).arg(city).arg(m_apiKey);

    if (m_apiKey.isEmpty()) {
        // 无API Key时返回模拟数据（便于调试）
        WeatherData mock;
        mock.city        = city;
        mock.description = "晴（模拟数据）";
        mock.temperature = 22.5;
        mock.humidity    = 65.0;
        mock.windSpeed   = 3.2;
        mock.icon        = "☀️";
        mock.valid       = true;
        emit weatherUpdated(mock);
        return;
    }

    QNetworkRequest request(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_nam->get(request);
}

void WeatherManager::onReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        emit fetchError(reply->errorString());
        return;
    }
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        emit fetchError("JSON解析失败");
        return;
    }
    WeatherData wd = parseResponse(doc.object());
    if (wd.valid)
        emit weatherUpdated(wd);
    else
        emit fetchError("数据解析错误");
}

WeatherData WeatherManager::parseResponse(const QJsonObject &obj)
{
    WeatherData wd;
    // OpenWeatherMap响应结构
    if (!obj.contains("main")) return wd;

    QJsonObject main = obj["main"].toObject();
    QJsonArray  weather = obj["weather"].toArray();

    wd.city        = obj["name"].toString();
    wd.temperature = main["temp"].toDouble();
    wd.humidity    = main["humidity"].toDouble();

    QJsonObject wind = obj["wind"].toObject();
    wd.windSpeed   = wind["speed"].toDouble();

    if (!weather.isEmpty()) {
        QJsonObject w = weather[0].toObject();
        wd.description = w["description"].toString();
        // 根据图标ID映射emoji
        QString iconId = w["icon"].toString();
        if      (iconId.startsWith("01")) wd.icon = "☀️";
        else if (iconId.startsWith("02")) wd.icon = "⛅";
        else if (iconId.startsWith("03") || iconId.startsWith("04")) wd.icon = "☁️";
        else if (iconId.startsWith("09") || iconId.startsWith("10")) wd.icon = "🌧️";
        else if (iconId.startsWith("11")) wd.icon = "⛈️";
        else if (iconId.startsWith("13")) wd.icon = "❄️";
        else wd.icon = "🌫️";
    }

    wd.valid = true;
    return wd;
}

void WeatherManager::startAutoRefresh(int intervalMinutes)
{
    m_refreshTimer->start(intervalMinutes * 60 * 1000);
}

void WeatherManager::stopAutoRefresh()
{
    m_refreshTimer->stop();
}

void WeatherManager::onAutoRefresh()
{
    fetchWeather(m_city);
}

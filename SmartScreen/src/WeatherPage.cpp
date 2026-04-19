#include "WeatherPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDateTime>

WeatherPage::WeatherPage(QWidget *parent)
    : QWidget(parent)
{
    m_manager = new WeatherManager(this);
    setupUI();
    applyStyle();
    // 启动时加载（无Key使用模拟数据）
    m_manager->fetchWeather("Nanchang");
    m_manager->startAutoRefresh(10);

    connect(m_manager, &WeatherManager::weatherUpdated,
            this, &WeatherPage::onWeatherUpdated);
    connect(m_manager, &WeatherManager::fetchError,
            this, &WeatherPage::onFetchError);
}

void WeatherPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(12);

    // 城市+图标行
    QHBoxLayout *topRow = new QHBoxLayout;
    m_iconLabel = new QLabel("☀️");
    m_iconLabel->setObjectName("weatherIcon");
    m_cityLabel = new QLabel("--");
    m_cityLabel->setObjectName("cityLabel");
    topRow->addStretch();
    topRow->addWidget(m_iconLabel);
    topRow->addSpacing(10);
    topRow->addWidget(m_cityLabel);
    topRow->addStretch();

    m_tempLabel = new QLabel("--°C");
    m_tempLabel->setObjectName("tempLabel");
    m_tempLabel->setAlignment(Qt::AlignCenter);

    m_descLabel = new QLabel("--");
    m_descLabel->setObjectName("descLabel");
    m_descLabel->setAlignment(Qt::AlignCenter);

    // 详情行
    QHBoxLayout *detailRow = new QHBoxLayout;
    m_humidityLabel = new QLabel("💧 湿度: --%");
    m_windLabel     = new QLabel("💨 风速: -- m/s");
    m_humidityLabel->setObjectName("detailLabel");
    m_windLabel->setObjectName("detailLabel");
    detailRow->addStretch();
    detailRow->addWidget(m_humidityLabel);
    detailRow->addSpacing(40);
    detailRow->addWidget(m_windLabel);
    detailRow->addStretch();

    // 城市输入+刷新
    QHBoxLayout *inputRow = new QHBoxLayout;
    m_cityInput = new QLineEdit;
    m_cityInput->setPlaceholderText("输入城市（英文）如: Beijing");
    m_cityInput->setObjectName("cityInput");
    m_cityInput->setMaximumWidth(220);
    m_refreshBtn = new QPushButton("🔄 刷新");
    m_refreshBtn->setObjectName("refreshBtn");
    inputRow->addStretch();
    inputRow->addWidget(m_cityInput);
    inputRow->addWidget(m_refreshBtn);
    inputRow->addStretch();

    m_updateTimeLabel = new QLabel("上次更新: --");
    m_updateTimeLabel->setObjectName("updateTime");
    m_updateTimeLabel->setAlignment(Qt::AlignCenter);

    layout->addStretch();
    layout->addLayout(topRow);
    layout->addWidget(m_tempLabel);
    layout->addWidget(m_descLabel);
    layout->addLayout(detailRow);
    layout->addSpacing(20);
    layout->addLayout(inputRow);
    layout->addWidget(m_updateTimeLabel);
    layout->addStretch();

    connect(m_refreshBtn, &QPushButton::clicked,
            this, &WeatherPage::onRefreshClicked);
    connect(m_cityInput, &QLineEdit::returnPressed,
            this, &WeatherPage::onCityChanged);
}

void WeatherPage::applyStyle()
{
    setStyleSheet(R"(
        #weatherIcon { font-size: 52px; }
        #cityLabel {
            font-size: 26px;
            font-weight: bold;
            color: #e0e0e0;
        }
        #tempLabel {
            font-size: 64px;
            font-weight: bold;
            color: #53d8fb;
        }
        #descLabel {
            font-size: 16px;
            color: #a0a0b0;
        }
        #detailLabel {
            font-size: 14px;
            color: #808090;
        }
        #cityInput {
            background: #16213e;
            color: #e0e0e0;
            border: 1px solid #0f3460;
            border-radius: 6px;
            padding: 6px 10px;
            font-size: 13px;
        }
        #refreshBtn {
            background: #0f3460;
            color: #e0e0e0;
            border: none;
            border-radius: 6px;
            padding: 6px 16px;
            font-size: 13px;
        }
        #refreshBtn:hover { background: #1a5276; }
        #updateTime { font-size: 11px; color: #404060; }
    )");
}

void WeatherPage::onWeatherUpdated(const WeatherData &data)
{
    m_cityLabel->setText(data.city);
    m_tempLabel->setText(QString("%1°C").arg(data.temperature, 0, 'f', 1));
    m_descLabel->setText(data.description);
    m_humidityLabel->setText(QString("💧 湿度: %1%").arg(data.humidity, 0, 'f', 0));
    m_windLabel->setText(QString("💨 风速: %1 m/s").arg(data.windSpeed, 0, 'f', 1));
    m_iconLabel->setText(data.icon);
    m_updateTimeLabel->setText("上次更新: " +
        QDateTime::currentDateTime().toString("HH:mm:ss"));
}

void WeatherPage::onFetchError(const QString &error)
{
    m_descLabel->setText("获取失败: " + error);
}

void WeatherPage::onRefreshClicked()
{
    onCityChanged();
}

void WeatherPage::onCityChanged()
{
    QString city = m_cityInput->text().trimmed();
    if (city.isEmpty()) city = "Nanchang";
    m_manager->fetchWeather(city);
    m_descLabel->setText("加载中...");
}

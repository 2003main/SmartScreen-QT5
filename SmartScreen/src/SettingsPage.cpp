#include "SettingsPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    applyStyle();
}

void SettingsPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    QLabel *title = new QLabel("⚙ 系统设置");
    title->setObjectName("settingsTitle");
    title->setAlignment(Qt::AlignCenter);

    // 番茄时钟设置组
    QGroupBox *pomodoroGroup = new QGroupBox("🍅 番茄时钟");
    pomodoroGroup->setObjectName("settingsGroup");
    QFormLayout *pomodoroForm = new QFormLayout(pomodoroGroup);

    m_workSpin = new QSpinBox;
    m_workSpin->setRange(1, 60);
    m_workSpin->setValue(25);
    m_workSpin->setSuffix(" 分钟");
    m_workSpin->setObjectName("settingsSpin");

    m_breakSpin = new QSpinBox;
    m_breakSpin->setRange(1, 30);
    m_breakSpin->setValue(5);
    m_breakSpin->setSuffix(" 分钟");
    m_breakSpin->setObjectName("settingsSpin");

    pomodoroForm->addRow("专注时长:", m_workSpin);
    pomodoroForm->addRow("休息时长:", m_breakSpin);

    // 天气设置组
    QGroupBox *weatherGroup = new QGroupBox("🌤 天气设置");
    weatherGroup->setObjectName("settingsGroup");
    QFormLayout *weatherForm = new QFormLayout(weatherGroup);

    m_cityEdit = new QLineEdit("Nanchang");
    m_cityEdit->setObjectName("settingsEdit");
    m_cityEdit->setPlaceholderText("城市名（英文）");

    m_apiKeyEdit = new QLineEdit;
    m_apiKeyEdit->setObjectName("settingsEdit");
    m_apiKeyEdit->setPlaceholderText("OpenWeatherMap API Key");
    m_apiKeyEdit->setEchoMode(QLineEdit::Password);

    weatherForm->addRow("默认城市:", m_cityEdit);
    weatherForm->addRow("API Key:", m_apiKeyEdit);

    // 快捷键说明
    QGroupBox *shortcutGroup = new QGroupBox("⌨ 快捷键");
    shortcutGroup->setObjectName("settingsGroup");
    QVBoxLayout *scLayout = new QVBoxLayout(shortcutGroup);
    QLabel *scInfo = new QLabel(
        "F1  —  专注页\n"
        "F2  —  天气页\n"
        "F3  —  设置页\n"
        "空格 — 开始/暂停番茄钟\n"
        "Esc — 重置番茄钟\n"
        "← → — 物理按键切换页面（嵌入式）"
    );
    scInfo->setObjectName("scInfo");
    scLayout->addWidget(scInfo);

    m_applyBtn = new QPushButton("✔ 应用设置");
    m_applyBtn->setObjectName("applyBtn");
    m_applyBtn->setFixedSize(160, 44);

    layout->addWidget(title);
    layout->addWidget(pomodoroGroup);
    layout->addWidget(weatherGroup);
    layout->addWidget(shortcutGroup);
    layout->addWidget(m_applyBtn, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(m_applyBtn, &QPushButton::clicked, this, &SettingsPage::onApply);
}

void SettingsPage::applyStyle()
{
    setStyleSheet(R"(
        #settingsTitle {
            font-size: 20px;
            font-weight: bold;
            color: #e94560;
        }
        QGroupBox#settingsGroup {
            color: #a0a0b0;
            border: 1px solid #0f3460;
            border-radius: 8px;
            margin-top: 8px;
            padding: 12px;
            font-size: 14px;
            max-width: 420px;
        }
        QGroupBox#settingsGroup::title {
            subcontrol-origin: margin;
            padding: 0 6px;
        }
        QSpinBox#settingsSpin, QLineEdit#settingsEdit {
            background: #16213e;
            color: #e0e0e0;
            border: 1px solid #0f3460;
            border-radius: 4px;
            padding: 4px 8px;
            font-size: 13px;
            min-width: 180px;
        }
        #scInfo {
            color: #808090;
            font-size: 13px;
            line-height: 1.8;
        }
        #applyBtn {
            background-color: #e94560;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 15px;
            font-weight: bold;
        }
        #applyBtn:hover { background-color: #ff6b9d; }
    )");
}

void SettingsPage::onApply()
{
    emit workTimeChanged(m_workSpin->value());
    emit breakTimeChanged(m_breakSpin->value());
    emit cityChanged(m_cityEdit->text());
    emit apiKeyChanged(m_apiKeyEdit->text());
    m_applyBtn->setText("✔ 已应用！");
    QTimer::singleShot(1500, this, [this](){
        m_applyBtn->setText("✔ 应用设置");
    });
}

/**
 * DEE WALLET - Welcome Screen Implementation
 */

#include "WelcomeScreen.h"
#include "CreateWalletDialog.h"
#include "ImportWalletDialog.h"
#include "StyleHelper.h"
#include "DesignTokens.h"
#include "../core/KeyfileManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QStandardPaths>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QInputDialog>
#include <QMessageBox>

// KeyfileCard Implementation
KeyfileCard::KeyfileCard(const KeyfileInfo &info, QWidget *parent)
    : QWidget(parent)
    , keyfileInfo(info)
    , isHovered(false)
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(20);
    
    // Name - cleaner typography
    nameLabel = new QLabel(info.filename, this);
    nameLabel->setStyleSheet(QString(R"(
        QLabel {
            background: transparent;
            font-size: %1px;
            font-weight: %2;
            color: %3;
            border: none;
        }
    )")
        .arg(DesignTokens::Typography::FONT_SIZE_MD)
        .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
        .arg(DesignTokens::Colors::TEXT_BODY));
    layout->addWidget(nameLabel);

    layout->addStretch();

    // Date - subtle styling
    QDateTime dt = QDateTime::fromMSecsSinceEpoch(info.updatedAt);
    QString dateStr = dt.toString("yyyy-MM-dd HH:mm");
    dateLabel = new QLabel(dateStr, this);
    dateLabel->setStyleSheet(QString(R"(
        QLabel {
            background: transparent;
            font-size: %1px;
            color: %2;
            border: none;
        }
    )")
        .arg(DesignTokens::Typography::FONT_SIZE_SM)
        .arg(DesignTokens::Colors::TEXT_SUBTLE));
    layout->addWidget(dateLabel);
    
    setMinimumHeight(64);
    setMaximumHeight(64);
    setCursor(Qt::PointingHandCursor);
    
    updateStyle();
}

void KeyfileCard::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    isHovered = true;
    
    // Animate height
    QPropertyAnimation *animation = new QPropertyAnimation(this, "minimumHeight");
    animation->setDuration(150);
    animation->setStartValue(60);
    animation->setEndValue(70);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    
    QPropertyAnimation *animation2 = new QPropertyAnimation(this, "maximumHeight");
    animation2->setDuration(150);
    animation2->setStartValue(60);
    animation2->setEndValue(70);
    animation2->start(QAbstractAnimation::DeleteWhenStopped);
    
    updateStyle();
}

void KeyfileCard::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    isHovered = false;
    
    // Animate height back
    QPropertyAnimation *animation = new QPropertyAnimation(this, "minimumHeight");
    animation->setDuration(150);
    animation->setStartValue(70);
    animation->setEndValue(60);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    
    QPropertyAnimation *animation2 = new QPropertyAnimation(this, "maximumHeight");
    animation2->setDuration(150);
    animation2->setStartValue(70);
    animation2->setEndValue(60);
    animation2->start(QAbstractAnimation::DeleteWhenStopped);
    
    updateStyle();
}
void KeyfileCard::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked(keyfileInfo.filepath);
}

void KeyfileCard::updateStyle()
{
    if (isHovered) {
        setStyleSheet(QString(R"(
            KeyfileCard {
                background-color: %1;
                border: 1px solid %2;
                border-radius: %3px;
            }
        )")
            .arg(DesignTokens::Colors::BG_SECONDARY)
            .arg(DesignTokens::Colors::BRAND_PRIMARY)
            .arg(DesignTokens::BorderRadius::RADIUS_XL));
    } else {
        setStyleSheet(QString(R"(
            KeyfileCard {
                background-color: %1;
                border: 1px solid %2;
                border-radius: %3px;
            }
        )")
            .arg(DesignTokens::Colors::BG_SECONDARY)
            .arg(DesignTokens::Colors::BORDER_DEFAULT)
            .arg(DesignTokens::BorderRadius::RADIUS_XL));
    }
}

// WelcomeScreen Implementation
WelcomeScreen::WelcomeScreen(QWidget *parent)
    : QWidget(parent)
    , scrollArea(new QScrollArea(this))
    , keyfileContainer(new QWidget(this))
    , keyfileLayout(new QVBoxLayout(keyfileContainer))
    , createButton(new QPushButton("새 지갑 만들기", this))
    , importButton(new QPushButton("지갑 가져오기", this))
    , titleLabel(new QLabel("DEE WALLET", this))
    , subtitleLabel(new QLabel("다중 체인 암호화폐 지갑", this))
{
    setupUI();
    refreshKeyfileList();
}

void WelcomeScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 40, 50, 40);
    mainLayout->setSpacing(24);

    // Title section - cleaner design
    titleLabel->setStyleSheet(StyleHelper::titleLabel());
    titleLabel->setAlignment(Qt::AlignCenter);

    subtitleLabel->setStyleSheet(QString(R"(
        QLabel {
            background: transparent;
            font-size: %1px;
            color: %2;
            font-weight: %3;
            border: none;
        }
    )")
        .arg(DesignTokens::Typography::FONT_SIZE_MD)
        .arg(DesignTokens::Colors::TEXT_SUBTLE)
        .arg(DesignTokens::Typography::FONT_WEIGHT_NORMAL));
    subtitleLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addSpacing(DesignTokens::Spacing::SPACING_XL);

    // Keyfiles section header
    auto *listLabel = new QLabel("저장된 지갑", this);
    listLabel->setStyleSheet(StyleHelper::subheadingLabel());
    mainLayout->addWidget(listLabel);

    // Scroll area for keyfile cards
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(keyfileContainer);
    scrollArea->setStyleSheet(StyleHelper::scrollArea());

    // Set container background to transparent
    keyfileContainer->setStyleSheet(StyleHelper::transparentBackground());
    
    keyfileLayout->setSpacing(16);
    keyfileLayout->setAlignment(Qt::AlignTop);
    
    mainLayout->addWidget(scrollArea, 1);

    // Button section - cleaner design
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(16);

    createButton->setMinimumHeight(DesignTokens::Sizes::BUTTON_HEIGHT_LG);
    createButton->setCursor(Qt::PointingHandCursor);
    createButton->setStyleSheet(StyleHelper::primaryButton());

    importButton->setMinimumHeight(DesignTokens::Sizes::BUTTON_HEIGHT_LG);
    importButton->setCursor(Qt::PointingHandCursor);
    importButton->setStyleSheet(StyleHelper::secondaryButton());

    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(importButton);

    mainLayout->addLayout(buttonLayout);

    connect(createButton, &QPushButton::clicked, this, &WelcomeScreen::onCreateWallet);
    connect(importButton, &QPushButton::clicked, this, &WelcomeScreen::onImportWallet);
}

QVector<KeyfileInfo> WelcomeScreen::scanKeyfiles()
{
    QVector<KeyfileInfo> keyfiles;
    
    // Scan the folder where deewallet.app is located (build folder)
    QString appDir = QApplication::applicationDirPath();
    QDir appDirObj(appDir);
    appDirObj.cdUp(); // Contents
    appDirObj.cdUp(); // deewallet.app
    appDirObj.cdUp(); // build
    QDir buildDir(appDirObj.absolutePath());
    QStringList filters;
    filters << "*.keyfile";
    QFileInfoList fileList = buildDir.entryInfoList(filters, QDir::Files);

    for (const QFileInfo &fileInfo : fileList) {
        KeyfileInfo info;
        info.filename = fileInfo.fileName();
        info.filepath = fileInfo.absoluteFilePath();
        info.createdAt = fileInfo.birthTime().toMSecsSinceEpoch();
        info.updatedAt = fileInfo.lastModified().toMSecsSinceEpoch();
        info.size = fileInfo.size();

        // Basic validation: check if it's valid JSON
        QFile file(info.filepath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            info.isValid = !doc.isNull() && doc.isObject();
            file.close();
        } else {
            info.isValid = false;
        }

        keyfiles.append(info);
    }

    // Sort by update time (newest first)
    std::sort(keyfiles.begin(), keyfiles.end(),
              [](const KeyfileInfo &a, const KeyfileInfo &b) {
                  return a.updatedAt > b.updatedAt;
              });

    return keyfiles;
}

void WelcomeScreen::refreshKeyfileList()
{
    // Clear existing cards
    while (keyfileLayout->count() > 0) {
        QLayoutItem *item = keyfileLayout->takeAt(0);
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    QVector<KeyfileInfo> keyfiles = scanKeyfiles();

    if (keyfiles.isEmpty()) {
        auto *emptyLabel = new QLabel("지갑이 없습니다. 새 지갑을 만들어보세요!", this);
        emptyLabel->setStyleSheet(R"(
            QLabel {
                font-size: 16px;
                color: #666666;
                padding: 40px;
            }
        )");
        emptyLabel->setAlignment(Qt::AlignCenter);
        keyfileLayout->addWidget(emptyLabel);
        return;
    }

    // Add cards as vertical list
    for (const KeyfileInfo &info : keyfiles) {
        auto *card = new KeyfileCard(info, this);
        connect(card, &KeyfileCard::clicked, this, &WelcomeScreen::onKeyfileClicked);
        keyfileLayout->addWidget(card);
    }
    
    keyfileLayout->addStretch();
}

QString WelcomeScreen::formatDate(qint64 timestamp)
{
    QDateTime dt = QDateTime::fromMSecsSinceEpoch(timestamp);
    return dt.toString("MMM dd, yyyy");
}

void WelcomeScreen::onCreateWallet()
{
    CreateWalletDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString mnemonic = dialog.getMnemonic();
        refreshKeyfileList();
        emit walletCreated(mnemonic);
    }
}

void WelcomeScreen::onImportWallet()
{
    ImportWalletDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString mnemonic = dialog.getMnemonic();
        refreshKeyfileList();
        emit walletImported(mnemonic);
    }
}

void WelcomeScreen::onKeyfileClicked(const QString &filepath)
{
    showPasswordDialog(filepath);
}

void WelcomeScreen::showPasswordDialog(const QString &filepath)
{
    bool ok;
    QString password = QInputDialog::getText(this, 
        "지갑 열기",
        "비밀번호를 입력하세요:",
        QLineEdit::Password,
        QString(), &ok);
    
    if (!ok || password.isEmpty()) {
        return;
    }
    
    // Emit signal to show loading screen and decrypt in MainWindow
    emit loadingStarted(filepath, password);
}

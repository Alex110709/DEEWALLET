/**
 * DEE WALLET - Welcome Screen Implementation
 */

#include "WelcomeScreen.h"
#include "CreateWalletDialog.h"
#include "ImportWalletDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>

WelcomeScreen::WelcomeScreen(QWidget *parent)
    : QWidget(parent)
    , keyfileList(new QListWidget(this))
    , createButton(new QPushButton("Create Wallet", this))
    , importButton(new QPushButton("Import Wallet from File", this))
    , titleLabel(new QLabel("DEE WALLET", this))
    , subtitleLabel(new QLabel("Multi-chain Cryptocurrency Wallet", this))
{
    setupUI();
    refreshKeyfileList();
}

void WelcomeScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);

    // Title section
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 36px;
            font-weight: bold;
            color: #ffffff;
        }
    )");
    titleLabel->setAlignment(Qt::AlignCenter);

    subtitleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            color: #999999;
        }
    )");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addSpacing(20);

    // Keyfile list section
    auto *listLabel = new QLabel("Found Keyfiles:", this);
    listLabel->setStyleSheet("font-size: 14px; color: #cccccc;");
    mainLayout->addWidget(listLabel);

    keyfileList->setStyleSheet(R"(
        QListWidget {
            background-color: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 8px;
        }
        QListWidget::item {
            padding: 12px;
            border-radius: 6px;
            margin: 4px 0;
        }
        QListWidget::item:hover {
            background-color: #3a3a3a;
        }
        QListWidget::item:selected {
            background-color: #0066ff;
        }
    )");
    keyfileList->setMinimumHeight(200);
    mainLayout->addWidget(keyfileList);

    connect(keyfileList, &QListWidget::itemDoubleClicked,
            this, &WelcomeScreen::onKeyfileDoubleClicked);

    // Button section
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(16);

    createButton->setMinimumHeight(50);
    importButton->setMinimumHeight(50);
    importButton->setObjectName("secondaryButton");

    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(importButton);

    mainLayout->addLayout(buttonLayout);

    connect(createButton, &QPushButton::clicked, this, &WelcomeScreen::onCreateWallet);
    connect(importButton, &QPushButton::clicked, this, &WelcomeScreen::onImportWallet);
}

QVector<KeyfileInfo> WelcomeScreen::scanKeyfiles()
{
    QVector<KeyfileInfo> keyfiles;

    // Scan application directory
    QDir appDir(QApplication::applicationDirPath());
    QStringList filters;
    filters << "*.keyfile";

    QFileInfoList fileList = appDir.entryInfoList(filters, QDir::Files);

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
    keyfileList->clear();

    QVector<KeyfileInfo> keyfiles = scanKeyfiles();

    if (keyfiles.isEmpty()) {
        auto *item = new QListWidgetItem("No keyfiles found. Create a new wallet to get started.");
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        keyfileList->addItem(item);
        return;
    }

    for (const KeyfileInfo &info : keyfiles) {
        QString icon = info.isValid ? "🔐" : "⚠️";
        QString text = QString("%1 %2\n   %3  •  %4")
                          .arg(icon)
                          .arg(info.filename)
                          .arg(formatDate(info.updatedAt))
                          .arg(formatFileSize(info.size));

        auto *item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, info.filepath);
        keyfileList->addItem(item);
    }
}

QString WelcomeScreen::formatFileSize(qint64 bytes)
{
    if (bytes < 1024) {
        return QString("%1 B").arg(bytes);
    } else if (bytes < 1024 * 1024) {
        return QString("%1 KB").arg(bytes / 1024);
    } else {
        return QString("%1 MB").arg(bytes / (1024 * 1024));
    }
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
        refreshKeyfileList();
        emit walletCreated(dialog.getMnemonic());
    }
}

void WelcomeScreen::onImportWallet()
{
    ImportWalletDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshKeyfileList();
        emit walletImported(dialog.getMnemonic());
    }
}

void WelcomeScreen::onKeyfileDoubleClicked(QListWidgetItem *item)
{
    QString filepath = item->data(Qt::UserRole).toString();
    if (!filepath.isEmpty()) {
        emit keyfileSelected(filepath);
        // TODO: Open wallet with password prompt
    }
}

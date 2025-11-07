/**
 * DEE WALLET - Import Wallet Dialog Implementation
 */

#include "ImportWalletDialog.h"
#include "../core/KeyfileManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

ImportWalletDialog::ImportWalletDialog(QWidget *parent)
    : QDialog(parent)
    , filePathInput(new QLineEdit(this))
    , passwordInput(new QLineEdit(this))
    , browseButton(new QPushButton("Browse...", this))
    , importButton(new QPushButton("Import Wallet", this))
    , cancelButton(new QPushButton("Cancel", this))
    , fileInfoLabel(new QLabel(this))
{
    setWindowTitle("Import Wallet from Keyfile");
    setupUI();
}

void ImportWalletDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // Title
    auto *titleLabel = new QLabel("Import Wallet", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    // File selection section
    auto *fileLabel = new QLabel("Keyfile Location:", this);
    fileLabel->setStyleSheet("font-size: 14px; color: #cccccc;");
    mainLayout->addWidget(fileLabel);

    auto *fileLayout = new QHBoxLayout();

    filePathInput->setReadOnly(true);
    filePathInput->setPlaceholderText("Select a .keyfile...");
    filePathInput->setStyleSheet(R"(
        QLineEdit {
            background-color: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 12px;
            font-size: 14px;
        }
    )");
    fileLayout->addWidget(filePathInput);

    browseButton->setObjectName("secondaryButton");
    fileLayout->addWidget(browseButton);

    mainLayout->addLayout(fileLayout);

    // File info
    fileInfoLabel->setStyleSheet("font-size: 12px; color: #999999;");
    fileInfoLabel->setWordWrap(true);
    mainLayout->addWidget(fileInfoLabel);

    // Password section
    auto *passwordLabel = new QLabel("Decryption Password:", this);
    passwordLabel->setStyleSheet("font-size: 14px; color: #cccccc;");
    mainLayout->addWidget(passwordLabel);

    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setPlaceholderText("Enter keyfile password...");
    passwordInput->setStyleSheet(filePathInput->styleSheet());
    mainLayout->addWidget(passwordInput);

    // Button section
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);

    cancelButton->setObjectName("secondaryButton");
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(importButton);

    mainLayout->addLayout(buttonLayout);

    // Initially disable import button
    importButton->setEnabled(false);

    // Connections
    connect(browseButton, &QPushButton::clicked, this, &ImportWalletDialog::onBrowseFile);
    connect(importButton, &QPushButton::clicked, this, &ImportWalletDialog::onImportWallet);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    setMinimumWidth(600);
}

void ImportWalletDialog::onBrowseFile()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Select Keyfile",
        QString(),
        "Keyfile (*.keyfile);;All Files (*)"
    );

    if (!filePath.isEmpty()) {
        filePathInput->setText(filePath);

        // Display file info
        QFileInfo info(filePath);
        QString sizeStr;
        if (info.size() < 1024) {
            sizeStr = QString("%1 B").arg(info.size());
        } else {
            sizeStr = QString("%1 KB").arg(info.size() / 1024);
        }

        fileInfoLabel->setText(QString("File: %1  •  Size: %2  •  Modified: %3")
                                  .arg(info.fileName())
                                  .arg(sizeStr)
                                  .arg(info.lastModified().toString("MMM dd, yyyy")));

        importButton->setEnabled(true);
    }
}

void ImportWalletDialog::onImportWallet()
{
    QString filePath = filePathInput->text();
    QString password = passwordInput->text();

    // Validation
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a keyfile.");
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter the password.");
        return;
    }

    try {
        // Decrypt keyfile
        KeyfileManager keyfileManager;
        QByteArray decrypted = keyfileManager.loadAndDecrypt(filePath, password);

        if (decrypted.isEmpty()) {
            QMessageBox::critical(this, "Error",
                "Failed to decrypt keyfile.\n\n"
                "Possible reasons:\n"
                "• Incorrect password\n"
                "• Corrupted keyfile\n"
                "• Invalid file format");
            return;
        }

        // Parse JSON
        QJsonDocument doc = QJsonDocument::fromJson(decrypted);
        if (!doc.isObject()) {
            QMessageBox::critical(this, "Error", "Invalid keyfile format.");
            return;
        }

        QJsonObject keyfileData = doc.object();
        QString mnemonic = keyfileData["mnemonic"].toString();

        if (mnemonic.isEmpty()) {
            QMessageBox::critical(this, "Error", "No mnemonic found in keyfile.");
            return;
        }

        // Get wallet addresses
        QJsonObject wallets = keyfileData["wallets"].toObject();
        QString btcAddress = wallets["bitcoin"].toString();
        QString ethAddress = wallets["ethereum"].toString();
        QString trxAddress = wallets["tron"].toString();
        QString solAddress = wallets["solana"].toString();

        QMessageBox::information(this, "Success",
            QString("Wallet imported successfully!\n\n"
                    "Bitcoin: %1\n"
                    "Ethereum: %2\n"
                    "Tron: %3\n"
                    "Solana: %4")
            .arg(btcAddress.isEmpty() ? "Pending" : btcAddress)
            .arg(ethAddress.isEmpty() ? "Pending" : ethAddress)
            .arg(trxAddress.isEmpty() ? "Pending" : trxAddress)
            .arg(solAddress.isEmpty() ? "Pending" : solAddress));

        importedMnemonic = mnemonic;
        accept();

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error",
            QString("Failed to import wallet:\n%1").arg(e.what()));
    }
}

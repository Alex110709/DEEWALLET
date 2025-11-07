/**
 * DEE WALLET - Create Wallet Dialog Implementation
 */

#include "CreateWalletDialog.h"
#include "../core/WalletCore.h"
#include "../core/KeyfileManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QApplication>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>

CreateWalletDialog::CreateWalletDialog(QWidget *parent)
    : QDialog(parent)
    , mnemonicDisplay(new QTextEdit(this))
    , passwordInput(new QLineEdit(this))
    , confirmPasswordInput(new QLineEdit(this))
    , generateButton(new QPushButton("Generate Mnemonic", this))
    , createButton(new QPushButton("Create Wallet", this))
    , cancelButton(new QPushButton("Cancel", this))
{
    setWindowTitle("Create New Wallet");
    setupUI();
}

void CreateWalletDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // Title
    auto *titleLabel = new QLabel("Create New Wallet", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    // Mnemonic section
    auto *mnemonicLabel = new QLabel("Recovery Phrase (BIP39 Mnemonic):", this);
    mnemonicLabel->setStyleSheet("font-size: 14px; color: #cccccc;");
    mainLayout->addWidget(mnemonicLabel);

    mnemonicDisplay->setReadOnly(true);
    mnemonicDisplay->setStyleSheet(R"(
        QTextEdit {
            background-color: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 12px;
            font-size: 14px;
            font-family: monospace;
        }
    )");
    mnemonicDisplay->setPlaceholderText("Click 'Generate Mnemonic' to create a new recovery phrase...");
    mnemonicDisplay->setMinimumHeight(100);
    mainLayout->addWidget(mnemonicDisplay);

    mainLayout->addWidget(generateButton);

    // Warning
    auto *warningLabel = new QLabel("⚠️ Write down and store your recovery phrase safely. It cannot be recovered if lost.", this);
    warningLabel->setStyleSheet("color: #ff9900; font-size: 12px;");
    warningLabel->setWordWrap(true);
    mainLayout->addWidget(warningLabel);

    // Password section
    auto *passwordLabel = new QLabel("Encryption Password:", this);
    passwordLabel->setStyleSheet("font-size: 14px; color: #cccccc;");
    mainLayout->addWidget(passwordLabel);

    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setPlaceholderText("Enter strong password...");
    passwordInput->setStyleSheet(R"(
        QLineEdit {
            background-color: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 12px;
            font-size: 14px;
        }
    )");
    mainLayout->addWidget(passwordInput);

    confirmPasswordInput->setEchoMode(QLineEdit::Password);
    confirmPasswordInput->setPlaceholderText("Confirm password...");
    confirmPasswordInput->setStyleSheet(passwordInput->styleSheet());
    mainLayout->addWidget(confirmPasswordInput);

    // Button section
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);

    cancelButton->setObjectName("secondaryButton");
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(createButton);

    mainLayout->addLayout(buttonLayout);

    // Initially disable create button
    createButton->setEnabled(false);

    // Connections
    connect(generateButton, &QPushButton::clicked, this, &CreateWalletDialog::onGenerateMnemonic);
    connect(createButton, &QPushButton::clicked, this, &CreateWalletDialog::onCreateWallet);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    setMinimumWidth(600);
}

void CreateWalletDialog::onGenerateMnemonic()
{
    // Generate 12-word BIP39 mnemonic
    WalletCore wallet;
    QString mnemonic = wallet.generateMnemonic(12);

    mnemonicDisplay->setText(mnemonic);
    createButton->setEnabled(true);
}

void CreateWalletDialog::onCreateWallet()
{
    QString password = passwordInput->text();
    QString confirm = confirmPasswordInput->text();
    QString mnemonic = mnemonicDisplay->toPlainText();

    // Validation
    if (mnemonic.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please generate a mnemonic first.");
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a password.");
        return;
    }

    if (password != confirm) {
        QMessageBox::warning(this, "Error", "Passwords do not match.");
        return;
    }

    if (password.length() < 8) {
        QMessageBox::warning(this, "Error", "Password must be at least 8 characters.");
        return;
    }

    try {
        // Create wallet from mnemonic
        WalletCore wallet;
        if (!wallet.restoreFromMnemonic(mnemonic)) {
            QMessageBox::critical(this, "Error", "Failed to create wallet from mnemonic.");
            return;
        }

        // Generate addresses for all chains
        QString btcAddress = wallet.deriveAddress("BTC", 0);
        QString ethAddress = wallet.deriveAddress("ETH", 0);
        QString trxAddress = wallet.deriveAddress("TRX", 0);
        QString solAddress = wallet.deriveAddress("SOL", 0);

        // Create keyfile JSON
        QJsonObject keyfileData;
        keyfileData["version"] = 1;
        keyfileData["mnemonic"] = mnemonic;
        keyfileData["createdAt"] = QDateTime::currentMSecsSinceEpoch();

        QJsonObject walletsObj;
        walletsObj["bitcoin"] = btcAddress;
        walletsObj["ethereum"] = ethAddress;
        walletsObj["tron"] = trxAddress;
        walletsObj["solana"] = solAddress;
        keyfileData["wallets"] = walletsObj;

        QJsonDocument doc(keyfileData);
        QByteArray plaintext = doc.toJson();

        // Encrypt and save keyfile
        KeyfileManager keyfileManager;
        QString appDir = QApplication::applicationDirPath();
        QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
        QString filename = QString("wallet_%1.keyfile").arg(timestamp);
        QString filepath = appDir + "/" + filename;

        if (!keyfileManager.encryptAndSave(filepath, plaintext, password)) {
            QMessageBox::critical(this, "Error", "Failed to save keyfile.");
            return;
        }

        QMessageBox::information(this, "Success",
            QString("Wallet created successfully!\n\nKeyfile saved to:\n%1\n\n"
                    "⚠️ IMPORTANT: Write down your recovery phrase and store it safely!\n\n"
                    "Bitcoin: %2\n"
                    "Ethereum: %3\n"
                    "Tron: %4\n"
                    "Solana: %5")
            .arg(filename)
            .arg(btcAddress.isEmpty() ? "Pending" : btcAddress)
            .arg(ethAddress.isEmpty() ? "Pending" : ethAddress)
            .arg(trxAddress.isEmpty() ? "Pending" : trxAddress)
            .arg(solAddress.isEmpty() ? "Pending" : solAddress));

        createdMnemonic = mnemonic;
        accept();

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error",
            QString("Failed to create wallet:\n%1").arg(e.what()));
    }
}

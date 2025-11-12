/**
 * DEE WALLET - Create Wallet Dialog Implementation
 */

#include "CreateWalletDialog.h"
#include "../core/WalletCore.h"
#include "../core/KeyfileManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QApplication>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QScrollArea>

CreateWalletDialog::CreateWalletDialog(QWidget *parent)
    : QDialog(parent)
    , stackedWidget(new QStackedWidget(this))
    , passwordPage(new QWidget(this))
    , mnemonicPage(new QWidget(this))
    , passwordInput(new QLineEdit(this))
    , confirmPasswordInput(new QLineEdit(this))
    , nextButton(new QPushButton("Next", this))
    , cancelButton(new QPushButton("Cancel", this))
    , createButton(new QPushButton("Create Wallet", this))
    , backButton(new QPushButton("Back", this))
{
    setWindowTitle("Create New Wallet");
    setupUI();
}

void CreateWalletDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    setupPasswordPage();
    setupMnemonicPage();
    
    stackedWidget->addWidget(passwordPage);
    stackedWidget->addWidget(mnemonicPage);
    stackedWidget->setCurrentWidget(passwordPage);
    
    mainLayout->addWidget(stackedWidget);
    
    setMinimumWidth(700);
    setMinimumHeight(600);
}

void CreateWalletDialog::setupPasswordPage()
{
    auto *layout = new QVBoxLayout(passwordPage);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);
    
    // Title
    auto *titleLabel = new QLabel("새 지갑 만들기", this);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #ffffff;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    // Subtitle
    auto *subtitleLabel = new QLabel("지갑을 암호화할 비밀번호를 설정하세요", this);
    subtitleLabel->setStyleSheet("font-size: 14px; color: #cccccc;");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(subtitleLabel);
    
    layout->addSpacing(30);
    
    // Password input
    auto *passwordLabel = new QLabel("비밀번호:", this);
    passwordLabel->setStyleSheet("font-size: 14px; color: #cccccc;");
    layout->addWidget(passwordLabel);
    
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setPlaceholderText("8자 이상의 강력한 비밀번호를 입력하세요...");
    passwordInput->setStyleSheet(R"(
        QLineEdit {
            background-color: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 10px;
            padding: 15px;
            font-size: 16px;
            color: #ffffff;
        }
        QLineEdit:focus {
            border: 2px solid #4a9eff;
        }
    )");
    layout->addWidget(passwordInput);
    
    // Confirm password input
    auto *confirmLabel = new QLabel("비밀번호 확인:", this);
    confirmLabel->setStyleSheet("font-size: 14px; color: #cccccc;");
    layout->addWidget(confirmLabel);
    
    confirmPasswordInput->setEchoMode(QLineEdit::Password);
    confirmPasswordInput->setPlaceholderText("비밀번호를 다시 입력하세요...");
    confirmPasswordInput->setStyleSheet(passwordInput->styleSheet());
    layout->addWidget(confirmPasswordInput);
    
    layout->addSpacing(20);
    
    // Info box
    auto *infoBox = new QLabel("ℹ️  이 비밀번호는 지갑 파일을 암호화하는데 사용됩니다.\n"
                                "분실 시 복구할 수 없으므로 안전하게 보관하세요.", this);
    infoBox->setStyleSheet(R"(
        QLabel {
            background-color: #2a3a4a;
            border: 1px solid #3a4a5a;
            border-radius: 8px;
            padding: 15px;
            font-size: 13px;
            color: #b0c4de;
        }
    )");
    infoBox->setWordWrap(true);
    layout->addWidget(infoBox);
    
    layout->addStretch();
    
    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    
    cancelButton->setStyleSheet(R"(
        QPushButton {
            background-color: #3a3a3a;
            border: none;
            border-radius: 8px;
            padding: 12px 30px;
            font-size: 14px;
            color: #ffffff;
        }
        QPushButton:hover {
            background-color: #4a4a4a;
        }
    )");
    
    nextButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4a9eff;
            border: none;
            border-radius: 8px;
            padding: 12px 40px;
            font-size: 14px;
            font-weight: bold;
            color: #ffffff;
        }
        QPushButton:hover {
            background-color: #5aafff;
        }
        QPushButton:pressed {
            background-color: #3a8eef;
        }
    )");
    
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(nextButton);
    
    layout->addLayout(buttonLayout);
    
    // Connections
    connect(nextButton, &QPushButton::clicked, this, &CreateWalletDialog::onNextClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void CreateWalletDialog::setupMnemonicPage()
{
    auto *layout = new QVBoxLayout(mnemonicPage);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);
    
    // Title
    auto *titleLabel = new QLabel("복구 문구 (Recovery Phrase)", this);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #ffffff;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    // Warning
    auto *warningLabel = new QLabel("⚠️  이 24개의 단어를 안전한 곳에 적어서 보관하세요!\n"
                                    "지갑을 복구할 수 있는 유일한 방법입니다.", this);
    warningLabel->setStyleSheet(R"(
        QLabel {
            background-color: #4a3a2a;
            border: 2px solid #ff9900;
            border-radius: 8px;
            padding: 15px;
            font-size: 14px;
            color: #ffcc66;
            font-weight: bold;
        }
    )");
    warningLabel->setWordWrap(true);
    warningLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(warningLabel);
    
    layout->addSpacing(10);
    
    // Scroll area for mnemonic words
    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            border: none;
            background-color: transparent;
        }
    )");
    
    auto *scrollWidget = new QWidget();
    mnemonicGrid = new QGridLayout(scrollWidget);
    mnemonicGrid->setSpacing(12);
    mnemonicGrid->setContentsMargins(0, 0, 0, 0);
    
    scrollArea->setWidget(scrollWidget);
    layout->addWidget(scrollArea, 1);
    
    layout->addSpacing(10);
    
    // Info text
    auto *infoLabel = new QLabel("💡  단어를 순서대로 종이에 적어 안전한 장소에 보관하세요.\n"
                                 "디지털 형태로 저장하거나 스크린샷을 찍지 마세요.", this);
    infoLabel->setStyleSheet("font-size: 13px; color: #999999;");
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);
    
    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    
    backButton->setStyleSheet(R"(
        QPushButton {
            background-color: #3a3a3a;
            border: none;
            border-radius: 8px;
            padding: 12px 30px;
            font-size: 14px;
            color: #ffffff;
        }
        QPushButton:hover {
            background-color: #4a4a4a;
        }
    )");
    
    createButton->setStyleSheet(R"(
        QPushButton {
            background-color: #28a745;
            border: none;
            border-radius: 8px;
            padding: 12px 40px;
            font-size: 14px;
            font-weight: bold;
            color: #ffffff;
        }
        QPushButton:hover {
            background-color: #34b755;
        }
        QPushButton:pressed {
            background-color: #229735;
        }
    )");
    
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(createButton);
    
    layout->addLayout(buttonLayout);
    
    // Connections
    connect(backButton, &QPushButton::clicked, this, &CreateWalletDialog::onBackClicked);
    connect(createButton, &QPushButton::clicked, this, &CreateWalletDialog::onCreateWallet);
}

void CreateWalletDialog::showMnemonicWords(const QString &mnemonic)
{
    // Clear existing labels
    for (auto *label : wordLabels) {
        delete label;
    }
    wordLabels.clear();
    
    // Split mnemonic into words
    QStringList words = mnemonic.split(" ", Qt::SkipEmptyParts);
    
    // Create labels for each word (4 columns)
    int columns = 4;
    for (int i = 0; i < words.size(); i++) {
        auto *wordBox = new QLabel(this);
        
        QString wordText = QString("<span style='color: #888888; font-size: 12px;'>%1.</span><br>"
                                   "<span style='font-size: 16px; font-weight: bold;'>%2</span>")
                          .arg(i + 1)
                          .arg(words[i]);
        
        wordBox->setText(wordText);
        wordBox->setAlignment(Qt::AlignCenter);
        wordBox->setStyleSheet(R"(
            QLabel {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                           stop:0 #3a3a3a,
                                           stop:1 #2a2a2a);
                border: 2px solid #4a4a4a;
                border-radius: 10px;
                padding: 15px 10px;
                min-height: 60px;
                color: #ffffff;
            }
        )");
        
        int row = i / columns;
        int col = i % columns;
        mnemonicGrid->addWidget(wordBox, row, col);
        wordLabels.append(wordBox);
    }
}

void CreateWalletDialog::onNextClicked()
{
    QString password = passwordInput->text();
    QString confirm = confirmPasswordInput->text();
    
    // Validation
    if (password.isEmpty()) {
        QMessageBox::warning(this, "오류", "비밀번호를 입력하세요.");
        return;
    }
    
    if (password.length() < 8) {
        QMessageBox::warning(this, "오류", "비밀번호는 최소 8자 이상이어야 합니다.");
        return;
    }
    
    if (password != confirm) {
        QMessageBox::warning(this, "오류", "비밀번호가 일치하지 않습니다.");
        return;
    }
    
    // Generate 24-word mnemonic
    WalletCore wallet;
    QString mnemonic = wallet.generateMnemonic(24); // 24 words
    createdMnemonic = mnemonic;
    
    // Show mnemonic page
    showMnemonicWords(mnemonic);
    stackedWidget->setCurrentWidget(mnemonicPage);
}

void CreateWalletDialog::onBackClicked()
{
    stackedWidget->setCurrentWidget(passwordPage);
}

void CreateWalletDialog::onCreateWallet()
{
    QString password = passwordInput->text();
    QString mnemonic = createdMnemonic;
    
    if (mnemonic.isEmpty()) {
        QMessageBox::warning(this, "오류", "복구 문구가 생성되지 않았습니다.");
        return;
    }
    
    try {
        // Create wallet from mnemonic
        WalletCore wallet;
        if (!wallet.restoreFromMnemonic(mnemonic)) {
            QMessageBox::critical(this, "오류", "복구 문구에서 지갑을 생성하지 못했습니다.");
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
        // Encrypt and save keyfile to application directory
        KeyfileManager keyfileManager;
        QString appDir = QApplication::applicationDirPath();
        QDir appDirObj(appDir);
        appDirObj.cdUp(); // Contents
        appDirObj.cdUp(); // deewallet.app
        appDirObj.cdUp(); // build (deewallet.app이 있는 폴더)
        QString buildDir = appDirObj.absolutePath();
        QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
        QString filename = QString("wallet_%1.keyfile").arg(timestamp);
        QString filepath = buildDir + "/" + filename;
        
        if (!keyfileManager.encryptAndSave(filepath, plaintext, password)) {
            QMessageBox::critical(this, "오류", "키파일 저장에 실패했습니다.");
            return;
        }
        
        QMessageBox::information(this, "성공",
            QString("지갑이 성공적으로 생성되었습니다!\n\n"
                    "키파일 저장 위치 (애플리케이션 폴더):\n%1\n\n"
                    "⚠️ 중요: 복구 문구를 안전하게 보관하세요!\n\n"
                    "Bitcoin: %2\n"
                    "Ethereum: %3\n"
                    "Tron: %4\n"
                    "Solana: %5")
            .arg(filepath)
            .arg(btcAddress.isEmpty() ? "대기 중" : btcAddress)
            .arg(ethAddress.isEmpty() ? "대기 중" : ethAddress)
            .arg(trxAddress.isEmpty() ? "대기 중" : trxAddress)
            .arg(solAddress.isEmpty() ? "대기 중" : solAddress));
        
        accept();
        
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "오류",
            QString("지갑 생성 실패:\n%1").arg(e.what()));
    }
}

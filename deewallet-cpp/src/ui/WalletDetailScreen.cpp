/**
 * DEE WALLET - Wallet Detail Screen Implementation
 */

#include "WalletDetailScreen.h"
#include "SendTransactionDialog.h"
#include "QRCodeDialog.h"
#include "AddressBookDialog.h"
#include "../chains/BitcoinAdapter.h"
#include "../chains/EthereumAdapter.h"
#include "../chains/TronAdapter.h"
#include "../chains/SolanaAdapter.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>

WalletDetailScreen::WalletDetailScreen(const QString &mnemonic, QWidget *parent)
    : QWidget(parent)
    , mnemonic(mnemonic)
    , totalBalanceLabel(new QLabel("$0.00", this))
    , chainTabs(new QTabWidget(this))
    , refreshButton(new QPushButton("Refresh", this))
    , sendButton(new QPushButton("Send", this))
    , receiveButton(new QPushButton("Receive", this))
    , exportButton(new QPushButton("Export Keyfile", this))
    , backButton(new QPushButton("← Back", this))
{
    setupUI();
    loadWallet();
    refreshBalances();
}

void WalletDetailScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // Top bar with back button
    auto *topBar = new QHBoxLayout();
    backButton->setObjectName("secondaryButton");
    backButton->setMaximumWidth(100);
    topBar->addWidget(backButton);
    topBar->addStretch();

    auto *titleLabel = new QLabel("DEE WALLET", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    topBar->addWidget(titleLabel);
    topBar->addStretch();

    mainLayout->addLayout(topBar);

    // Total balance section
    auto *balanceCard = new QWidget(this);
    balanceCard->setStyleSheet(R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #667eea, stop:1 #764ba2);
            border-radius: 12px;
            padding: 20px;
        }
    )");

    auto *balanceLayout = new QVBoxLayout(balanceCard);
    auto *balanceTitleLabel = new QLabel("Total Balance", this);
    balanceTitleLabel->setStyleSheet("font-size: 14px; color: rgba(255,255,255,0.8);");

    totalBalanceLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: white;");

    balanceLayout->addWidget(balanceTitleLabel);
    balanceLayout->addWidget(totalBalanceLabel);

    mainLayout->addWidget(balanceCard);

    // Action buttons
    auto *actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(12);

    refreshButton->setMinimumHeight(45);
    sendButton->setMinimumHeight(45);
    receiveButton->setMinimumHeight(45);
    exportButton->setMinimumHeight(45);
    exportButton->setObjectName("secondaryButton");

    actionLayout->addWidget(refreshButton);
    actionLayout->addWidget(sendButton);
    actionLayout->addWidget(receiveButton);
    actionLayout->addWidget(exportButton);

    mainLayout->addLayout(actionLayout);

    // Chain tabs
    chainTabs->setStyleSheet(R"(
        QTabWidget::pane {
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            background: #2a2a2a;
        }
        QTabBar::tab {
            background: #2a2a2a;
            color: #999999;
            padding: 12px 24px;
            border: none;
        }
        QTabBar::tab:selected {
            background: #3a3a3a;
            color: #ffffff;
        }
    )");

    mainLayout->addWidget(chainTabs);

    // Initialize chain data
    chains = {
        {"Bitcoin", "", "0.0", nullptr, nullptr, nullptr, nullptr},
        {"Ethereum", "", "0.0", nullptr, nullptr, nullptr, nullptr},
        {"Tron", "", "0.0", nullptr, nullptr, nullptr, nullptr},
        {"Solana", "", "0.0", nullptr, nullptr, nullptr, nullptr}
    };

    // Create tabs for each chain
    for (int i = 0; i < chains.size(); ++i) {
        auto *chainWidget = new QWidget(this);
        auto *chainLayout = new QVBoxLayout(chainWidget);
        chainLayout->setSpacing(20);

        // Address section
        auto *addressCard = new QWidget(chainWidget);
        addressCard->setStyleSheet(R"(
            QWidget {
                background: #1a1a1a;
                border-radius: 8px;
                padding: 16px;
            }
        )");
        auto *addressLayout = new QVBoxLayout(addressCard);

        auto *addressTitle = new QLabel("Address:", chainWidget);
        addressTitle->setStyleSheet("font-size: 12px; color: #999999;");

        auto *addressLabel = new QLabel("Loading...", chainWidget);
        addressLabel->setStyleSheet("font-size: 14px; font-family: monospace; color: #ffffff;");
        addressLabel->setWordWrap(true);

        auto *copyButton = new QPushButton("Copy Address", chainWidget);
        copyButton->setObjectName("secondaryButton");
        copyButton->setMaximumWidth(150);

        addressLayout->addWidget(addressTitle);
        addressLayout->addWidget(addressLabel);
        addressLayout->addWidget(copyButton);

        // Balance section
        auto *balanceCard = new QWidget(chainWidget);
        balanceCard->setStyleSheet(addressCard->styleSheet());
        auto *balanceLayout = new QVBoxLayout(balanceCard);

        auto *balanceTitle = new QLabel("Balance:", chainWidget);
        balanceTitle->setStyleSheet("font-size: 12px; color: #999999;");

        auto *balanceLabel = new QLabel("0.0", chainWidget);
        balanceLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #ffffff;");

        balanceLayout->addWidget(balanceTitle);
        balanceLayout->addWidget(balanceLabel);

        chainLayout->addWidget(addressCard);
        chainLayout->addWidget(balanceCard);
        chainLayout->addStretch();

        chains[i].widget = chainWidget;
        chains[i].addressLabel = addressLabel;
        chains[i].balanceLabel = balanceLabel;
        chains[i].copyButton = copyButton;

        chainTabs->addTab(chainWidget, chains[i].name);

        // Connect copy button
        connect(copyButton, &QPushButton::clicked, this, [this, i]() {
            onCopyAddress(chains[i].address);
        });
    }

    // Connect buttons
    connect(backButton, &QPushButton::clicked, this, &WalletDetailScreen::onBackClicked);
    connect(refreshButton, &QPushButton::clicked, this, &WalletDetailScreen::onRefreshBalances);
    connect(sendButton, &QPushButton::clicked, this, &WalletDetailScreen::onSendTransaction);
    connect(receiveButton, &QPushButton::clicked, this, &WalletDetailScreen::onReceive);
    connect(exportButton, &QPushButton::clicked, this, &WalletDetailScreen::onExportKeyfile);
}

void WalletDetailScreen::loadWallet()
{
    if (!wallet.restoreFromMnemonic(mnemonic)) {
        QMessageBox::critical(this, "Error", "Failed to restore wallet from mnemonic.");
        return;
    }

    // Generate addresses for all chains
    chains[0].address = wallet.deriveAddress("BTC", 0);
    chains[1].address = wallet.deriveAddress("ETH", 0);
    chains[2].address = wallet.deriveAddress("TRX", 0);
    chains[3].address = wallet.deriveAddress("SOL", 0);

    // Update UI
    for (int i = 0; i < chains.size(); ++i) {
        if (chains[i].addressLabel) {
            chains[i].addressLabel->setText(chains[i].address);
        }
    }
}

void WalletDetailScreen::refreshBalances()
{
    // Fetch real balances from blockchain APIs
    for (int i = 0; i < chains.size(); ++i) {
        QString balance = "0.0";
        QString symbol;

        try {
            if (i == 0) {
                // Bitcoin
                BitcoinAdapter adapter("", false);
                balance = adapter.getBalance(chains[i].address);
                symbol = "BTC";
            } else if (i == 1) {
                // Ethereum
                EthereumAdapter adapter("");
                balance = adapter.getBalance(chains[i].address);
                symbol = "ETH";
            } else if (i == 2) {
                // Tron
                TronAdapter adapter("");
                balance = adapter.getBalance(chains[i].address);
                symbol = "TRX";
            } else if (i == 3) {
                // Solana
                SolanaAdapter adapter("");
                balance = adapter.getBalance(chains[i].address);
                symbol = "SOL";
            }

            chains[i].balance = balance;
            if (chains[i].balanceLabel) {
                chains[i].balanceLabel->setText(QString("%1 %2").arg(balance).arg(symbol));
            }
        } catch (...) {
            // If balance query fails, keep showing 0.0
            chains[i].balance = "0.0";
            if (chains[i].balanceLabel) {
                chains[i].balanceLabel->setText(QString("0.0 %1").arg(symbol));
            }
        }
    }

    // Calculate total in USD (placeholder conversion rates)
    double totalUSD = 0.0;
    // Note: In production, fetch real exchange rates from API
    totalUSD += chains[0].balance.toDouble() * 45000;  // BTC ~ $45k
    totalUSD += chains[1].balance.toDouble() * 2500;   // ETH ~ $2.5k
    totalUSD += chains[2].balance.toDouble() * 0.10;   // TRX ~ $0.10
    totalUSD += chains[3].balance.toDouble() * 100;    // SOL ~ $100

    totalBalanceLabel->setText(QString("$%1").arg(totalUSD, 0, 'f', 2));
}

void WalletDetailScreen::updateChainCard(int index, const QString &chain, const QString &address, const QString &balance)
{
    if (index >= 0 && index < chains.size()) {
        chains[index].address = address;
        chains[index].balance = balance;

        if (chains[index].addressLabel) {
            chains[index].addressLabel->setText(address);
        }
        if (chains[index].balanceLabel) {
            chains[index].balanceLabel->setText(balance);
        }
    }
}

void WalletDetailScreen::onRefreshBalances()
{
    // Show loading indicator
    refreshButton->setEnabled(false);
    refreshButton->setText("Refreshing...");

    refreshBalances();

    refreshButton->setEnabled(true);
    refreshButton->setText("Refresh");

    QMessageBox::information(this, "Success",
        "Balances refreshed successfully!\n\n"
        "Bitcoin: " + chains[0].balance + " BTC\n"
        "Ethereum: " + chains[1].balance + " ETH\n"
        "Tron: " + chains[2].balance + " TRX\n"
        "Solana: " + chains[3].balance + " SOL");
}

void WalletDetailScreen::onSendTransaction()
{
    int currentTab = chainTabs->currentIndex();
    if (currentTab >= 0 && currentTab < chains.size()) {
        QString chainType = chains[currentTab].name;
        QString fromAddress = chains[currentTab].address;

        SendTransactionDialog dialog(chainType, fromAddress, mnemonic, this);
        if (dialog.exec() == QDialog::Accepted) {
            // Transaction sent, refresh balances
            refreshBalances();
        }
    }
}

void WalletDetailScreen::onReceive()
{
    int currentTab = chainTabs->currentIndex();
    if (currentTab >= 0 && currentTab < chains.size()) {
        QString address = chains[currentTab].address;
        QString chainName = chains[currentTab].name;

        QRCodeDialog dialog(address, chainName, this);
        dialog.exec();
    }
}

void WalletDetailScreen::onCopyAddress(const QString &address)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(address);

    QMessageBox::information(this, "Copied",
        "Address copied to clipboard:\n\n" + address);
}

void WalletDetailScreen::onExportKeyfile()
{
    QMessageBox::information(this, "Export Keyfile",
        "Keyfile export functionality.\n\n"
        "This will:\n"
        "• Prompt for password\n"
        "• Encrypt mnemonic and keys\n"
        "• Save to selected location\n"
        "• Show backup reminder");
}

void WalletDetailScreen::onBackClicked()
{
    emit backToWelcome();
}

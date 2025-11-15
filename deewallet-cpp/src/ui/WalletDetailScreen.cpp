/**
 * DEE WALLET - Wallet Detail Screen Implementation
 */

#include "WalletDetailScreen.h"
#include "SendTransactionDialog.h"
#include "QRCodeDialog.h"
#include "AddressBookDialog.h"
#include "StyleHelper.h"
#include "DesignTokens.h"
#include "../chains/BitcoinAdapter.h"
#include "../chains/EthereumAdapter.h"
#include "../chains/TronAdapter.h"
#include "../chains/SolanaAdapter.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>

WalletDetailScreen::WalletDetailScreen(const QString &mnemonic, QWidget *parent)
    : QWidget(parent)
    , mnemonic(mnemonic)
    , chainListContainer(new QWidget(this))
    , chainListLayout(new QVBoxLayout(chainListContainer))
    , refreshButton(new QPushButton("새로고침", this))
    , backButton(new QPushButton("← 뒤로", this))
{
    setupUI();
    loadWallet();
    refreshBalances();
}

void WalletDetailScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 25, 30, 25);
    mainLayout->setSpacing(20);

    // Top bar - cleaner layout
    auto *topBar = new QHBoxLayout();

    auto *titleLabel = new QLabel("DEE WALLET", this);
    titleLabel->setStyleSheet(StyleHelper::headingLabel());
    topBar->addWidget(titleLabel);
    topBar->addStretch();

    backButton->setObjectName("secondaryButton");
    backButton->setMaximumWidth(100);
    backButton->setMinimumHeight(DesignTokens::Sizes::BUTTON_HEIGHT_SM);
    backButton->setStyleSheet(StyleHelper::secondaryButton());
    topBar->addWidget(backButton);

    mainLayout->addLayout(topBar);

    // Refresh button - cleaner design
    refreshButton->setMinimumHeight(DesignTokens::Sizes::BUTTON_HEIGHT_MD);
    refreshButton->setCursor(Qt::PointingHandCursor);
    refreshButton->setStyleSheet(StyleHelper::primaryButton());

    mainLayout->addWidget(refreshButton);

    // Chain list title
    auto *listTitle = new QLabel("보유 자산", this);
    listTitle->setStyleSheet(StyleHelper::subheadingLabel());
    mainLayout->addWidget(listTitle);

    // Chain list container with scroll
    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(chainListContainer);
    scrollArea->setStyleSheet(StyleHelper::scrollArea());

    // Set container background to transparent
    chainListContainer->setStyleSheet(StyleHelper::transparentBackground());
    
    chainListLayout->setSpacing(12);
    chainListLayout->setAlignment(Qt::AlignTop);
    
    mainLayout->addWidget(scrollArea, 1);

    // Initialize chain data
    chains = {
        // Non-EVM
        {"Bitcoin", "BTC", "", "0.0", nullptr, nullptr},
        {"Litecoin", "LTC", "", "0.0", nullptr, nullptr},
        {"Dogecoin", "DOGE", "", "0.0", nullptr, nullptr},
        {"Tron", "TRX", "", "0.0", nullptr, nullptr},
        {"Solana", "SOL", "", "0.0", nullptr, nullptr},
        
        // EVM Chains
        {"Ethereum", "ETH", "", "0.0", nullptr, nullptr},
        {"BNB Chain", "BNB", "", "0.0", nullptr, nullptr},
        {"Polygon", "POL", "", "0.0", nullptr, nullptr},
        {"Arbitrum", "ARB", "", "0.0", nullptr, nullptr},
        {"Optimism", "OP", "", "0.0", nullptr, nullptr},
        {"Avalanche", "AVAX", "", "0.0", nullptr, nullptr},
        {"Base", "BASE", "", "0.0", nullptr, nullptr},
        {"Fantom", "FTM", "", "0.0", nullptr, nullptr},
        {"Cronos", "CRO", "", "0.0", nullptr, nullptr},
        {"Gnosis", "xDAI", "", "0.0", nullptr, nullptr}
    };

    // Create card for each chain
    for (int i = 0; i < chains.size(); ++i) {
        auto *chainCard = new QWidget(chainListContainer);
        chainCard->setMinimumHeight(DesignTokens::Sizes::CARD_MIN_HEIGHT_MD);
        chainCard->setCursor(Qt::PointingHandCursor);
        chainCard->setStyleSheet(StyleHelper::interactiveCard());

        auto *cardLayout = new QHBoxLayout(chainCard);
        cardLayout->setContentsMargins(DesignTokens::Spacing::SPACING_XL,
                                       DesignTokens::Spacing::SPACING_LG,
                                       DesignTokens::Spacing::SPACING_XL,
                                       DesignTokens::Spacing::SPACING_LG);
        cardLayout->setSpacing(DesignTokens::Spacing::SPACING_LG);

        // Left side: Chain info
        auto *leftLayout = new QVBoxLayout();
        leftLayout->setSpacing(DesignTokens::Spacing::SPACING_XS);

        auto *chainNameLabel = new QLabel(chains[i].name + " (" + chains[i].symbol + ")", chainCard);
        chainNameLabel->setStyleSheet(QString(R"(
            QLabel {
                background: transparent;
                font-size: %1px;
                font-weight: %2;
                color: %3;
                border: none;
            }
        )")
            .arg(DesignTokens::Typography::FONT_SIZE_LG)
            .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
            .arg(DesignTokens::Colors::TEXT_BODY));

        auto *tapHintLabel = new QLabel("탭하여 주소 관리", chainCard);
        tapHintLabel->setStyleSheet(StyleHelper::hintLabel());

        leftLayout->addWidget(chainNameLabel);
        leftLayout->addWidget(tapHintLabel);
        leftLayout->addStretch();

        cardLayout->addLayout(leftLayout, 1);

        // Right side: Balance
        auto *rightLayout = new QVBoxLayout();
        rightLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        auto *balanceLabel = new QLabel("0.0 " + chains[i].symbol, chainCard);
        balanceLabel->setStyleSheet(StyleHelper::balanceLabel());
        balanceLabel->setAlignment(Qt::AlignRight);

        rightLayout->addWidget(balanceLabel);

        cardLayout->addLayout(rightLayout);

        chains[i].cardWidget = chainCard;
        chains[i].balanceLabel = balanceLabel;

        chainListLayout->addWidget(chainCard);

        // Make card clickable
        chainCard->setProperty("clickable", true);
        chainCard->setProperty("chainIndex", i);
        chainCard->installEventFilter(this);
    }
    
    chainListLayout->addStretch();

    // Connect buttons
    connect(backButton, &QPushButton::clicked, this, &WalletDetailScreen::onBackClicked);
    connect(refreshButton, &QPushButton::clicked, this, &WalletDetailScreen::onRefreshBalances);
}

void WalletDetailScreen::loadWallet()
{
    if (!wallet.restoreFromMnemonic(mnemonic)) {
        QMessageBox::critical(this, "오류", "복구 문구에서 지갑을 복원하지 못했습니다.");
        return;
    }

    // Generate addresses for all chains (index 0 only, user can add more)
    chains[0].address = wallet.deriveAddress("BTC", 0);  // Bitcoin
    chains[1].address = wallet.deriveAddress("ETH", 0);  // Ethereum
    chains[2].address = wallet.deriveAddress("TRX", 0);  // Tron
    chains[3].address = wallet.deriveAddress("SOL", 0);  // Solana
    chains[4].address = wallet.deriveAddress("LTC", 0);  // Litecoin
    chains[5].address = wallet.deriveAddress("DOGE", 0); // Dogecoin
}

void WalletDetailScreen::refreshBalances()
{
    // Fetch real balances from blockchain APIs
    for (int i = 0; i < chains.size(); ++i) {
        QString balance = "0.0";

        try {
            if (i == 0) {
                // Bitcoin
                BitcoinAdapter adapter("", false);
                balance = adapter.getBalance(chains[i].address);
            } else if (i == 1) {
                // Ethereum
                EthereumAdapter adapter("");
                balance = adapter.getBalance(chains[i].address);
            } else if (i == 2) {
                // Tron
                TronAdapter adapter("");
                balance = adapter.getBalance(chains[i].address);
            } else if (i == 3) {
                // Solana
                SolanaAdapter adapter("");
                balance = adapter.getBalance(chains[i].address);
            } else if (i == 4) {
                // Litecoin - use Bitcoin adapter (same format)
                BitcoinAdapter adapter("", false);
                balance = adapter.getBalance(chains[i].address);
            } else if (i == 5) {
                // Dogecoin - use Bitcoin adapter (same format)
                BitcoinAdapter adapter("", false);
                balance = adapter.getBalance(chains[i].address);
            }

            chains[i].balance = balance;
            if (chains[i].balanceLabel) {
                chains[i].balanceLabel->setText(balance + " " + chains[i].symbol);
            }
        } catch (...) {
            // If balance query fails, keep showing 0.0
            chains[i].balance = "0.0";
            if (chains[i].balanceLabel) {
                chains[i].balanceLabel->setText("0.0 " + chains[i].symbol);
            }
        }
    }
}

void WalletDetailScreen::updateChainCard(int index, const QString &chain, const QString &address, const QString &balance)
{
    if (index >= 0 && index < chains.size()) {
        chains[index].address = address;
        chains[index].balance = balance;

        if (chains[index].balanceLabel) {
            chains[index].balanceLabel->setText(balance + " " + chains[index].symbol);
        }
    }
}

void WalletDetailScreen::onRefreshBalances()
{
    // Show loading indicator
    refreshButton->setEnabled(false);
    refreshButton->setText("새로고침 중...");

    refreshBalances();

    refreshButton->setEnabled(true);
    refreshButton->setText("새로고침");

    QString message = "잔액을 갱신했습니다!\n\n";
    for (int i = 0; i < chains.size(); ++i) {
        message += chains[i].name + ": " + chains[i].balance + " " + chains[i].symbol + "\n";
    }
    
    QMessageBox::information(this, "완료", message);
}


void WalletDetailScreen::onCopyAddress(const QString &address)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(address);

    QMessageBox::information(this, "복사 완료",
        "주소가 클립보드에 복사되었습니다:\n\n" + address);
}


void WalletDetailScreen::onBackClicked()
{
    emit backToWelcome();
}
bool WalletDetailScreen::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *widget = qobject_cast<QWidget*>(obj);
    
    if (widget && widget->property("clickable").toBool()) {
        if (event->type() == QEvent::MouseButtonPress) {
            int chainIndex = widget->property("chainIndex").toInt();
            onChainCardClicked(chainIndex);
            return true;
        }
    }
    
    return QWidget::eventFilter(obj, event);
}

void WalletDetailScreen::onChainCardClicked(int chainIndex)
{
    if (chainIndex < 0 || chainIndex >= chains.size()) {
        return;
    }
    
    // Emit signal to navigate to ChainDetailScreen
    emit chainClicked(chains[chainIndex].name, 
                     chains[chainIndex].symbol, 
                     mnemonic);
}

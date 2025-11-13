/**
 * DEE WALLET - Chain Detail Screen Implementation
 */

#include "ChainDetailScreen.h"
#include "SendTransactionDialog.h"
#include "QRCodeDialog.h"
#include "StyleHelper.h"
#include "DesignTokens.h"
#include "../chains/BitcoinAdapter.h"
#include "../chains/EthereumAdapter.h"
#include "../chains/TronAdapter.h"
#include "../chains/SolanaAdapter.h"
#include <QScrollArea>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>

ChainDetailScreen::ChainDetailScreen(const QString &chainName,
                                     const QString &chainSymbol,
                                     const QString &mnemonic,
                                     QWidget *parent)
    : QWidget(parent)
    , chainName(chainName)
    , chainSymbol(chainSymbol)
    , mnemonic(mnemonic)
    , addressListContainer(new QWidget(this))
    , addressListLayout(new QVBoxLayout(addressListContainer))
    , addAddressButton(new QPushButton("+ 주소 추가", this))
    , sendButton(new QPushButton("전송", this))
    , receiveButton(new QPushButton("받기", this))
    , backButton(new QPushButton("← 뒤로", this))
    , chainTitleLabel(new QLabel(chainName + " (" + chainSymbol + ")", this))
    , totalBalanceLabel(new QLabel("스캔 중...", this))
    , transactionTable(new QTableWidget(this))
{
    setupUI();
    
    // Start address scanning in background
    QTimer::singleShot(100, this, [this]() {
        scanAddressesWithBalance();
    });
    
    loadTransactionHistory();
}

void ChainDetailScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 25, 30, 25);
    mainLayout->setSpacing(20);

    // Top bar
    auto *topBar = new QHBoxLayout();
    backButton->setObjectName("secondaryButton");
    backButton->setMinimumHeight(DesignTokens::Sizes::BUTTON_HEIGHT_SM);
    backButton->setMaximumWidth(100);
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(StyleHelper::secondaryButton());

    topBar->addWidget(backButton);
    topBar->addStretch();

    mainLayout->addLayout(topBar);

    // Title with Balance
    auto *titleBalanceLayout = new QVBoxLayout();
    titleBalanceLayout->setSpacing(DesignTokens::Spacing::SPACING_SM);

    chainTitleLabel->setStyleSheet(QString(R"(
        QLabel {
            background: transparent;
            font-size: %1px;
            font-weight: %2;
            color: %3;
            border: none;
        }
    )")
        .arg(DesignTokens::Typography::FONT_SIZE_3XL)
        .arg(DesignTokens::Typography::FONT_WEIGHT_BOLD)
        .arg(DesignTokens::Colors::TEXT_PRIMARY));
    chainTitleLabel->setAlignment(Qt::AlignCenter);

    totalBalanceLabel->setStyleSheet(QString(R"(
        QLabel {
            background: transparent;
            font-size: %1px;
            font-weight: %2;
            color: %3;
            border: none;
        }
    )")
        .arg(DesignTokens::Typography::FONT_SIZE_4XL)
        .arg(DesignTokens::Typography::FONT_WEIGHT_EXTRABOLD)
        .arg(DesignTokens::Colors::BRAND_PRIMARY));
    totalBalanceLabel->setAlignment(Qt::AlignCenter);

    titleBalanceLayout->addWidget(chainTitleLabel);
    titleBalanceLayout->addWidget(totalBalanceLabel);
    mainLayout->addLayout(titleBalanceLayout);

    // Send / Receive buttons
    auto *actionButtonLayout = new QHBoxLayout();
    actionButtonLayout->setSpacing(DesignTokens::Spacing::SPACING_MD);

    sendButton->setMinimumHeight(DesignTokens::Sizes::BUTTON_HEIGHT_LG);
    sendButton->setCursor(Qt::PointingHandCursor);
    sendButton->setStyleSheet(StyleHelper::primaryButton());

    receiveButton->setMinimumHeight(DesignTokens::Sizes::BUTTON_HEIGHT_LG);
    receiveButton->setCursor(Qt::PointingHandCursor);
    receiveButton->setStyleSheet(StyleHelper::successButton());

    actionButtonLayout->addWidget(sendButton);
    actionButtonLayout->addWidget(receiveButton);
    mainLayout->addLayout(actionButtonLayout);

    // Transaction History
    auto *historyTitle = new QLabel("거래 내역", this);
    historyTitle->setStyleSheet(StyleHelper::subheadingLabel());
    mainLayout->addWidget(historyTitle);

    transactionTable->setColumnCount(4);
    transactionTable->setHorizontalHeaderLabels({"날짜", "유형", "금액", "상태"});
    transactionTable->horizontalHeader()->setStretchLastSection(true);
    transactionTable->verticalHeader()->setVisible(false);
    transactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionTable->setMinimumHeight(150);
    transactionTable->setMaximumHeight(250);
    transactionTable->setStyleSheet(StyleHelper::tableWidget());
    mainLayout->addWidget(transactionTable);

    // Add address button
    addAddressButton->setMinimumHeight(DesignTokens::Sizes::BUTTON_HEIGHT_MD);
    addAddressButton->setCursor(Qt::PointingHandCursor);
    addAddressButton->setStyleSheet(StyleHelper::customButton(
        DesignTokens::Colors::BG_HOVER,
        DesignTokens::Colors::BG_ACTIVE));
    mainLayout->addWidget(addAddressButton);

    // Address list title
    auto *listTitle = new QLabel("주소 목록", this);
    listTitle->setStyleSheet(StyleHelper::subheadingLabel());
    mainLayout->addWidget(listTitle);

    // Scroll area for addresses
    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(addressListContainer);
    scrollArea->setStyleSheet(StyleHelper::scrollArea());

    // Set container background to transparent
    addressListContainer->setStyleSheet(StyleHelper::transparentBackground());
    
    addressListLayout->setSpacing(12);
    addressListLayout->setAlignment(Qt::AlignTop);
    
    mainLayout->addWidget(scrollArea, 1);

    // Connections
    connect(backButton, &QPushButton::clicked, this, &ChainDetailScreen::onBackClicked);
    connect(addAddressButton, &QPushButton::clicked, this, &ChainDetailScreen::onAddAddress);
    connect(sendButton, &QPushButton::clicked, this, &ChainDetailScreen::onSendClicked);
    connect(receiveButton, &QPushButton::clicked, this, &ChainDetailScreen::onReceiveClicked);
}

void ChainDetailScreen::loadAddresses()
{
    if (!wallet.restoreFromMnemonic(mnemonic)) {
        QMessageBox::critical(this, "오류", "지갑 복원 실패");
        return;
    }

    // Load first address (index 0)
    QString address = wallet.deriveAddress(chainSymbol, 0);
    if (!address.isEmpty()) {
        addAddressCard(0, address, "0.0");
        // Load balance asynchronously
        QTimer::singleShot(100, this, [this]() {
            updateBalance(0);
        });
    }
}

void ChainDetailScreen::addAddressCard(int index, const QString &address, const QString &balance)
{
    auto *card = new QWidget(addressListContainer);
    card->setMinimumHeight(DesignTokens::Sizes::CARD_MIN_HEIGHT_LG);
    card->setCursor(Qt::PointingHandCursor);
    card->setStyleSheet(StyleHelper::interactiveCard());

    // Make card clickable
    card->installEventFilter(this);
    card->setProperty("addressIndex", index);

    auto *cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(DesignTokens::Spacing::SPACING_XL,
                                   DesignTokens::Spacing::SPACING_LG,
                                   DesignTokens::Spacing::SPACING_XL,
                                   DesignTokens::Spacing::SPACING_LG);
    cardLayout->setSpacing(DesignTokens::Spacing::SPACING_LG);

    // Left: Address info
    auto *leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(DesignTokens::Spacing::SPACING_XS);

    auto *indexLabel = new QLabel(QString("주소 #%1").arg(index), card);
    indexLabel->setStyleSheet(QString(R"(
        QLabel {
            background: transparent;
            font-size: %1px;
            font-weight: %2;
            color: %3;
            border: none;
        }
    )")
        .arg(DesignTokens::Typography::FONT_SIZE_SM)
        .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
        .arg(DesignTokens::Colors::TEXT_SUBTLE));

    auto *addressLabel = new QLabel(address, card);
    addressLabel->setStyleSheet(StyleHelper::monoLabel());
    addressLabel->setWordWrap(true);

    auto *balanceLabel = new QLabel(balance + " " + chainSymbol, card);
    balanceLabel->setStyleSheet(QString(R"(
        QLabel {
            background: transparent;
            font-size: %1px;
            font-weight: %2;
            color: %3;
            border: none;
        }
    )")
        .arg(DesignTokens::Typography::FONT_SIZE_BASE)
        .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
        .arg(DesignTokens::Colors::BRAND_PRIMARY));

    leftLayout->addWidget(indexLabel);
    leftLayout->addWidget(addressLabel);
    leftLayout->addWidget(balanceLabel);

    cardLayout->addLayout(leftLayout, 1);

    // Right: Copy button
    auto *copyButton = new QPushButton("복사", card);
    copyButton->setMaximumWidth(80);
    copyButton->setMinimumHeight(40);
    copyButton->setStyleSheet(StyleHelper::customButton(
        DesignTokens::Colors::BRAND_PRIMARY,
        DesignTokens::Colors::BRAND_PRIMARY_LIGHT));

    connect(copyButton, &QPushButton::clicked, this, [this, address, event = static_cast<QObject*>(nullptr)]() {
        onCopyAddress(address);
    });

    cardLayout->addWidget(copyButton);

    // Store address data
    AddressData data;
    data.index = index;
    data.address = address;
    data.balance = balance;
    data.cardWidget = card;
    data.balanceLabel = balanceLabel;
    addresses.append(data);

    addressListLayout->addWidget(card);
}

void ChainDetailScreen::onAddAddress()
{
    int nextIndex = addresses.size();
    
    // Derive new address
    QString newAddress = wallet.deriveAddress(chainSymbol, nextIndex);
    
    if (newAddress.isEmpty()) {
        QMessageBox::warning(this, "오류", "주소 생성 실패");
        return;
    }

    addAddressCard(nextIndex, newAddress, "0.0");
    
    // Load balance for new address
    QTimer::singleShot(100, this, [this, nextIndex]() {
        updateBalance(nextIndex);
    });
    
    QMessageBox::information(this, "완료", 
        QString("새 주소가 추가되었습니다!\n\n주소 #%1").arg(nextIndex));
}

void ChainDetailScreen::onCopyAddress(const QString &address)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(address);
    
    QMessageBox::information(this, "복사 완료", 
        "주소가 클립보드에 복사되었습니다:\n\n" + address);
}

void ChainDetailScreen::onBackClicked()
{
    emit backClicked();
}

void ChainDetailScreen::onSendClicked()
{
    if (addresses.isEmpty()) {
        QMessageBox::warning(this, "경고", "주소가 없습니다");
        return;
    }
    
    // Open send dialog with first address
    SendTransactionDialog dialog(chainSymbol, addresses[0].address, mnemonic, this);
    dialog.exec();
    
    // Refresh balance after sending
    QTimer::singleShot(1000, this, [this]() {
        for (int i = 0; i < addresses.size(); ++i) {
            updateBalance(i);
        }
    });
}

void ChainDetailScreen::onReceiveClicked()
{
    if (addresses.isEmpty()) {
        QMessageBox::warning(this, "경고", "주소가 없습니다");
        return;
    }
    
    // Show QR code for first address
    QRCodeDialog dialog(addresses[0].address, chainSymbol, this);
    dialog.exec();
}

void ChainDetailScreen::onRefreshBalance()
{
    for (int i = 0; i < addresses.size(); ++i) {
        updateBalance(i);
    }
}

void ChainDetailScreen::updateBalance(int index)
{
    if (index < 0 || index >= addresses.size()) {
        return;
    }
    
    QString address = addresses[index].address;
    QString balance = "0.0";
    
    // Get balance based on chain type
    if (chainSymbol == "BTC" || chainSymbol == "LTC" || chainSymbol == "DOGE") {
        BitcoinAdapter adapter("", false);
        balance = adapter.getBalance(address);
    } else if (chainSymbol == "ETH") {
        EthereumAdapter adapter("");
        balance = adapter.getBalance(address);
    } else if (chainSymbol == "TRX") {
        TronAdapter adapter("");
        balance = adapter.getBalance(address);
    } else if (chainSymbol == "SOL") {
        SolanaAdapter adapter("");
        balance = adapter.getBalance(address);
    }
    
    // Update address card balance
    addresses[index].balance = balance;
    if (addresses[index].balanceLabel) {
        addresses[index].balanceLabel->setText(balance + " " + chainSymbol);
    }
    
    // Update total balance
    double total = 0.0;
    for (const auto &addr : addresses) {
        total += addr.balance.toDouble();
    }
    totalBalanceLabel->setText(QString::number(total, 'f', 8) + " " + chainSymbol);
}

void ChainDetailScreen::loadTransactionHistory()
{
    // Clear existing rows
    transactionTable->setRowCount(0);
    
    if (addresses.isEmpty()) {
        return;
    }
    
    // TODO: Load actual transaction history from blockchain
    // For now, show placeholder message
    transactionTable->setRowCount(1);
    
    QLabel *noTxLabel = new QLabel("거래 내역을 불러오는 중...", this);
    noTxLabel->setStyleSheet(R"(
        QLabel {
            background: transparent;
            color: #64748B;
            font-size: 13px;
        }
    )");
    noTxLabel->setAlignment(Qt::AlignCenter);
    
    transactionTable->setCellWidget(0, 0, noTxLabel);
    transactionTable->setSpan(0, 0, 1, 4);
}

void ChainDetailScreen::scanAddressesWithBalance()
{
    if (!wallet.restoreFromMnemonic(mnemonic)) {
        qDebug() << "[ChainDetailScreen] ERROR: Failed to restore wallet from mnemonic";
        QMessageBox::critical(this, "오류", "지갑 복원 실패");
        totalBalanceLabel->setText("0.0 " + chainSymbol);
        return;
    }
    
    qDebug() << "[ChainDetailScreen] Starting address scan for" << chainSymbol;
    
    totalBalanceLabel->setText("스캔 중...");
    QApplication::processEvents();
    
    // Simplified scan - only check first 5 addresses to prevent crashes
    const int MAX_SCAN = 5;
    double totalBalance = 0.0;
    int foundCount = 0;
    
    for (int i = 0; i < MAX_SCAN; i++) {
        QString address = wallet.deriveAddress(chainSymbol, i);
        
        if (address.isEmpty()) {
            qDebug() << "[ChainDetailScreen] WARNING: Failed to derive address at index" << i;
            continue;
        }
        
        qDebug() << "[ChainDetailScreen] Checking index" << i << ":" << address;
        
        // Get balance with error handling
        QString balance = "0.0";
        try {
            balance = getBalance(address);
        } catch (const std::exception &e) {
            qDebug() << "[ChainDetailScreen] ERROR: Exception getting balance:" << e.what();
        } catch (...) {
            qDebug() << "[ChainDetailScreen] ERROR: Unknown exception getting balance";
        }
        
        double balanceValue = balance.toDouble();
        qDebug() << "[ChainDetailScreen] Balance for index" << i << ":" << balance;
        
        // Always add first address, and others if they have balance
        if (i == 0 || balanceValue > 0.0) {
            addAddressCard(i, address, balance);
            totalBalance += balanceValue;
            foundCount++;
            
            if (balanceValue > 0.0) {
                qDebug() << "[ChainDetailScreen] Found address with balance at index" << i;
            }
        }
        
        // Update UI periodically
        QApplication::processEvents();
    }
    
    // Update total balance
    totalBalanceLabel->setText(QString::number(totalBalance, 'f', 8) + " " + chainSymbol);
    
    qDebug() << "[ChainDetailScreen] Scan complete. Found" << foundCount << "addresses. Total balance:" << totalBalance;
}

QString ChainDetailScreen::getBalance(const QString &address)
{
    QString balance = "0.0";
    
    if (address.isEmpty()) {
        qDebug() << "[ChainDetailScreen] ERROR: Empty address provided";
        return balance;
    }
    
    try {
        if (chainSymbol == "BTC" || chainSymbol == "LTC" || chainSymbol == "DOGE") {
            qDebug() << "[ChainDetailScreen] Getting BTC/LTC/DOGE balance for" << address;
            BitcoinAdapter adapter("", false);
            balance = adapter.getBalance(address);
        } else if (chainSymbol == "ETH") {
            qDebug() << "[ChainDetailScreen] Getting ETH balance for" << address;
            EthereumAdapter adapter("");
            balance = adapter.getBalance(address);
        } else if (chainSymbol == "TRX") {
            qDebug() << "[ChainDetailScreen] Getting TRX balance for" << address;
            TronAdapter adapter("");
            balance = adapter.getBalance(address);
        } else if (chainSymbol == "SOL") {
            qDebug() << "[ChainDetailScreen] Getting SOL balance for" << address;
            SolanaAdapter adapter("");
            balance = adapter.getBalance(address);
        } else {
            qDebug() << "[ChainDetailScreen] WARNING: Unknown chain symbol" << chainSymbol;
        }
        
        // Validate balance
        if (balance.isEmpty()) {
            qDebug() << "[ChainDetailScreen] WARNING: Empty balance returned, defaulting to 0.0";
            balance = "0.0";
        }
        
    } catch (const std::exception &e) {
        qDebug() << "[ChainDetailScreen] ERROR: Exception getting balance for" << address << ":" << e.what();
        balance = "0.0";
    } catch (...) {
        qDebug() << "[ChainDetailScreen] ERROR: Unknown exception getting balance for" << address;
        balance = "0.0";
    }
    
    return balance;
}

void ChainDetailScreen::onAddressCardClicked(int index)
{
    if (index < 0 || index >= addresses.size()) {
        return;
    }
    
    qDebug() << "[ChainDetailScreen] Address card clicked:" << index;
    
    // Emit signal to open address detail view
    emit addressClicked(addresses[index].address, addresses[index].balance, index);
    
    // For now, show a message with address details
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("주소 상세");
    msgBox.setText(QString("주소 #%1").arg(index));
    msgBox.setInformativeText(
        QString("주소: %1\n\n잔액: %2 %3")
            .arg(addresses[index].address)
            .arg(addresses[index].balance)
            .arg(chainSymbol)
    );
    
    QPushButton *sendBtn = msgBox.addButton("전송", QMessageBox::ActionRole);
    QPushButton *receiveBtn = msgBox.addButton("받기", QMessageBox::ActionRole);
    QPushButton *copyBtn = msgBox.addButton("복사", QMessageBox::ActionRole);
    msgBox.addButton("닫기", QMessageBox::RejectRole);
    
    msgBox.exec();
    
    if (msgBox.clickedButton() == sendBtn) {
        SendTransactionDialog dialog(chainSymbol, addresses[index].address, mnemonic, this);
        dialog.exec();
    } else if (msgBox.clickedButton() == receiveBtn) {
        QRCodeDialog dialog(addresses[index].address, chainSymbol, this);
        dialog.exec();
    } else if (msgBox.clickedButton() == copyBtn) {
        onCopyAddress(addresses[index].address);
    }
}

bool ChainDetailScreen::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QWidget *widget = qobject_cast<QWidget*>(obj);
        if (widget && widget->property("addressIndex").isValid()) {
            int index = widget->property("addressIndex").toInt();
            
            // Don't trigger on copy button click
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QWidget *childWidget = widget->childAt(mouseEvent->pos());
            if (childWidget && qobject_cast<QPushButton*>(childWidget)) {
                return QWidget::eventFilter(obj, event);
            }
            
            onAddressCardClicked(index);
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

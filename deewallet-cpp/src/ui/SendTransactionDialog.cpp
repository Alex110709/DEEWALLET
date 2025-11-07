/**
 * DEE WALLET - Send Transaction Dialog Implementation
 */

#include "SendTransactionDialog.h"
#include "../core/WalletCore.h"
#include "../chains/BitcoinAdapter.h"
#include "../chains/EthereumAdapter.h"
#include "../chains/TronAdapter.h"
#include "../chains/SolanaAdapter.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QRegularExpression>

SendTransactionDialog::SendTransactionDialog(const QString &chainType,
                                             const QString &fromAddress,
                                             const QString &mnemonic,
                                             QWidget *parent)
    : QDialog(parent)
    , chainType(chainType)
    , fromAddress(fromAddress)
    , mnemonic(mnemonic)
    , estimatedFee(0.0)
    , recipientInput(new QLineEdit(this))
    , amountInput(new QLineEdit(this))
    , maxButton(new QPushButton("MAX", this))
    , feeSelector(new QComboBox(this))
    , feeLabel(new QLabel("Calculating...", this))
    , totalLabel(new QLabel("0.0", this))
    , balanceLabel(new QLabel("0.0", this))
    , memoInput(new QTextEdit(this))
    , sendButton(new QPushButton("Send Transaction", this))
    , cancelButton(new QPushButton("Cancel", this))
    , addressBookButton(new QPushButton("📖 Address Book", this))
{
    setWindowTitle("Send " + chainType);
    setupUI();
    onEstimateFee();
}

void SendTransactionDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // Title
    auto *titleLabel = new QLabel("Send " + chainType, this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    // From address
    auto *fromLabel = new QLabel("From:", this);
    fromLabel->setStyleSheet("font-size: 12px; color: #999999;");
    auto *fromAddressLabel = new QLabel(fromAddress, this);
    fromAddressLabel->setStyleSheet("font-size: 14px; font-family: monospace;");
    mainLayout->addWidget(fromLabel);
    mainLayout->addWidget(fromAddressLabel);

    // Balance
    QString symbol = chainType;
    balanceLabel->setText("Balance: 0.0 " + symbol);
    balanceLabel->setStyleSheet("font-size: 14px; color: #00ff00;");
    mainLayout->addWidget(balanceLabel);

    // Recipient
    auto *recipientLabel = new QLabel("Recipient Address:", this);
    recipientLabel->setStyleSheet("font-size: 14px; color: #cccccc;");
    mainLayout->addWidget(recipientLabel);

    auto *recipientLayout = new QHBoxLayout();
    recipientInput->setPlaceholderText("Enter " + chainType + " address...");
    recipientInput->setStyleSheet(R"(
        QLineEdit {
            background-color: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 12px;
            font-size: 14px;
            font-family: monospace;
        }
    )");
    recipientLayout->addWidget(recipientInput);
    addressBookButton->setObjectName("secondaryButton");
    addressBookButton->setMaximumWidth(150);
    recipientLayout->addWidget(addressBookButton);
    mainLayout->addLayout(recipientLayout);

    // Amount
    auto *amountLabel = new QLabel("Amount:", this);
    amountLabel->setStyleSheet("font-size: 14px; color: #cccccc;");
    mainLayout->addWidget(amountLabel);

    auto *amountLayout = new QHBoxLayout();
    amountInput->setPlaceholderText("0.0");
    amountInput->setStyleSheet(recipientInput->styleSheet());
    amountLayout->addWidget(amountInput);
    maxButton->setObjectName("secondaryButton");
    maxButton->setMaximumWidth(80);
    amountLayout->addWidget(maxButton);
    mainLayout->addLayout(amountLayout);

    // Fee selector
    auto *feeLabel2 = new QLabel("Transaction Fee:", this);
    feeLabel2->setStyleSheet("font-size: 14px; color: #cccccc;");
    mainLayout->addWidget(feeLabel2);

    feeSelector->addItem("Slow (Low Fee)");
    feeSelector->addItem("Normal (Recommended)");
    feeSelector->addItem("Fast (High Fee)");
    feeSelector->setCurrentIndex(1);
    feeSelector->setStyleSheet(R"(
        QComboBox {
            background-color: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 12px;
            font-size: 14px;
        }
    )");
    mainLayout->addWidget(feeSelector);

    feeLabel->setStyleSheet("font-size: 14px; color: #ffaa00;");
    mainLayout->addWidget(feeLabel);

    // Total
    totalLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #ffffff;");
    auto *totalTextLabel = new QLabel("Total (Amount + Fee):", this);
    totalTextLabel->setStyleSheet("font-size: 12px; color: #999999;");
    mainLayout->addWidget(totalTextLabel);
    mainLayout->addWidget(totalLabel);

    // Memo (optional)
    if (chainType != "BTC") {
        auto *memoLabel = new QLabel("Memo (Optional):", this);
        memoLabel->setStyleSheet("font-size: 14px; color: #cccccc;");
        mainLayout->addWidget(memoLabel);

        memoInput->setPlaceholderText("Add a note (not encrypted)...");
        memoInput->setMaximumHeight(60);
        memoInput->setStyleSheet(R"(
            QTextEdit {
                background-color: #2a2a2a;
                border: 2px solid #3a3a3a;
                border-radius: 8px;
                padding: 12px;
                font-size: 14px;
            }
        )");
        mainLayout->addWidget(memoInput);
    }

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);

    cancelButton->setObjectName("secondaryButton");
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(sendButton);

    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(recipientInput, &QLineEdit::textChanged, this, &SendTransactionDialog::onRecipientChanged);
    connect(amountInput, &QLineEdit::textChanged, this, &SendTransactionDialog::onAmountChanged);
    connect(maxButton, &QPushButton::clicked, this, &SendTransactionDialog::onMaxClicked);
    connect(feeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SendTransactionDialog::onEstimateFee);
    connect(sendButton, &QPushButton::clicked, this, &SendTransactionDialog::onSendTransaction);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(addressBookButton, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Address Book",
            "Address book feature coming soon!\n\nYou'll be able to:\n"
            "• Save frequently used addresses\n"
            "• Add labels and notes\n"
            "• Quick select from list");
    });

    setMinimumWidth(600);
}

void SendTransactionDialog::onRecipientChanged()
{
    // Basic validation
    QString recipient = recipientInput->text();
    if (recipient.isEmpty()) return;

    bool valid = false;
    if (chainType == "BTC" && (recipient.startsWith("bc1") || recipient.startsWith("1") || recipient.startsWith("3"))) {
        valid = true;
    } else if (chainType == "ETH" && recipient.startsWith("0x") && recipient.length() == 42) {
        valid = true;
    } else if (chainType == "TRX" && recipient.startsWith("T") && recipient.length() == 34) {
        valid = true;
    } else if (chainType == "SOL" && recipient.length() >= 32 && recipient.length() <= 44) {
        valid = true;
    }

    recipientInput->setStyleSheet(valid ?
        "QLineEdit { background-color: #2a2a2a; border: 2px solid #00ff00; border-radius: 8px; padding: 12px; }" :
        "QLineEdit { background-color: #2a2a2a; border: 2px solid #ff0000; border-radius: 8px; padding: 12px; }");
}

void SendTransactionDialog::onAmountChanged()
{
    updateFeeEstimate();
}

void SendTransactionDialog::onMaxClicked()
{
    // TODO: Get actual balance and subtract fee
    amountInput->setText(currentBalance);
    updateFeeEstimate();
}

void SendTransactionDialog::onEstimateFee()
{
    // Estimate fee based on chain and fee level
    int feeLevel = feeSelector->currentIndex();

    if (chainType == "BTC") {
        // Bitcoin: sat/vB
        double feeRates[] = {1.0, 5.0, 10.0};  // sat/vB
        double txSize = 250;  // Approximate SegWit tx size
        estimatedFee = (feeRates[feeLevel] * txSize) / 100000000.0;  // Convert to BTC
        feeLabel->setText(QString("Fee: %1 BTC (~%2 sat/vB)")
            .arg(estimatedFee, 0, 'f', 8)
            .arg(feeRates[feeLevel]));
    } else if (chainType == "ETH") {
        // Ethereum: Gwei
        double gasPrice[] = {10.0, 20.0, 40.0};  // Gwei
        double gasLimit = 21000;  // Standard transfer
        estimatedFee = (gasPrice[feeLevel] * gasLimit) / 1e9;  // Convert to ETH
        feeLabel->setText(QString("Fee: %1 ETH (~%2 Gwei)")
            .arg(estimatedFee, 0, 'f', 6)
            .arg(gasPrice[feeLevel]));
    } else if (chainType == "TRX") {
        // Tron: Bandwidth/Energy
        estimatedFee = 0.0;  // Usually free with bandwidth
        feeLabel->setText("Fee: ~0 TRX (using bandwidth)");
    } else if (chainType == "SOL") {
        // Solana: Lamports
        estimatedFee = 0.000005;  // 5000 lamports
        feeLabel->setText(QString("Fee: %1 SOL (5000 lamports)").arg(estimatedFee, 0, 'f', 6));
    }

    updateFeeEstimate();
}

void SendTransactionDialog::updateFeeEstimate()
{
    double amount = amountInput->text().toDouble();
    double total = amount + estimatedFee;
    totalLabel->setText(QString("%1 %2").arg(total, 0, 'f', 8).arg(chainType));
}

bool SendTransactionDialog::validateInputs()
{
    QString recipient = recipientInput->text();
    QString amount = amountInput->text();

    if (recipient.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a recipient address.");
        return false;
    }

    if (amount.isEmpty() || amount.toDouble() <= 0) {
        QMessageBox::warning(this, "Error", "Please enter a valid amount.");
        return false;
    }

    // Validate address format
    onRecipientChanged();

    return true;
}

QString SendTransactionDialog::signAndBroadcast()
{
    if (!validateInputs()) {
        return QString();
    }

    try {
        // Restore wallet from mnemonic
        WalletCore wallet;
        if (!wallet.restoreFromMnemonic(mnemonic)) {
            QMessageBox::critical(this, "Error", "Failed to restore wallet.");
            return QString();
        }

        QString recipient = recipientInput->text();
        QString amount = amountInput->text();
        QString txHash;

        if (chainType == "BTC") {
            BitcoinAdapter adapter("", false);

            // Get private key
            QString path = "m/44'/0'/0'/0/0";
            QByteArray privateKey = wallet.derivePrivateKey(path);

            // Create, sign, and broadcast transaction
            QString rawTx = adapter.createTransaction(fromAddress, recipient, amount, "");
            QString signedTx = adapter.signTransaction(rawTx, privateKey);
            txHash = adapter.broadcastTransaction(signedTx);

        } else if (chainType == "ETH") {
            EthereumAdapter adapter("");

            QString path = "m/44'/60'/0'/0/0";
            QByteArray privateKey = wallet.derivePrivateKey(path);

            QString gasPrice = QString::number(estimatedFee * 1e9 / 21000, 'f', 0);
            QString rawTx = adapter.createTransaction(fromAddress, recipient, amount, gasPrice);
            QString signedTx = adapter.signTransaction(rawTx, privateKey);
            txHash = adapter.broadcastTransaction(signedTx);

        } else if (chainType == "TRX") {
            TronAdapter adapter("");

            QString path = "m/44'/195'/0'/0/0";
            QByteArray privateKey = wallet.derivePrivateKey(path);

            QString rawTx = adapter.createTransaction(fromAddress, recipient, amount, "");
            QString signedTx = adapter.signTransaction(rawTx, privateKey);
            txHash = adapter.broadcastTransaction(signedTx);

        } else if (chainType == "SOL") {
            SolanaAdapter adapter("");

            QString path = "m/44'/501'/0'/0/0";
            QByteArray privateKey = wallet.derivePrivateKey(path);

            QString rawTx = adapter.createTransaction(fromAddress, recipient, amount, "");
            QString signedTx = adapter.signTransaction(rawTx, privateKey);
            txHash = adapter.broadcastTransaction(signedTx);
        }

        return txHash;

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error",
            QString("Transaction failed:\n%1").arg(e.what()));
        return QString();
    }
}

void SendTransactionDialog::onSendTransaction()
{
    // Confirmation dialog
    QString recipient = recipientInput->text();
    QString amount = amountInput->text();

    QMessageBox::StandardButton confirm = QMessageBox::question(this, "Confirm Transaction",
        QString("Send %1 %2 to:\n\n%3\n\nFee: %4 %2\nTotal: %5 %2\n\nThis action cannot be undone!")
            .arg(amount)
            .arg(chainType)
            .arg(recipient)
            .arg(estimatedFee, 0, 'f', 8)
            .arg(amount.toDouble() + estimatedFee, 0, 'f', 8),
        QMessageBox::Yes | QMessageBox::No);

    if (confirm != QMessageBox::Yes) {
        return;
    }

    // Disable button during processing
    sendButton->setEnabled(false);
    sendButton->setText("Sending...");

    QString txHash = signAndBroadcast();

    sendButton->setEnabled(true);
    sendButton->setText("Send Transaction");

    if (!txHash.isEmpty()) {
        QMessageBox::information(this, "Success",
            QString("Transaction sent successfully!\n\nTransaction Hash:\n%1\n\n"
                    "It may take a few minutes to confirm on the blockchain.")
            .arg(txHash));
        accept();
    }
}

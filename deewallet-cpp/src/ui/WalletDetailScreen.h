/**
 * DEE WALLET - Wallet Detail Screen
 * Shows balances, addresses, and transaction history
 */

#ifndef WALLETDETAILSCREEN_H
#define WALLETDETAILSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include "../core/WalletCore.h"

class WalletDetailScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WalletDetailScreen(const QString &mnemonic, QWidget *parent = nullptr);

signals:
    void backToWelcome();

private slots:
    void onRefreshBalances();
    void onSendTransaction();
    void onReceive();
    void onCopyAddress(const QString &address);
    void onExportKeyfile();
    void onBackClicked();

private:
    void setupUI();
    void loadWallet();
    void refreshBalances();
    void updateChainCard(int index, const QString &chain, const QString &address, const QString &balance);

    QString mnemonic;
    WalletCore wallet;

    // UI Components
    QLabel *totalBalanceLabel;
    QTabWidget *chainTabs;
    QPushButton *refreshButton;
    QPushButton *sendButton;
    QPushButton *receiveButton;
    QPushButton *exportButton;
    QPushButton *backButton;

    // Chain data
    struct ChainData {
        QString name;
        QString address;
        QString balance;
        QWidget *widget;
        QLabel *addressLabel;
        QLabel *balanceLabel;
        QPushButton *copyButton;
    };

    QVector<ChainData> chains;
};

#endif // WALLETDETAILSCREEN_H

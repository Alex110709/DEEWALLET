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
#include <QVBoxLayout>
#include <QObject>
#include <QEvent>
#include "../core/WalletCore.h"

class WalletDetailScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WalletDetailScreen(const QString &mnemonic, QWidget *parent = nullptr);
    void setKeyfilePath(const QString &path) { keyfilePath = path; }
    void setPassword(const QString &pwd) { password = pwd; }

signals:
    void backToWelcome();
    void chainClicked(const QString &chainName, const QString &chainSymbol, const QString &mnemonic);

private slots:
    void onRefreshBalances();
    void onCopyAddress(const QString &address);
    void onBackClicked();
    void onChainCardClicked(int chainIndex);

private:
    void setupUI();
    void loadWallet();
    void refreshBalances();
    void updateChainCard(int index, const QString &chain, const QString &address, const QString &balance);

    QString mnemonic;
    QString keyfilePath;
    QString password;
    WalletCore wallet;

    // UI Components
    QWidget *chainListContainer;
    QVBoxLayout *chainListLayout;
    QPushButton *refreshButton;
    QPushButton *backButton;

    // Chain data
    struct ChainData {
        QString name;
        QString symbol;
        QString address;
        QString balance;
        QWidget *cardWidget;
        QLabel *balanceLabel;
    };

    QVector<ChainData> chains;
    
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // WALLETDETAILSCREEN_H

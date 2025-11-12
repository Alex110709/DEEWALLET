/**
 * DEE WALLET - Chain Detail Screen
 * Shows addresses for a specific chain with add address functionality
 */

#ifndef CHAINDETAILSCREEN_H
#define CHAINDETAILSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QVector>
#include "../core/WalletCore.h"

class ChainDetailScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ChainDetailScreen(const QString &chainName,
                               const QString &chainSymbol,
                               const QString &mnemonic,
                               QWidget *parent = nullptr);

signals:
    void backClicked();
    void addressClicked(const QString &address, const QString &balance, int index);

private slots:
    void onAddAddress();
    void onCopyAddress(const QString &address);
    void onBackClicked();
    void onSendClicked();
    void onReceiveClicked();
    void onRefreshBalance();
    void onAddressCardClicked(int index);

private:
    void setupUI();
    void loadAddresses();
    void scanAddressesWithBalance();
    void addAddressCard(int index, const QString &address, const QString &balance);
    void updateBalance(int index);
    void loadTransactionHistory();
    QString getBalance(const QString &address);

    QString chainName;
    QString chainSymbol;
    QString mnemonic;
    WalletCore wallet;

    // UI Components
    QWidget *addressListContainer;
    QVBoxLayout *addressListLayout;
    QPushButton *addAddressButton;
    QPushButton *sendButton;
    QPushButton *receiveButton;
    QPushButton *backButton;
    QLabel *chainTitleLabel;
    QLabel *totalBalanceLabel;
    QTableWidget *transactionTable;

    // Address data
    struct AddressData {
        int index;
        QString address;
        QString balance;
        QWidget *cardWidget;
        QLabel *balanceLabel;
    };

    QVector<AddressData> addresses;
    
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // CHAINDETAILSCREEN_H

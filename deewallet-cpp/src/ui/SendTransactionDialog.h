/**
 * DEE WALLET - Send Transaction Dialog
 * UI for creating and broadcasting transactions
 */

#ifndef SENDTRANSACTIONDIALOG_H
#define SENDTRANSACTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

class SendTransactionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SendTransactionDialog(const QString &chainType,
                                   const QString &fromAddress,
                                   const QString &mnemonic,
                                   QWidget *parent = nullptr);

private slots:
    void onRecipientChanged();
    void onAmountChanged();
    void onMaxClicked();
    void onEstimateFee();
    void onSendTransaction();

private:
    void setupUI();
    void updateFeeEstimate();
    bool validateInputs();
    QString signAndBroadcast();

    QString chainType;
    QString fromAddress;
    QString mnemonic;
    QString currentBalance;

    QLineEdit *recipientInput;
    QLineEdit *amountInput;
    QPushButton *maxButton;
    QComboBox *feeSelector;
    QLabel *feeLabel;
    QLabel *totalLabel;
    QLabel *balanceLabel;
    QTextEdit *memoInput;
    QPushButton *sendButton;
    QPushButton *cancelButton;
    QPushButton *addressBookButton;

    double estimatedFee;
};

#endif // SENDTRANSACTIONDIALOG_H

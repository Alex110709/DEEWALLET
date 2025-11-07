/**
 * DEE WALLET - Create Wallet Dialog
 */

#ifndef CREATEWALLETDIALOG_H
#define CREATEWALLETDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

class CreateWalletDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateWalletDialog(QWidget *parent = nullptr);
    QString getMnemonic() const { return createdMnemonic; }

private slots:
    void onGenerateMnemonic();
    void onCreateWallet();

private:
    void setupUI();

    QTextEdit *mnemonicDisplay;
    QLineEdit *passwordInput;
    QLineEdit *confirmPasswordInput;
    QPushButton *generateButton;
    QPushButton *createButton;
    QPushButton *cancelButton;
    QString createdMnemonic;
};

#endif // CREATEWALLETDIALOG_H

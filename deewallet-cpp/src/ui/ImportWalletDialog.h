/**
 * DEE WALLET - Import Wallet Dialog
 */

#ifndef IMPORTWALLETDIALOG_H
#define IMPORTWALLETDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class ImportWalletDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportWalletDialog(QWidget *parent = nullptr);
    QString getMnemonic() const { return importedMnemonic; }

private slots:
    void onBrowseFile();
    void onImportWallet();

private:
    void setupUI();

    QLineEdit *filePathInput;
    QLineEdit *passwordInput;
    QPushButton *browseButton;
    QPushButton *importButton;
    QPushButton *cancelButton;
    QLabel *fileInfoLabel;
    QString importedMnemonic;
};

#endif // IMPORTWALLETDIALOG_H

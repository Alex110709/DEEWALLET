/**
 * DEE WALLET - QR Code Dialog
 * Display QR code for receiving payments
 */

#ifndef QRCODEDIALOG_H
#define QRCODEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class QRCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QRCodeDialog(const QString &address,
                         const QString &chainType,
                         QWidget *parent = nullptr);

private slots:
    void onCopyAddress();
    void onSaveQRCode();

private:
    void setupUI();
    QPixmap generateQRCode(const QString &data, int size = 300);

    QString address;
    QString chainType;

    QLabel *qrCodeLabel;
    QLabel *addressLabel;
    QLineEdit *amountInput;
    QPushButton *copyButton;
    QPushButton *saveButton;
    QPushButton *closeButton;
};

#endif // QRCODEDIALOG_H

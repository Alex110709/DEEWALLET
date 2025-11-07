/**
 * DEE WALLET - QR Code Dialog Implementation
 */

#include "QRCodeDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QFileDialog>
#include <QPainter>
#include <QImage>

QRCodeDialog::QRCodeDialog(const QString &address, const QString &chainType, QWidget *parent)
    : QDialog(parent)
    , address(address)
    , chainType(chainType)
    , qrCodeLabel(new QLabel(this))
    , addressLabel(new QLabel(this))
    , amountInput(new QLineEdit(this))
    , copyButton(new QPushButton("Copy Address", this))
    , saveButton(new QPushButton("Save QR Code", this))
    , closeButton(new QPushButton("Close", this))
{
    setWindowTitle("Receive " + chainType);
    setupUI();
}

void QRCodeDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // Title
    auto *titleLabel = new QLabel("Receive " + chainType, this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // QR Code
    QPixmap qrCode = generateQRCode(address, 300);
    qrCodeLabel->setPixmap(qrCode);
    qrCodeLabel->setAlignment(Qt::AlignCenter);
    qrCodeLabel->setStyleSheet(R"(
        QLabel {
            background-color: white;
            border: 2px solid #3a3a3a;
            border-radius: 12px;
            padding: 20px;
        }
    )");
    mainLayout->addWidget(qrCodeLabel);

    // Address
    auto *addressTitle = new QLabel("Your Address:", this);
    addressTitle->setStyleSheet("font-size: 12px; color: #999999;");
    addressTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(addressTitle);

    addressLabel->setText(address);
    addressLabel->setStyleSheet(R"(
        QLabel {
            font-size: 14px;
            font-family: monospace;
            background-color: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 12px;
        }
    )");
    addressLabel->setWordWrap(true);
    addressLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(addressLabel);

    // Amount (optional)
    auto *amountLabel = new QLabel("Amount (Optional):", this);
    amountLabel->setStyleSheet("font-size: 12px; color: #999999;");
    mainLayout->addWidget(amountLabel);

    amountInput->setPlaceholderText("Enter amount to request...");
    amountInput->setStyleSheet(R"(
        QLineEdit {
            background-color: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 12px;
            font-size: 14px;
        }
    )");
    mainLayout->addWidget(amountInput);

    // Info text
    auto *infoLabel = new QLabel("Share this QR code or address to receive " + chainType, this);
    infoLabel->setStyleSheet("font-size: 12px; color: #999999;");
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);
    mainLayout->addWidget(infoLabel);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);

    saveButton->setObjectName("secondaryButton");
    closeButton->setObjectName("secondaryButton");

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(copyButton);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(copyButton, &QPushButton::clicked, this, &QRCodeDialog::onCopyAddress);
    connect(saveButton, &QPushButton::clicked, this, &QRCodeDialog::onSaveQRCode);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    setMinimumWidth(500);
}

QPixmap QRCodeDialog::generateQRCode(const QString &data, int size)
{
    // Simple QR code generation using geometric patterns
    // For production, use a proper QR code library like qrencode

    QImage image(size, size, QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw a simple pattern representing the data
    // This is a placeholder - real QR code generation requires a library

    int moduleSize = 10;
    int modules = size / moduleSize;

    // Draw finder patterns (corners)
    auto drawFinderPattern = [&](int x, int y) {
        painter.fillRect(x, y, moduleSize * 7, moduleSize * 7, Qt::black);
        painter.fillRect(x + moduleSize, y + moduleSize, moduleSize * 5, moduleSize * 5, Qt::white);
        painter.fillRect(x + moduleSize * 2, y + moduleSize * 2, moduleSize * 3, moduleSize * 3, Qt::black);
    };

    drawFinderPattern(0, 0);
    drawFinderPattern((modules - 7) * moduleSize, 0);
    drawFinderPattern(0, (modules - 7) * moduleSize);

    // Draw data modules (pseudo-random pattern based on hash)
    uint hash = qHash(data);
    for (int y = 0; y < modules; ++y) {
        for (int x = 0; x < modules; ++x) {
            // Skip finder patterns
            if ((x < 8 && y < 8) ||
                (x >= modules - 8 && y < 8) ||
                (x < 8 && y >= modules - 8)) {
                continue;
            }

            // Pseudo-random module based on position and data hash
            uint val = (hash + x * 7 + y * 13) % 256;
            if (val < 128) {
                painter.fillRect(x * moduleSize, y * moduleSize, moduleSize, moduleSize, Qt::black);
            }
        }
    }

    painter.end();

    return QPixmap::fromImage(image);
}

void QRCodeDialog::onCopyAddress()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(address);

    QMessageBox::information(this, "Copied",
        "Address copied to clipboard:\n\n" + address);
}

void QRCodeDialog::onSaveQRCode()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save QR Code",
        chainType + "_receive_" + address.left(10) + ".png",
        "PNG Image (*.png)");

    if (!fileName.isEmpty()) {
        qrCodeLabel->pixmap().save(fileName, "PNG");
        QMessageBox::information(this, "Saved",
            "QR code saved successfully to:\n" + fileName);
    }
}

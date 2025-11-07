/**
 * DEE WALLET - Address Book Dialog
 * Manage saved addresses with labels
 */

#ifndef ADDRESSBOOKDIALOG_H
#define ADDRESSBOOKDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVector>

struct AddressBookEntry {
    QString label;
    QString address;
    QString chainType;
    QString note;
    qint64 createdAt;
};

class AddressBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddressBookDialog(QWidget *parent = nullptr);
    QString getSelectedAddress() const { return selectedAddress; }

private slots:
    void onAddAddress();
    void onEditAddress();
    void onDeleteAddress();
    void onSelectAddress();
    void onSearchChanged();
    void onFilterChanged();

private:
    void setupUI();
    void loadAddresses();
    void saveAddresses();
    void refreshTable();
    void addEntry(const AddressBookEntry &entry);

    QTableWidget *addressTable;
    QLineEdit *searchInput;
    QComboBox *filterCombo;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *selectButton;
    QPushButton *closeButton;

    QVector<AddressBookEntry> addresses;
    QString selectedAddress;
    QString addressBookPath;
};

#endif // ADDRESSBOOKDIALOG_H

/**
 * DEE WALLET - Address Book Dialog Implementation
 */

#include "AddressBookDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

AddressBookDialog::AddressBookDialog(QWidget *parent)
    : QDialog(parent)
    , addressTable(new QTableWidget(this))
    , searchInput(new QLineEdit(this))
    , filterCombo(new QComboBox(this))
    , addButton(new QPushButton("➕ Add", this))
    , editButton(new QPushButton("✏️ Edit", this))
    , deleteButton(new QPushButton("🗑️ Delete", this))
    , selectButton(new QPushButton("Select", this))
    , closeButton(new QPushButton("Close", this))
{
    setWindowTitle("Address Book");
    addressBookPath = QApplication::applicationDirPath() + "/addressbook.json";
    setupUI();
    loadAddresses();
}

void AddressBookDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Title
    auto *titleLabel = new QLabel("📖 Address Book", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    // Search and filter bar
    auto *searchLayout = new QHBoxLayout();
    searchInput->setPlaceholderText("🔍 Search by label or address...");
    searchInput->setStyleSheet(R"(
        QLineEdit {
            background-color: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 10px;
            font-size: 14px;
        }
    )");
    searchLayout->addWidget(searchInput);

    filterCombo->addItem("All Chains");
    filterCombo->addItem("Bitcoin");
    filterCombo->addItem("Ethereum");
    filterCombo->addItem("Tron");
    filterCombo->addItem("Solana");
    filterCombo->setStyleSheet(searchInput->styleSheet());
    filterCombo->setMaximumWidth(150);
    searchLayout->addWidget(filterCombo);

    mainLayout->addLayout(searchLayout);

    // Address table
    addressTable->setColumnCount(4);
    addressTable->setHorizontalHeaderLabels({"Label", "Address", "Chain", "Note"});
    addressTable->horizontalHeader()->setStretchLastSection(true);
    addressTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    addressTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    addressTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    addressTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    addressTable->setSelectionMode(QAbstractItemView::SingleSelection);
    addressTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    addressTable->setAlternatingRowColors(true);
    addressTable->setStyleSheet(R"(
        QTableWidget {
            background-color: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            gridline-color: #3a3a3a;
        }
        QTableWidget::item {
            padding: 8px;
        }
        QTableWidget::item:selected {
            background-color: #0066ff;
        }
        QHeaderView::section {
            background-color: #1a1a1a;
            color: #ffffff;
            padding: 8px;
            border: none;
            font-weight: bold;
        }
    )");
    mainLayout->addWidget(addressTable);

    // Action buttons
    auto *actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(10);

    addButton->setMinimumHeight(40);
    editButton->setMinimumHeight(40);
    deleteButton->setMinimumHeight(40);
    editButton->setObjectName("secondaryButton");
    deleteButton->setObjectName("secondaryButton");

    actionLayout->addWidget(addButton);
    actionLayout->addWidget(editButton);
    actionLayout->addWidget(deleteButton);
    actionLayout->addStretch();

    mainLayout->addLayout(actionLayout);

    // Bottom buttons
    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(10);

    closeButton->setObjectName("secondaryButton");
    closeButton->setMinimumHeight(40);
    selectButton->setMinimumHeight(40);

    bottomLayout->addWidget(closeButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(selectButton);

    mainLayout->addLayout(bottomLayout);

    // Connections
    connect(searchInput, &QLineEdit::textChanged, this, &AddressBookDialog::onSearchChanged);
    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddressBookDialog::onFilterChanged);
    connect(addButton, &QPushButton::clicked, this, &AddressBookDialog::onAddAddress);
    connect(editButton, &QPushButton::clicked, this, &AddressBookDialog::onEditAddress);
    connect(deleteButton, &QPushButton::clicked, this, &AddressBookDialog::onDeleteAddress);
    connect(selectButton, &QPushButton::clicked, this, &AddressBookDialog::onSelectAddress);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(addressTable, &QTableWidget::doubleClicked, this, &AddressBookDialog::onSelectAddress);

    setMinimumSize(800, 500);
}

void AddressBookDialog::loadAddresses()
{
    QFile file(addressBookPath);
    if (!file.open(QIODevice::ReadOnly)) {
        // File doesn't exist yet, that's okay
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;

    QJsonArray array = doc.array();
    for (const QJsonValue &val : array) {
        QJsonObject obj = val.toObject();
        AddressBookEntry entry;
        entry.label = obj["label"].toString();
        entry.address = obj["address"].toString();
        entry.chainType = obj["chain"].toString();
        entry.note = obj["note"].toString();
        entry.createdAt = obj["created"].toVariant().toLongLong();
        addresses.append(entry);
    }

    refreshTable();
}

void AddressBookDialog::saveAddresses()
{
    QJsonArray array;
    for (const AddressBookEntry &entry : addresses) {
        QJsonObject obj;
        obj["label"] = entry.label;
        obj["address"] = entry.address;
        obj["chain"] = entry.chainType;
        obj["note"] = entry.note;
        obj["created"] = entry.createdAt;
        array.append(obj);
    }

    QJsonDocument doc(array);
    QFile file(addressBookPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void AddressBookDialog::refreshTable()
{
    addressTable->setRowCount(0);

    QString searchText = searchInput->text().toLower();
    QString chainFilter = filterCombo->currentText();

    for (const AddressBookEntry &entry : addresses) {
        // Apply filters
        if (!chainFilter.isEmpty() && chainFilter != "All Chains" && entry.chainType != chainFilter) {
            continue;
        }

        if (!searchText.isEmpty() &&
            !entry.label.toLower().contains(searchText) &&
            !entry.address.toLower().contains(searchText)) {
            continue;
        }

        addEntry(entry);
    }
}

void AddressBookDialog::addEntry(const AddressBookEntry &entry)
{
    int row = addressTable->rowCount();
    addressTable->insertRow(row);

    addressTable->setItem(row, 0, new QTableWidgetItem(entry.label));
    addressTable->setItem(row, 1, new QTableWidgetItem(entry.address));
    addressTable->setItem(row, 2, new QTableWidgetItem(entry.chainType));
    addressTable->setItem(row, 3, new QTableWidgetItem(entry.note));

    // Set font for address column to monospace
    addressTable->item(row, 1)->setFont(QFont("Courier", 10));
}

void AddressBookDialog::onAddAddress()
{
    // Get label
    QString label = QInputDialog::getText(this, "Add Address", "Enter a label:");
    if (label.isEmpty()) return;

    // Get chain type
    QStringList chains = {"Bitcoin", "Ethereum", "Tron", "Solana"};
    QString chain = QInputDialog::getItem(this, "Add Address", "Select chain:", chains, 0, false);
    if (chain.isEmpty()) return;

    // Get address
    QString address = QInputDialog::getText(this, "Add Address", "Enter address:");
    if (address.isEmpty()) return;

    // Get note (optional)
    QString note = QInputDialog::getText(this, "Add Address", "Enter note (optional):");

    // Create entry
    AddressBookEntry entry;
    entry.label = label;
    entry.address = address;
    entry.chainType = chain;
    entry.note = note;
    entry.createdAt = QDateTime::currentMSecsSinceEpoch();

    addresses.append(entry);
    saveAddresses();
    refreshTable();

    QMessageBox::information(this, "Success", "Address added to address book!");
}

void AddressBookDialog::onEditAddress()
{
    int row = addressTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select an address to edit.");
        return;
    }

    // Find the actual entry (considering filters)
    QString currentAddress = addressTable->item(row, 1)->text();
    int entryIndex = -1;
    for (int i = 0; i < addresses.size(); ++i) {
        if (addresses[i].address == currentAddress) {
            entryIndex = i;
            break;
        }
    }

    if (entryIndex < 0) return;

    // Edit label
    QString newLabel = QInputDialog::getText(this, "Edit Address", "Enter new label:",
                                             QLineEdit::Normal, addresses[entryIndex].label);
    if (!newLabel.isEmpty()) {
        addresses[entryIndex].label = newLabel;
    }

    // Edit note
    QString newNote = QInputDialog::getText(this, "Edit Address", "Enter new note:",
                                           QLineEdit::Normal, addresses[entryIndex].note);
    addresses[entryIndex].note = newNote;

    saveAddresses();
    refreshTable();
}

void AddressBookDialog::onDeleteAddress()
{
    int row = addressTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select an address to delete.");
        return;
    }

    QString currentAddress = addressTable->item(row, 1)->text();
    QString currentLabel = addressTable->item(row, 0)->text();

    QMessageBox::StandardButton confirm = QMessageBox::question(this, "Delete Address",
        QString("Delete address '%1'?\n\n%2").arg(currentLabel).arg(currentAddress),
        QMessageBox::Yes | QMessageBox::No);

    if (confirm != QMessageBox::Yes) return;

    // Find and remove entry
    for (int i = 0; i < addresses.size(); ++i) {
        if (addresses[i].address == currentAddress) {
            addresses.removeAt(i);
            break;
        }
    }

    saveAddresses();
    refreshTable();
}

void AddressBookDialog::onSelectAddress()
{
    int row = addressTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select an address.");
        return;
    }

    selectedAddress = addressTable->item(row, 1)->text();
    accept();
}

void AddressBookDialog::onSearchChanged()
{
    refreshTable();
}

void AddressBookDialog::onFilterChanged()
{
    refreshTable();
}

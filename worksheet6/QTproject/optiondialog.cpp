#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}
void OptionDialog::updateDialogFromPart(ModelPart* part) {
    // 1. Get the name from the part (stored in column 0 of its QVariant array)
    QString partName = part->data(0).toString();

    // 2. Put that name into the text box on the dialog
    ui->lineEdit->setText(partName);
}

void OptionDialog::updatePartFromDialog(ModelPart* part) {
    // 1. Read whatever the user typed into the text box
    QString newName = ui->lineEdit->text();

    // 2. Save it back to the part (you might need to check ModelPart.h for the exact setter function, usually it's set() or modifying the QVariant directly)
    part->set(0, newName);
}

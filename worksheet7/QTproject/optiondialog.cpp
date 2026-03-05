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
    // 1. Set Name (Column 0)
    ui->lineEdit->setText(part->data(0).toString());

    // 2. Set Visibility (Column 1)
    // Convert the string "true" or "false" to a boolean check
    bool isVis = (part->data(1).toString() == "true");
    ui->checkBoxVisible->setChecked(isVis);

    // 3. Set Colour (Columns 2, 3, 4)
    // If the data doesn't exist yet, toInt() will safely return 0
    ui->spinBoxR->setValue(part->data(2).toInt());
    ui->spinBoxG->setValue(part->data(3).toInt());
    ui->spinBoxB->setValue(part->data(4).toInt());
}


void OptionDialog::updatePartFromDialog(ModelPart* part) {
    // 1. Save Name (Column 0)
    part->set(0, ui->lineEdit->text());

    // 2. Save Visibility (Column 1)
    // Convert the checkbox state back to a "true" or "false" string
    QString visString = ui->checkBoxVisible->isChecked() ? "true" : "false";
    part->set(1, visString);

    // 3. Save Colour (Columns 2, 3, 4)
    part->set(2, ui->spinBoxR->value());
    part->set(3, ui->spinBoxG->value());
    part->set(4, ui->spinBoxB->value());
}

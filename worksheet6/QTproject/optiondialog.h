#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include "ModelPart.h"
#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();
    void updateDialogFromPart(ModelPart* part);
    void updatePartFromDialog(ModelPart* part);

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H

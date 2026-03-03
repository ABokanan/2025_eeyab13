#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optiondialog.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	
	connect (this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);	
	connect ( ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton );
    connect ( ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton1 );
    connect ( ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked );

	//-------------------------------------------------
	    // 1. Create/allocate the ModelList
    this->partList = new ModelPartList("PartsList");

    // 2. Link it to the treeview in the GUI
    ui->treeView->setModel(this->partList);

    // 3. Manually create a model tree
    ModelPart* rootItem = this->partList->getRootItem();

    // Add 3 top-level items
    for (int i = 0; i < 3; i++) {
        // Create strings for both data columns
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        // Create child item
        ModelPart* childItem = new ModelPart({name, visible});

        // Append to tree top-level
        rootItem->appendChild(childItem);

        // Add 5 sub-items
        for (int j = 0; j < 5; j++) {
            QString subName = QString("Item %1,%2").arg(i).arg(j);
            QString subVisible("true");
            
            ModelPart* childChildItem = new ModelPart({subName, subVisible});
            
            // Append to parent
            childItem->appendChild(childChildItem);
        }
    }
}

MainWindow::~MainWindow()
{
	
    delete ui;
	
}

void MainWindow::on_actionOpen_File_triggered() {
	
	QString fileName = QFileDialog::getOpenFileName(
	this,
	tr("Open File"),
	"C:\\",
	tr("STL Files (*.stl);;Text Files (*.txt)")
	);

    //add this line of code so you can see if the action is working.
    emit statusUpdateMessage(QString("Open File action triggered ")+ fileName, 0);
}

void MainWindow::handleButton() 
{
    emit statusUpdateMessage(QString("Button 1 was clicked"), 0);
}


void MainWindow::handleButton1() 
{
    // 1. Figure out which item the user has selected in the tree view (from Exercise 5)
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // 2. Create the dialog
    OptionDialog dialog(this);

    // 3. Populate the dialog with the part's current details BEFORE showing it
    dialog.updateDialogFromPart(selectedPart);

    // 4. Show the dialog
    if (dialog.exec() == QDialog::Accepted) {
        // 5. If they clicked OK, save the changes from the UI back to the part
        dialog.updatePartFromDialog(selectedPart);
        emit statusUpdateMessage(QString("Part updated!"), 0);
    } else {
        emit statusUpdateMessage(QString("Edit cancelled"), 0);
    }
}


void MainWindow::handleTreeClicked()
{
	/* Get the index of the selected item */
	QModelIndex index = ui->treeView->currentIndex();
	
	/* Get a pointer to the item from the index */
	ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
	
	/* In this case, we will retrieve the name string from the internal QVariant data array */
	QString text = selectedPart->data(0).toString();
	
	emit statusUpdateMessage(QString("The selected item is: ")+text, 0);
}

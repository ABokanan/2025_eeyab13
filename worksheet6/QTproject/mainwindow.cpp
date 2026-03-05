#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optiondialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <vtkNew.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Link a render window with the Qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        ui->vtkWidget->setRenderWindow(renderWindow); // Change 'widget' to 'vtkWidget' if you renamed it!

    // 2. Add a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
        renderWindow->AddRenderer(renderer);

        // 3. Create the cylinder model
        vtkNew<vtkCylinderSource> cylinder;
        cylinder->SetResolution(8);

        // 4. Create the mapper to push geometry into the graphics library
        vtkNew<vtkPolyDataMapper> cylinderMapper;
        cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

        // 5. Create the actor to group the geometry, properties, and transformation
        vtkNew<vtkActor> cylinderActor;
        cylinderActor->SetMapper(cylinderMapper);
        cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.35); // Set color to a pinkish-red
    cylinderActor->RotateX(30.0);
        cylinderActor->RotateY(-45.0);

        // 6. Add the actor to the renderer
        renderer->AddActor(cylinderActor);

        // 7. Reset the camera to frame the object
        renderer->ResetCamera();
        renderer->GetActiveCamera()->Azimuth(30);
        renderer->GetActiveCamera()->Elevation(30);
        renderer->ResetCameraClippingRange();
	
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
        ModelPart* childItem = new ModelPart({name, visible, 255, 255, 255});

        // Append to tree top-level
        rootItem->appendChild(childItem);

        // Add 5 sub-items
        for (int j = 0; j < 5; j++) {
            QString subName = QString("Item %1,%2").arg(i).arg(j);
            QString subVisible("true");
            
            ModelPart* childChildItem = new ModelPart({subName, subVisible, 255, 255, 255});
            
            // Append to parent
            childItem->appendChild(childChildItem);
        }
    }
}

void MainWindow::on_actionItem_Options_triggered()
{
    ui->treeView->addAction(ui->actionItem_Options);
    // 1. Figure out which item the user has selected in the tree view (from Exercise 5)
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // 2. Create the dialog
    OptionDialog dialog(this);

    dialog.updateDialogFromPart(selectedPart);


    // 3. Populate the dialog with the part's current details BEFORE showing it
    // 4. Show the dialog
    if (dialog.exec() == QDialog::Accepted) {
        // 5. Save the changes from the UI back to the part
        dialog.updatePartFromDialog(selectedPart);

        // --- NEW CODE: Get the updated name and show it in the status bar ---

        // Read the new name back out of the part (column 0)
        QString updatedName = selectedPart->data(0).toString();

        // Combine your text with the new name
        emit statusUpdateMessage(QString("Dialog Accepted: ") + updatedName, 0);

        // --------------------------------------------------------------------
    } else {
        emit statusUpdateMessage(QString("Dialog Rejected!!!"), 0);
    }
}

MainWindow::~MainWindow()
{
	
    delete ui;
	
}

void MainWindow::on_actionOpen_File_triggered() {

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\", tr("STL Files (*.stl);;Text Files (*.txt)"));

    // If the user didn't cancel the file dialog
    if (!fileName.isEmpty()) {
        // Find the currently selected item in the tree
        QModelIndex index = ui->treeView->currentIndex();

        if (index.isValid()) {
            ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

            // Assuming column 0 is the Name, update it to the file name!
            selectedPart->set(0, fileName);

            emit statusUpdateMessage(QString("Item renamed to file: ") + fileName, 0);
        }
    }
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

    dialog.updateDialogFromPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        // 5. Save the changes from the UI back to the part
        dialog.updatePartFromDialog(selectedPart);

        // --- NEW CODE: Get the updated name and show it in the status bar ---

        // Read the new name back out of the part (column 0)
        QString updatedName = selectedPart->data(0).toString();

        // Combine your text with the new name
        emit statusUpdateMessage(QString("Dialog Accepted: ") + updatedName, 0);

        // --------------------------------------------------------------------
    } else {
        emit statusUpdateMessage(QString("Dialog Rejected!!!"), 0);
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




#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optiondialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <vtkProperty.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- FIX 1: VTK Setup moved to the Constructor! ---
    // Link a render window with the Qt widget
    // (NOTE: change 'widget' to 'vtkWidget' if you renamed it in Qt Designer!)
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    // Add a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    // Change background to dark blue
    renderer->SetBackground(0.1, 0.2, 0.4);
    // --------------------------------------------------

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
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({name, visible, 255, 255, 255});
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString subName = QString("Item %1,%2").arg(i).arg(j);
            QString subVisible("true");

            ModelPart* childChildItem = new ModelPart({subName, subVisible, 255, 255, 255});
            childItem->appendChild(childChildItem);
        }
    }
}

void MainWindow::on_actionItem_Options_triggered()
{
    ui->treeView->addAction(ui->actionItem_Options);
    QModelIndex index = ui->treeView->currentIndex();

    // Safety check!
    if (!index.isValid()) return;

    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    OptionDialog dialog(this);
    dialog.updateDialogFromPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.updatePartFromDialog(selectedPart);

        this->updateRender();
        QString updatedName = selectedPart->data(0).toString();
        emit statusUpdateMessage(QString("Dialog Accepted: ") + updatedName, 0);
    } else {
        emit statusUpdateMessage(QString("Dialog Rejected!!!"), 0);
    }
}

MainWindow::~MainWindow()
{
    // The destructor should only delete UI, nothing else!
    delete ui;
}

void MainWindow::on_actionOpen_File_triggered() {

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\", tr("STL Files (*.stl);;Text Files (*.txt)"));

    if (!fileName.isEmpty()) {
        QModelIndex index = ui->treeView->currentIndex();

        if (index.isValid()) {
            ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

            QFileInfo fileInfo(fileName);
            QString shortName = fileInfo.fileName();

            ModelPart* newItem = new ModelPart({shortName, "true", 255, 255, 255});
            selectedPart->appendChild(newItem);

            newItem->loadSTL(fileName);

            ui->treeView->setModel(nullptr);
            ui->treeView->setModel(this->partList);
            ui->treeView->expandAll();

            // --- FIX 2: Added the Render call back! ---
            this->updateRender();
            // ------------------------------------------

            emit statusUpdateMessage(QString("Loaded CAD file: ") + shortName, 0);
        } else {
            emit statusUpdateMessage(QString("Please select a Tree item to add the file to!"), 0);
        }
    }
}

void MainWindow::handleButton()
{
    emit statusUpdateMessage(QString("Button 1 was clicked"), 0);
}

void MainWindow::handleButton1()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    OptionDialog dialog(this);
    dialog.updateDialogFromPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.updatePartFromDialog(selectedPart);
        QString updatedName = selectedPart->data(0).toString();
        emit statusUpdateMessage(QString("Dialog Accepted: ") + updatedName, 0);
    } else {
        emit statusUpdateMessage(QString("Dialog Rejected!!!"), 0);
    }
}

void MainWindow::handleTreeClicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage(QString("The selected item is: ")+text, 0);
}

void MainWindow::updateRender() {
    renderer->RemoveAllViewProps();
    updateRenderFromTree(QModelIndex());

    renderer->ResetCamera();
    renderer->ResetCameraClippingRange();
    renderer->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index) {
    if (index.isValid()) {
        ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

        vtkSmartPointer<vtkActor> partActor = selectedPart->getActor();
        if (partActor != nullptr) {

            // --- 1. APPLY VISIBILITY ---
            // Read column 1. If it says "true", show it. Otherwise, hide it.
            QString visString = selectedPart->data(1).toString();
            if (visString == "true" || visString == "1") {
                partActor->SetVisibility(true);
            } else {
                partActor->SetVisibility(false);
            }

            // --- 2. APPLY COLOR ---
            // Read columns 2, 3, and 4 for RGB. VTK needs colors as decimals from 0.0 to 1.0,
            // so we divide your 0-255 numbers by 255.0!
            double r = selectedPart->data(2).toDouble() / 255.0;
            double g = selectedPart->data(3).toDouble() / 255.0;
            double b = selectedPart->data(4).toDouble() / 255.0;
            partActor->GetProperty()->SetColor(r, g, b);

            // Add the modified actor to the screen
            renderer->AddActor(partActor);
        }
    }

    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}

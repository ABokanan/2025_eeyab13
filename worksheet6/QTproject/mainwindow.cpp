#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	
	connect ( ui-pushButton, &QPushButton::released, this, &MainWindow::handleButton );
	connect ( ui-pushButton2, &QPushButton::released, this, &MainWindow::handleButton2 );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void Main Window::handlebutton() 
{
	QMessageBox msgBox;
	msgBox.setText("Button Was Clicked.");
	msgBox.exec();
}

void Main Window::handlebutton2() 
{
	QMessageBox msgBox;
	msgBox.setText("Button 2 Was Clicked.");
	msgBox.exec();
}
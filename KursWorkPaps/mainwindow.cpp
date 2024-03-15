#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList list = NetworkInterface::getNetworkInterfacesList();
    ui->comboBox_network_interfaces->addItems(list);
}

MainWindow::~MainWindow()
{
    delete ui;
}


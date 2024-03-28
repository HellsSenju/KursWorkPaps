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


void MainWindow::on_comboBox_network_interfaces_currentTextChanged(const QString &arg1)
{
    ui->listWidget_rules->clear();
    ui->listWidget_rules->addItems(NetworkInterface::getExistingRules(arg1));
}


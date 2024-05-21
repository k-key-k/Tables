#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QList>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->tableEmployee->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::sortTable);
    connect(ui->tableEmployee, &QTableWidget::cellDoubleClicked, this, &MainWindow::editCell);

    QAction *openAction = new QAction(tr("&Open File"), this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    ui->menubar->addAction(openAction);

    QAction *saveAction = new QAction(tr("&Save File"), this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    ui->menubar->addAction(saveAction);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_addEmployeeBtn_clicked()
{
    QString surname = ui->surnameEdit->text();
    QString name = ui->nameEdit->text();
    QString position = ui->positionEdit->text();
    int age = ui->ageEdit->text().toInt();

    if (!surname.isEmpty() && !name.isEmpty() && !position.isEmpty() && age > 0) {
        Employee employee(name, surname, position, age);
        employees.append(employee);
        updateTable();
        clearInputs();
    } else {
        QMessageBox::warning(this, "Error", "Please enter all lines correctly.");
    }
}

void MainWindow::updateTable() {
    ui->tableEmployee->clearContents();
    ui->tableEmployee->setRowCount(employees.size());
    int row = 0;
    for (const auto& employee : employees) {
        QTableWidgetItem *surnameItem = new QTableWidgetItem(employee.surname);
        ui->tableEmployee->setItem(row, 0, surnameItem);

        QTableWidgetItem *nameItem = new QTableWidgetItem(employee.name);
        ui->tableEmployee->setItem(row, 1, nameItem);

        QTableWidgetItem *positionItem = new QTableWidgetItem(employee.position);
        ui->tableEmployee->setItem(row, 2, positionItem);

        QTableWidgetItem *ageItem = new QTableWidgetItem(QString::number(employee.age));
        ui->tableEmployee->setItem(row, 3, ageItem);

        ++row;
    }
}

void MainWindow::clearInputs(){
    ui->surnameEdit->clear();
    ui->nameEdit->clear();
    ui->positionEdit->clear();
    ui->ageEdit->clear();
}

void MainWindow::on_deleteEmployeeBtn_clicked()
{
    int index = ui->tableEmployee->currentRow();
    if (index != -1) {
        employees.removeAt(index);
        updateTable();
    }
}

void MainWindow::sortTable(int column) {
    static bool ascending = true;
    ui->tableEmployee->sortItems(column, ascending ? Qt::AscendingOrder : Qt::DescendingOrder);
    ascending = !ascending;
}

void MainWindow::editCell(int row, int column) {
    QTableWidgetItem *item = ui->tableEmployee->item(row, column);
    if (item != nullptr){
        ui->tableEmployee->editItem(item);
    }
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            in.setCodec("UTF-8");
            employees.clear();
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList fields = line.split(",");
                if (fields.size() == 4) {
                    QString surname = fields[0];
                    QString name = fields[1];
                    QString position = fields[2];
                    int age = fields[3].toInt();
                    Employee employee(name, surname, position, age);
                    employees.append(employee);
                }
            }
            file.close();
            updateTable();
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Could not open file for reading."));
        }
    }
}

void MainWindow::saveFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (const auto& employee : employees) {
                out << employee.surname << "," << employee.name << "," << employee.position << "," << employee.age << "\n";
            }
            file.close();
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Could not open file for writing."));
        }
    }
}




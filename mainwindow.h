#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    class Employee {
    public:
        QString name;
        QString surname;
        QString position;
        int age;

        Employee(const QString& name, const QString& surname, QString& position, int age)
            : name(name), surname(surname), position(position), age(age) {}
    };
private slots:
    void on_addEmployeeBtn_clicked();
    void updateTable();
    void clearInputs();
    void sortTable(int column);
    void on_deleteEmployeeBtn_clicked();
    void editCell(int row, int column);
    void openFile();
    void saveFile();



private:
    Ui::MainWindow *ui;

    QList<Employee> employees;
};
#endif // MAINWINDOW_H

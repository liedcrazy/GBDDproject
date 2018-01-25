#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

//для работы БД
#include "QtSql"

//инклуды для таблицы
#include "QStandardItemModel"
#include "QStandardItem"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_action_1_triggered();

    void on_pushButton_1_clicked();

    void on_pushButton_3_clicked();

    void on_action_triggered();

private:
    void connectdb();
    void saveAsCSV();
    QString SearchOverlapAdd(QString TableName, QString Column, QString Data);
    QString selectedRow();
    void Edit();

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H


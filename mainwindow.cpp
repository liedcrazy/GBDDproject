#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>//test string
#include <iostream>//
#include <cstdio>

QString EdataID;
QSqlDatabase db;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectdb();
    //запрет на ввод лишнего в едиты
    QRegExp regExp ("[а-я]{1}\[0-9]{3}\[а-я]{2}\\S");
    QRegExpValidator *validator = new QRegExpValidator(regExp, this);
    ui->lineEdit->setValidator(validator);
    QRegExp regExp2 ("[А-Я]{1}\[а-я]+\\S");//ФИО
    validator = new QRegExpValidator(regExp2, this);
    ui->lineEdit_1->setValidator(validator);
    ui->lineEdit_2->setValidator(validator);
    ui->lineEdit_3->setValidator(validator);
    QRegExp regExp3 ("[А-ЯA-Z]{1}\[а-яa-z]+\\S");//номер авто
    validator = new QRegExpValidator(regExp3, this);
    ui->lineEdit_4->setValidator(validator);
    //QRegExp regExp4 ("[а-яА-Я0-9]+"); //вид нарушения
    //validator = new QRegExpValidator(regExp4, this);
    //ui->lineEdit_5->setValidator(validator);
    QRegExp regExp5 ("[0-9]+\\S"); //сумма
    validator = new QRegExpValidator(regExp5, this);
    ui->lineEdit_6->setValidator(validator);

    ui->pushButton_3->setVisible(false);
    VisibleWiget();
}

void MainWindow::connectdb() //подкл бд и отрисовка табл. данных в табл.
{
    //проверяем созано ли уже подключение и если нет подключаем базу данных
    //QSqlDatabase db;
    if(QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        db = QSqlDatabase::database();
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("GBDDBaseTest.db");
    }
    if (!db.open()) {
        qDebug() << "Что-то пошло не так!";
        QMessageBox::warning(this, "Внимание","Не удалось открыть БД!");
    }

    //Осуществляем запрос
    QSqlQuery query;
    query.exec("SELECT Data.ID, CarBrand.Brand, CarNumber.Number, People.FullName, Violation.Type , Data.Payment, Data.DateViolation FROM CarNumber, Data, People, CarBrand, Violation WHERE CarNumber.ID = Data.CarNumber_ID and People.ID = Data.People_ID and CarBrand.ID = Data.CarBrand_ID and Violation.ID = Data.Violation_ID");

    //рисуем таблицу
    QStandardItemModel *model = new QStandardItemModel;
        QStandardItem *item;

        //Заголовки столбцов
        QStringList horizontalHeader;
        horizontalHeader.append("Номер квитанции");
        horizontalHeader.append("CarBrand");
        horizontalHeader.append("CarNumber");
        horizontalHeader.append("People");
        horizontalHeader.append("ViolationType");
        horizontalHeader.append("Payment");
        horizontalHeader.append("DateViolation");

        model->setHorizontalHeaderLabels(horizontalHeader);

    //Выводим значения из запроса
    int i=0;
    while (query.next())
    {
        QString ID = query.value(0).toString();
        QString CarBrand = query.value(1).toString();
        QString CarNumber = query.value(2).toString();
        QString People = query.value(3).toString();
        QString ViolationType = query.value(4).toString();
        QString Payment = query.value(5).toString();
        QString DateViolation = query.value(6).toString();

        //вывод в таблицу по рядам
        item = new QStandardItem(ID);
        model->setItem(i, 0, item);
        item = new QStandardItem(CarBrand);
        model->setItem(i, 1, item);
        item = new QStandardItem(CarNumber);
        model->setItem(i, 2, item);
        item = new QStandardItem(People);
        model->setItem(i, 3, item);
        item = new QStandardItem(ViolationType);
        model->setItem(i, 4, item);
        item = new QStandardItem(Payment);
        model->setItem(i, 5, item);
        item = new QStandardItem(DateViolation);
        model->setItem(i, 6, item);

        i++;

        ui->tableView->setModel(model);
        ui->tableView->resizeRowsToContents();
        ui->tableView->resizeColumnsToContents();
    }

}

void MainWindow::VisibleWiget() //прячет или показывает виджет
{
    if (ui->tableView->isVisible())
    {
        ui->tableView->setVisible(false);
        ui->lineEdit->setVisible(true);
        ui->lineEdit_1->setVisible(true);
        ui->lineEdit_2->setVisible(true);
        ui->lineEdit_3->setVisible(true);
        ui->lineEdit_4->setVisible(true);
        ui->lineEdit_5->setVisible(true);
        ui->lineEdit_6->setVisible(true);
        ui->label->setVisible(true);
        ui->label_1->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_4->setVisible(true);
        ui->label_5->setVisible(true);
        ui->label_6->setVisible(true);
        ui->label_8->setVisible(true);
        ui->dateEdit->setVisible(true);
        ui->pushButton_3->setVisible(true);
        ui->action_1->setVisible(false);
    }else{
        ui->tableView->setVisible(true);
        ui->lineEdit->setVisible(false);
        ui->lineEdit_1->setVisible(false);
        ui->lineEdit_2->setVisible(false);
        ui->lineEdit_3->setVisible(false);
        ui->lineEdit_4->setVisible(false);
        ui->lineEdit_5->setVisible(false);
        ui->lineEdit_6->setVisible(false);
        ui->label->setVisible(false);
        ui->label_1->setVisible(false);
        ui->label_2->setVisible(false);
        ui->label_3->setVisible(false);
        ui->label_4->setVisible(false);
        ui->label_5->setVisible(false);
        ui->label_6->setVisible(false);
        ui->label_8->setVisible(false);
        ui->dateEdit->setVisible(false);
        ui->pushButton_3->setVisible(false);
        ui->action_1->setVisible(true);
    }
}

void MainWindow::saveAsCSV() //сохраняет файл в указанную директорию
{

    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "",tr("CSV Files (*.csv)"));
    QFile f(filename);
    if (filename == "") {
        QMessageBox::warning(this, "Внимание","Имя файла пустое! "+filename);
            // error message
}

    if( f.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &f );
        QStringList strList;

        strList << "\" \"";
        for( int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c )
            strList << "\""+ui->tableView->model()->headerData(c, Qt::Horizontal).toString()+"\"";
        ts << strList.join( ";" )+"\n";

        for( int r = 0; r < ui->tableView->verticalHeader()->count(); ++r )
        {
            strList.clear();
            strList << "\""+ui->tableView->model()->headerData(r, Qt::Vertical).toString()+"\"";
            for( int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c )
            {
                strList << "\""+ui->tableView->model()->data(ui->tableView->model()->index(r, c), Qt::DisplayRole).toString()+"\"";
            }
            ts << strList.join( ";" )+"\n";
        }
        f.close();
        QMessageBox::warning(this, "Внимание","Файл выгружен в "+filename);
    }
}

QString MainWindow::selectedRow()
{
    //тут происходит магия поимки id из выделенной строки
    int row = ui->tableView->currentIndex().row();
    QString dataID = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString();
    return dataID;
}

QString MainWindow::SearchOverlapAdd (QString TableName, QString Column, QString Data) //Поиск совпадений, если не нашел добавляет
{
    if ((Data != "") && (Data != "  "))
    {
        QSqlQuery query;
        query.exec("SELECT ID FROM "+TableName+" WHERE "+Column+" Like '"+Data+"'");
        query.next();
        QString ID = query.value(0).toString();
        qDebug() << ID <<" Вот что нашел из "<<Data;
        if (ID == "")
        {
            query.exec("INSERT INTO "+TableName+"("+Column+") VALUES('"+Data+"')");
            query.exec("SELECT ID FROM "+TableName+" WHERE "+Column+" Like '"+Data+"'");
            query.next();
            ID = query.value(0).toString();
            qDebug() << ID <<" Вот это добавил "<<Data;
        }
        return ID;
    }else{
        qDebug() <<"Внимание!!! Нет данных для поиска!!!";
        return "";
    }
}

void MainWindow::Edit() //Редактирование
{
    QSqlQuery query;
    if (ui->tableView->isVisible()){EdataID = selectedRow();}
    qDebug() << EdataID <<"-ID \n\r";

    if (ui->tableView->isVisible() && EdataID != "") //отображение данных в edit
    {
        //запрос
        query.exec("SELECT Data.ID, People.FullName, CarNumber.Number, CarBrand.Brand, Violation.Type , Data.Payment, Data.DateViolation FROM CarNumber, Data, People, CarBrand, Violation WHERE Data.ID ="+EdataID+" and CarNumber.ID = Data.CarNumber_ID and People.ID = Data.People_ID and CarBrand.ID = Data.CarBrand_ID and Violation.ID = Data.Violation_ID");
        qDebug() << "Нашел! ID="<<EdataID;

        query.next();
        QString FullName = query.value(1).toString();
        QStringList FullNameList = FullName.split(' ');

        QDate Date = Date.fromString(query.value(6).toString(),"dd.MM.yyyy");
        qDebug()<<"Дата "<<Date;

        ui->lineEdit_1->setText(FullNameList[0]);
        ui->lineEdit_2->setText(FullNameList[1]);
        ui->lineEdit_3->setText(FullNameList[2]);
        ui->lineEdit->setText(query.value(2).toString());
        ui->lineEdit_4->setText(query.value(3).toString());
        ui->lineEdit_5->setText(query.value(4).toString());
        ui->lineEdit_6->setText( query.value(5).toString());
        ui->dateEdit->setDate(Date);

    }
    if (!ui->tableView->isVisible())//обновление данных
    {
        QString Payment, People_ID, CarNumber_ID, CarBrand_ID, Violation_ID, DateViolation;
        //ФИО
        People_ID = ui->lineEdit_1->text()+" "+ui->lineEdit_2->text()+" "+ui->lineEdit_3->text();
        People_ID = SearchOverlapAdd("People", "FullName", People_ID);
        //Номер авто
        CarNumber_ID = ui->lineEdit->text();
        CarNumber_ID = SearchOverlapAdd("CarNumber", "Number", CarNumber_ID);
        qDebug() << CarNumber_ID;
        //Марка
        CarBrand_ID = ui->lineEdit_4->text();
        CarBrand_ID = SearchOverlapAdd("CarBrand", "Brand", CarBrand_ID);
        //Вид Нарушения
        Violation_ID = ui->lineEdit_5->text();
        Violation_ID = SearchOverlapAdd("Violation", "Type", Violation_ID);
        //Сумма оплаты
        Payment = ui->lineEdit_6->text();
        //Дата нарушения
        DateViolation= ui->dateEdit->text();
        //Запрос на редактирование данных в таблице
        if ((Payment != "") && (People_ID != "") && (CarNumber_ID != "") && (CarBrand_ID != "") && (Violation_ID != "") && (DateViolation != ""))
        {
            QSqlQuery query;
            query.exec("UPDATE Data SET Payment = "+Payment+", People_ID = "+People_ID+", CarNumber_ID = "+CarNumber_ID+", CarBrand_ID = "+CarBrand_ID+", Violation_ID = "+Violation_ID+", DateViolation = '"+DateViolation+"' WHERE ID = "+EdataID);
            qDebug() << "Редактирование успешно! "<<" UPDATE Data SET "+Payment+", "+People_ID+", "+CarNumber_ID+", "+CarBrand_ID+", "+Violation_ID+", "+DateViolation+"' WHERE ID = "+EdataID;
        }else{
            QMessageBox::warning(this, "Внимание","Добавить не удалось!!!");
            qDebug() << "Редактирование не успешно! "<<" UPDATE Data SET "+Payment+", "+People_ID+", "+CarNumber_ID+", "+CarBrand_ID+", "+Violation_ID+", "+DateViolation+"' WHERE ID = "+EdataID;
        }
        CarNumber_ID = "";
        ui->lineEdit->setText("");
        People_ID = "";
        ui->lineEdit_1->setText("");
        ui->lineEdit_2->setText("");
        ui->lineEdit_3->setText("");
        CarBrand_ID = "";
        ui->lineEdit_4->setText("");
        Violation_ID = "";
        ui->lineEdit_5->setText("");
        Payment = "";
        ui->lineEdit_6->setText("");
        EdataID = "";//почистим

        QMessageBox::warning(this, "Внимание","Редактирование успешно!");
    }
    if(ui->pushButton->isVisible()){
        ui->pushButton->setVisible(false);
    }else{ui->pushButton->setVisible(true);}
    if(ui->pushButton_2->isVisible()){
        ui->pushButton_2->setVisible(false);
    }else{ui->pushButton_2->setVisible(true);}
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() //добавление
{
    if (!ui->tableView->isVisible())
    {
        QString Payment, People_ID, CarNumber_ID, CarBrand_ID, Violation_ID, DateViolation;
        //ФИО
        People_ID = ui->lineEdit_1->text()+" "+ui->lineEdit_2->text()+" "+ui->lineEdit_3->text();
        People_ID = SearchOverlapAdd("People", "FullName", People_ID);
        //Номер авто
        CarNumber_ID = ui->lineEdit->text();
        CarNumber_ID = SearchOverlapAdd("CarNumber", "Number", CarNumber_ID);
        //Марка
        CarBrand_ID = ui->lineEdit_4->text();
        CarBrand_ID = SearchOverlapAdd("CarBrand", "Brand", CarBrand_ID);
        //Вид Нарушения
        Violation_ID = ui->lineEdit_5->text();
        Violation_ID = SearchOverlapAdd("Violation", "Type", Violation_ID);
        //Сумма оплаты
        Payment = ui->lineEdit_6->text();
        //Дата нарушения
        DateViolation= ui->dateEdit->text();
        //Запрос на добавление данных в таблицу
        if ((Payment != "") && (People_ID != "") && (CarNumber_ID != "") && (CarBrand_ID != "") && (Violation_ID != "") && (DateViolation != ""))
        {
            QSqlQuery query;
            query.exec("INSERT INTO Data(Payment, People_ID, CarNumber_ID, CarBrand_ID, Violation_ID, DateViolation) VALUES('"+Payment+"', '"+People_ID+"', '"+CarNumber_ID+"', '"+CarBrand_ID+"', '"+Violation_ID+"', '"+DateViolation+"')");
            qDebug() << "Добавить удалось! "<<" VALUES('"+Payment+"', '"+People_ID+"', '"+CarNumber_ID+"', '"+CarBrand_ID+"', '"+Violation_ID+"', '"+DateViolation+"')";
        }else{
            QMessageBox::warning(this, "Внимание","Добавить не удалось!!!");
            qDebug() << "Добавить не удалось! "<<"VALUES('"+Payment+"', '"+People_ID+"', '"+CarNumber_ID+"', '"+CarBrand_ID+"', '"+Violation_ID+"', '"+DateViolation+"')";
        }
        CarNumber_ID = "";
        ui->lineEdit->setText("");
        People_ID = "";
        ui->lineEdit_1->setText("");
        ui->lineEdit_2->setText("");
        ui->lineEdit_3->setText("");
        CarBrand_ID = "";
        ui->lineEdit_4->setText("");
        Violation_ID = "";
        ui->lineEdit_5->setText("");
        Payment = "";
        ui->lineEdit_6->setText("");
    }
    if(ui->pushButton_1->isVisible()){
        ui->pushButton_1->setVisible(false);
    }else{ui->pushButton_1->setVisible(true);}
    if(ui->pushButton_2->isVisible()){
        ui->pushButton_2->setVisible(false);
    }else{ui->pushButton_2->setVisible(true);}
    VisibleWiget();
    connectdb();
}

void MainWindow::on_pushButton_2_clicked() //удаление выделенной строки
{

    //тут происходит магия поимки id из выделенной строки
    int row = ui->tableView->currentIndex().row();
    QString dataIDtable = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString();
    qDebug() << dataIDtable <<"-ID \n\r";
    if(dataIDtable != "")
    {
        //запрос на удаление из БД
        QSqlQuery query;
        query.exec("DELETE FROM Data WHERE id="+dataIDtable); //запрос в БД на удаление строки
        ui->tableView->setRowHidden(row, true);                 //убираем строку которую удалили
        qDebug() <<  row << dataIDtable <<"Убрано \n\r";
    }else{QMessageBox::warning(this, "Внимание","Ничего не выделено!");}
}

void MainWindow::on_action_1_triggered() //Выгрузка
{
    saveAsCSV();
}

void MainWindow::on_pushButton_1_clicked() //Редактирование
{
    if(selectedRow() == "" && ui->tableView->isVisible())
    {
        QMessageBox::warning(this, "Внимание","Ничего не выделено!");
    }else{
        Edit();
        VisibleWiget();
        connectdb();

    }
}

void MainWindow::on_pushButton_3_clicked() //Отмена
{
    ui->lineEdit->setText("");
    ui->lineEdit_1->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    ui->lineEdit_6->setText("");
    ui->pushButton->setVisible(true);
    ui->pushButton_1->setVisible(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton_3->setVisible(false);
    VisibleWiget();
}

void MainWindow::on_action_triggered()
{
    db.close();
    close();
}

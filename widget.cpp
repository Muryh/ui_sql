#include "widget.h"
#include "ui_widget.h"
#include <iostream>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    creatDatabase();
    init_widget();





}

Widget::~Widget()
{
    delete ui;
}

void Widget::creatDatabase()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");//声明数据库
    database.setDatabaseName("/home/joo/user.db");//数据库命名

    if(!database.open())
    {
        qDebug()<< "Error: connection with database failed";
    }
    else {
        qDebug() << "Database: connection ok";
    }

    QSqlQuery query;
    //定义表的结构：表名：users， id, username, password
    query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT NOT NULL, password TEXT NOT NULL)");


}

bool Widget::userExisted(const QString &username)//判断用户是否存在
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if(!query.exec())
    {
        qDebug() << "userExists error: " << query.lastError();
        return false;
    }

    if(query.next() &&query.value(0).toInt() == 0)
    {
        return false;
    }

    return true;
}

bool Widget::addUser(const QString &username, const QString &password)
{
    if(userExisted(username))
    {
        QMessageBox::warning(this, "WARING", "USER HAS EXISTED!");
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username,password)VALUES(:username,:password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec())
    {
        qDebug() << "addUser error: " << query.lastError();
        return false;
    }
    else
    {

        qDebug() << "User added successfully!";
    }
    QMessageBox::information(this, "info", "User added successfully!");
    return true;
}

bool Widget::verifyUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username AND password =:password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if(query.exec() && query.next() && query.value(0).toInt() > 0)
    {
        QMessageBox::information(this, "提示", "登陆成功!");
        return  true;
    }
    else {
        QMessageBox::warning(this, "WARNING", "Username or Password ERROR!");
        return false;
    }
}

bool Widget::findUserPassword(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);
    std::cout<<username.toStdString()<<std::endl;

    if(query.exec())
    {
        while(query.next())
        {
            ui->LBL_show_password->setText(query.value(0).toString());
            return  true;
        }

    }
    else
    {
        QMessageBox::warning(this, "WARNING", "USER NOT FOUND!");
    }
    return false;
}

bool Widget::deleteUser(const QString &username)
{
    if(!userExisted(username))//用户不存在
    {
        QMessageBox::warning(this, "WARNING", "USER DOESN'T EXIST!");
        return false;
    }
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if(!query.exec())
    {
        qDebug() << "deleteUser error: " << query.lastError();
        return false;
    }
    else
    {
        QMessageBox::information(this, "info", "DELETE USER FUNISHED!");
        return true;
    }
}



void Widget::init_widget()
{
    ui->CB_Username->clear();
    ui->CB_delete_user->clear();
    ui->CB_find_user->clear();

    ui->LE_add_username->clear();
    ui->LE_add_password->clear();

    ui->LBL_show_password->clear();
    populateCB();
}

void Widget::populateCB()
{
    QSqlQuery query("SELECT username FROM users");
    if(!query.exec())
    {
       qDebug() << "Error: query execution failed" << query.lastError();
       return;
    }

    while(query.next())
    {
        QString username = query.value(0).toString();
        ui->CB_Username->addItem(username);
        ui->CB_delete_user->addItem(username);
        ui->CB_find_user->addItem(username);
    }
}



void Widget::on_BT_delete_user_clicked()
{
    QString username = ui->CB_delete_user->currentText();
    deleteUser(username);
    init_widget();//更新COMBOBOX
}

void Widget::on_BT_find_clicked()
{
    QString username = ui->CB_find_user->currentText();
    findUserPassword(username);
}

void Widget::on_BT_addUser_clicked()
{
    QString username = ui->LE_add_username->text();
    QString password = ui->LE_add_password->text();

    if(password.isEmpty() || username.isEmpty())
    {
        QMessageBox::warning(this, "WARNING", "username or password can't be empty!");
        return;
    }
    addUser(username, password);
    init_widget();
}

void Widget::on_BT_Login_clicked()
{
  QString username;
  QString password;
  username = ui->CB_Username->currentText();
  password = ui->LE_password->text();
  verifyUser(username, password);
}

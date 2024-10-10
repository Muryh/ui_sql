#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_BT_addUser_clicked();

    void on_BT_Login_clicked();

    void on_BT_delete_user_clicked();

    void on_BT_find_clicked();

private:
    Ui::Widget *ui;

    void init_widget();//初始化控件
    void populateCB();//填充用户选择框


    void creatDatabase();
    bool userExisted(const QString &username);
    bool addUser(const QString &username, const QString &password);
    bool verifyUser(const QString &username, const QString &password);
    bool findUserPassword(const QString &username);
    bool deleteUser(const QString &username);
};

#endif // WIDGET_H

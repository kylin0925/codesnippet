#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QtSql>
#include <QtSql/QSqlQuery>
#include <QListWidgetItem>
#include <QVector>
#include <QStringList>
#include <QMap>
#include "hightlight.h"
namespace Ui {
class MainWindow;
}
class datahelper{
private:
    QString dbfile;
    QSqlDatabase db;
    int total_code;
    int total_tags;

public:
    datahelper();
    ~datahelper();
    void checkdb();
    void createtbl();
    void checktable();
    //modify data
    int addcode(QString title,QString code,QStringList tag);
    int update(QString tar_title,QString title,QString code,QStringList tag);
    int addTags(QStringList tag,QString code_no);
    int deleteCode(QString code_no);
    //query
    int queryTag(QString tag);
    QString queryCode(QString code_no);
    void loaddata();
    void loaddata(QString tag);
    QStringList queryCodeTag(QString code_no);
   // void add(QString code,QString tag);
    QStringList title_code;
    QStringList tag_list;
    QStringList ui_tag_list;
    QStringList code_no;


};
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_lstTitle_itemClicked(QListWidgetItem *item);

    void on_btnNew_clicked();

    void on_btnAdd_clicked();

    void on_btnCancel_clicked();

    void on_btnModify_clicked();

    void on_btnDel_clicked();

    void on_edtTag_returnPressed();

    void on_lstCodeTag_doubleClicked(const QModelIndex &index);

    void on_lstTag_clicked(const QModelIndex &index);

    void on_lstTitle_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *);
    datahelper *db;
    void clear_ui();
    void update_ui();
    hightlight *hter;
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QList>
#include <QCryptographicHash>
#include <QByteArray>
#include <QListWidgetItem>

//database
// code_num,title,code,date
// no,tag,code_num

datahelper::datahelper()
{
    qDebug("datahelper");
    dbfile = "db.db";
    QFile qf(dbfile);
    int exists = qf.exists();

    db =  QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbfile);
    db.open();

    if( exists == false)
    {
        qDebug("database not exists");
        createtbl();
    }

}
datahelper::~datahelper()
{
    qDebug("~datahelper");
    db.close();
}

void datahelper::createtbl()
{
    QString codetbl = "create table codetbl(code_no text,title text,code text,date text)";
    QString taglist = "create table taglist(no text,tag text,code_no text)";

    QSqlQuery query;
    qDebug("createtbl");
    query.exec(codetbl);
    query.exec(taglist);
}

void datahelper::checktable()
{
    QString chksql = "select count(*) from sqlite_master";
    QSqlQuery query;
    int cnt =0;
    query.exec(chksql);
    query.next();

    cnt = (query.value(0).toInt());

    if (cnt == 0)
        createtbl();
}

void datahelper::checkdb()
{
    qDebug("checkdb");
    QFile qf(dbfile);
    if(qf.exists() == false)
    {
        qDebug("database not exists");
        createtbl();
    }
    else
    {
        qDebug("database ok");
    }
}

int datahelper::addTags(QStringList tag,QString code_no)
{
    QSqlQuery query;
    query.prepare("insert into taglist (no,tag,code_no) values (?,?,?)");
    qDebug()<<"addTags";
    //add tag
    int cnt = tag.count();
    int i;

    for(i=0;i<cnt;i++)
    {
        query.addBindValue(total_tags);
        query.addBindValue(tag[i]);
        query.addBindValue(code_no);
        bool r = query.exec();
        total_tags++;
    }
    return 0;
}

int datahelper::addcode(QString title,QString code,QStringList tag)
{
    QDateTime qt = QDateTime::currentDateTime();
    QString d = qt.toString("yyyy/MM/dd HH:mm:ss") ;
    QSqlQuery query;
    QByteArray sh = QCryptographicHash::hash((title+d).toUtf8(),QCryptographicHash::Sha1);

    QString code_no(sh.toHex());
    qDebug()<<code_no;


    query.prepare("insert into codetbl (code_no ,title,code ,date) values (?,?,?,?)");
    query.addBindValue(code_no);
    query.addBindValue(title);
    query.addBindValue(code);
    query.addBindValue(d);
    bool r = query.exec();
    if(r == true){
        addTags(tag,code_no);//add tags

        this->title_code.append(title);
        this->code_no.append(code_no);
    }

    return 0;
}

int datahelper::update(QString code_no,QString title,QString code,QStringList tag)
{
    QSqlQuery query;
    query.prepare("update codetbl set title=?,code=? where code_no=?");

    query.addBindValue(title);
    query.addBindValue(code);
    query.addBindValue(code_no);
    query.exec();

    //delete all tag
    query.prepare("delete from taglist where code_no=?");
    query.addBindValue(code_no);
    bool r = query.exec();
    if(r)
    {
        //add
        addTags(tag,code_no);
    }
}

int datahelper::queryTag(QString tag)
{
    QString chksql = "select count(tag) from taglist where tag=""%s"" ";
    QSqlQuery query;
    int cnt=0;
    QString sql;
    sql.sprintf(chksql.toStdString().c_str(),tag.toStdString().c_str());
    query.exec(sql);
    query.next();
    cnt = query.value(0).toInt();

    return cnt;
}

QString datahelper::queryCode(QString code_no)
{
    QString sql = "select code from codetbl where code_no=?";
    QSqlQuery query;
    QString code=0;
    qDebug()<<code_no;

    query.prepare(sql);
    query.addBindValue(code_no);
    query.exec();
    query.next();

    code = query.value(0).toString();

    return code;
}

QStringList datahelper::queryCodeTag(QString code_no)
{
    QString sql = "select tag from taglist where code_no=?";
    QSqlQuery query;
    QStringList taglist;
    qDebug()<<"queryCodeTag "<<code_no;

    query.prepare(sql);
    query.addBindValue(code_no);
    query.exec();
    while(query.next())
    {
        taglist.append( query.value(0).toString());
        qDebug()<<query.value(0).toString();
    }

    return taglist;
}
int datahelper::deleteCode(QString code_no)
{
    QString sql = "delete from codetbl where code_no=?";
    QSqlQuery query;
    QStringList taglist;
    qDebug()<<"queryCodeTag "<<code_no;
    query.prepare(sql);
    query.addBindValue(code_no);
    query.exec();
    sql = "delete from taglist where code_no=?";

    query.prepare(sql);
    query.addBindValue(code_no);
    query.exec();
}
void datahelper::loaddata(QString tag)
{
    QString sql = "select code_no,title from codetbl where code_no in (select code_no from taglist where tag like ?)";
    QSqlQuery query;
    total_code = 0;
    total_tags = 0;

    title_code.clear();

    query.prepare(sql);
    query.addBindValue(tag);
    query.exec();
    while(query.next()){
        this->title_code.append(query.value(1).toString());
        this->code_no.append(query.value(0).toString());

        qDebug()<<query.value(0).toString()<<query.value(1).toString();
        total_code++;
    }

    return;
}
void datahelper::loaddata()
{
    QString sql = "select code_no,title from codetbl";

    QSqlQuery query;
    total_code = 0;
    total_tags = 0;

    title_code.clear();
    tag_list.clear();
    ui_tag_list.clear();
    query.exec(sql);
    while(query.next()){
        this->title_code.append(query.value(1).toString());
        this->code_no.append(query.value(0).toString());

        qDebug()<<query.value(0).toString()<<query.value(1).toString();
        total_code++;
    }

    QString sel = "";
    QString tmp;
    int cnt=0;
    sql = "select tag from taglist order by tag";
    query.exec(sql);
    this->tag_list.append("All");
    this->ui_tag_list.append("All");
    while(query.next()){
        if(sel == "")
        {
            sel = query.value(0).toString();
            qDebug()<<" sel "<<sel;
        }
        else if(sel ==query.value(0).toString())
        {
            cnt ++;
            qDebug()<<" cnt "<<cnt;
        }
        else
        {
            tmp.sprintf("%d",cnt+1);
            qDebug()<< tmp;
            //this->tag_list.append(query.value(0).toString());
            this->ui_tag_list.append(sel + " (" + tmp + ")");
            this->tag_list.append(sel);
            cnt =0;
            sel = query.value(0).toString();
        }
        total_tags++;
    }
    tmp.sprintf("%d",cnt+1);
    qDebug()<<"sel "<<sel <<" cnt "<<cnt;
    this->ui_tag_list.append(sel + " (" + tmp + ")");
    this->tag_list.append(sel);
    return;
}

//------------------
//
//
void MainWindow::clear_ui()
{
    ui->lstTitle->clear();
    ui->lstTag->clear();
    ui->lstCodeTag->clear();
}
void MainWindow::update_ui()
{
    clear_ui();
    db->checkdb();
    db->loaddata();
    ui->lstTitle->addItems(db->title_code);
    ui->lstTag->addItems(db->ui_tag_list);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDateTime qt = QDateTime::currentDateTime();

    //ui->lblmsg->setText(qt.toString("yyyy/MM/dd HH:mm:ss") );

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    db = new datahelper();
   /* db->checkdb();
    db->loaddata();
    ui->lstTitle->addItems(db->title_code);
    ui->lstTag->addItems(db->tag_list);*/
    update_ui();

    hter = new hightlight(ui->txtCode->document());
//    QList<int> l;
//    l.append(70);
//    l.append(200);
//    ui->splitterMain->setSizes(l);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
   // QDateTime qt = QDateTime::currentDateTime();

    //ui->lblmsg->setText(qt.toString("yyyy/MM/dd HH:mm:ss") );
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug("on_listWidget_itemClicked ok");
    qDebug()<<item->text();
}

void MainWindow::on_lstTitle_itemClicked(QListWidgetItem *item)
{
    QString code;
    int row = ui->lstTitle->currentIndex().row();

    //qDebug()<<db->code_no[row];

    code = db->queryCode(db->code_no[row]);

    QStringList taglst =db->queryCodeTag(db->code_no[row]);
    ui->lstCodeTag->clear();
    ui->lstCodeTag->addItems(taglst);
//show
    ui->edtTitle->setText(item->text());
    ui->txtCode->setPlainText(code);
// change ui
    ui->edtTitle->setEnabled(true);
    ui->txtCode->setEnabled(true);
    ui->edtTag->setEnabled(true);

    ui->btnAdd->setEnabled(false);
    ui->btnModify->setEnabled(true);
    ui->btnCancel->setEnabled(true);
    ui->btnDel->setEnabled(true);

   // qDebug()<<db->code_no[ui->lstTitle->currentIndex()];

}

void MainWindow::on_btnAdd_clicked()
{
    QDateTime qt = QDateTime::currentDateTime();

    if(ui->edtTitle->text() == "")
        QMessageBox::warning(this,"test","input title");
    else if( ui->txtCode->toPlainText() == "")
        QMessageBox::warning(this,"test","input code");
    else
    {
        ui->lstTitle->addItem(ui->edtTitle->text());
        if(ui->edtTag->text() != "")
            ui->lstTag->addItem(ui->edtTag->text());

        QStringList tags;
        int n;
        for(n = 0;n<ui->lstCodeTag->count();n++)
            tags.append(ui->lstCodeTag->item(n)->text());

        db->addcode(ui->edtTitle->text(),ui->txtCode->toPlainText(),tags);
        update_ui();
    }

    ui->btnCancel->setEnabled(false);
    ui->edtTitle->setEnabled(false);
    ui->txtCode->setEnabled(false);
    ui->edtTag->setEnabled(false);
    ui->btnNew->setEnabled(true);
    ui->btnAdd->setEnabled(false);

    ui->edtTitle->setText("");
    ui->txtCode->setPlainText("");
    ui->edtTag->setText("");

    ui->lstTitle->setEnabled(true);
    ui->lstTag->setEnabled(true);
}

void MainWindow::on_btnCancel_clicked()
{
    ui->btnCancel->setEnabled(false);
    ui->edtTitle->setEnabled(false);
    ui->txtCode->setEnabled(false);
    ui->edtTag->setEnabled(false);
    ui->btnNew->setEnabled(true);
    ui->btnAdd->setEnabled(false);

    ui->lstTitle->setEnabled(true);
    ui->lstTag->setEnabled(true);
}

void MainWindow::on_btnNew_clicked()
{
    ui->btnCancel->setEnabled(true);
    ui->edtTitle->setEnabled(true);
    ui->txtCode->setEnabled(true);
    ui->edtTag->setEnabled(true);
    ui->btnNew->setEnabled(false);
    ui->btnAdd->setEnabled(true);
    ui->btnModify->setEnabled(false);
    ui->btnDel->setEnabled(false);
    ui->lstTitle->setEnabled(false);
    ui->lstTag->setEnabled(false);
    ui->edtTitle->setText("");
    ui->txtCode->setPlainText("");
    ui->edtTag->setText("");
    ui->lstCodeTag->clear();
}

void MainWindow::on_btnModify_clicked()
{
    int row = ui->lstTitle->currentIndex().row();
    qDebug()<<ui->lstTitle->currentIndex().row();
    qDebug()<<db->code_no[row];
    QString code_no = db->code_no[row];
    qDebug()<<code_no;
    QStringList tags;
    //
    int n;
    for(n = 0;n<ui->lstCodeTag->count();n++)
        tags.append(ui->lstCodeTag->item(n)->text());
    //

    db->update(code_no,ui->edtTitle->text(),ui->txtCode->toPlainText(),tags);

    //QListWidgetItem *i = ui->lstTitle->currentItem();
    //i->setText(ui->edtTitle->text());
    update_ui();
}

void MainWindow::on_btnDel_clicked()
{
    int row = ui->lstTitle->currentIndex().row();
    qDebug()<<ui->lstTitle->currentIndex().row();
    qDebug()<<db->code_no[row];
    QString code_no = db->code_no[row];
    qDebug()<<code_no;
    db->deleteCode(code_no);
    update_ui();
}

void MainWindow::on_edtTag_returnPressed()
{
    QString tag = ui->edtTag->text();
    qDebug()<<tag;
    if(tag!=""){
        int cnt = ui->lstCodeTag->findItems(tag,Qt::MatchFixedString).count();
        if(cnt == 0){
            ui->lstCodeTag->addItem(tag);
            ui->edtTag->setText("");
        }
    }
}

void MainWindow::on_lstCodeTag_doubleClicked(const QModelIndex &index)
{
    int row = ui->lstCodeTag->currentIndex().row();
    ui->lstCodeTag->takeItem(row);
}

void MainWindow::on_lstTag_clicked(const QModelIndex &index)
{
    int row = ui->lstTag->currentIndex().row();

    QString tag = db->tag_list[row];// ui->lstTag->currentItem()->text();
    qDebug()<<"on_lstTag_clicked "<<row<<" "<<tag;
    if(row == 0)
    {
        db->loaddata();
        update_ui();
    }
    else
    {
        db->loaddata(tag);
        ui->lstTitle->clear();
        ui->lstTitle->addItems(db->title_code);
    }
}

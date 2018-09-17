#include "configs.hpp"
//#include "ui_configs.h"

#include <QDebug>

configs::configs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configs)
{
    ui->setupUi(this);
    //ui->tabWidget->widget(0)->children().at(1)->objectName();
}

configs::~configs()
{
    delete ui;
}

void configs::refresh(QStringList *vars){
    ui->libdir_list->insertRow(ui->libdir_list->rowCount());
    //ui->libdir_list->ad
}

void configs::on_tabWidget_tabBarClicked(int index){
     qDebug()  << ui->tabWidget->widget(index)->objectName();
    //qDebug() << ui->tabWidget->widget(index)->children().toStdList();
}

void configs::on_listView_doubleClicked(const QModelIndex &index)
{

}

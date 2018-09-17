#ifndef LIBMGR_HPP
#define LIBMGR_HPP

#include<QStringList>
#include<library.hpp>
//#include "vars.hpp"

class libmgr
{
public:
    libmgr();
    ~libmgr();
    QList<QTreeWidgetItem*> *dumptree();
private:
    QList<library*> *libs;
    QSqlDatabase *db;
    void initdb();
};

#endif // LIBMGR_HPP

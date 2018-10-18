/*
 * Ocelot Music Manager: A music player and library manager built using Qt
 * Copyright (C) 2018 Matheus Scattolin Anselmo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "libtree.hpp"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFrame>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSqlRecord>
#include <QSqlError>
#include <QMenu>

#include "src/mwindow.hpp"

libtree::libtree(mwindow *win) : QFrame(win){
    QGridLayout *grid =  new QGridLayout(this);
    QLineEdit *filterbox = new QLineEdit(this);
    QPushButton *help = new QPushButton("?", this);
    QPushButton *config = new QPushButton(QIcon(":/internals/wrench"), "", this);
    QLabel *label = new QLabel("Filter", this);
    tree = new QTreeWidget(this);
    tree->setContextMenuPolicy(Qt::CustomContextMenu);

    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Sunken);
    this->layout()->setContentsMargins(1,1,1,1);
    this->layout()->setSpacing(1);
    this->setMinimumWidth(250);
    tree->setHeaderHidden(true);
    help->setMaximumWidth(20);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setMinimumWidth(40);
    filterbox->setPlaceholderText("Enter a keyword or SQL statement to begin");

    grid->addWidget(tree,0,0,1,5);
    grid->addWidget(label,1,1);
    grid->addWidget(filterbox,1,2);
    grid->addWidget(help,1,3);
    grid->addWidget(config,1,4);

    this->setLayout(grid);
    //parent->window()->
    connect(tree, &QTreeWidget::customContextMenuRequested, this, &libtree::menu_items);
    connect(tree, &QTreeWidget::itemDoubleClicked, win, &mwindow::play);
    connect(tree, &QTreeWidget::itemClicked, win, &mwindow::select);
    connect(win, &mwindow::libchanged, this, &libtree::populate);
}

/* take one and pass it ahead */
/* i'm so uglyyyyyyy */// 999663822
void libtree::recurtree(QTreeWidgetItem *parent, QStringList levels, QString conditions, QSqlDatabase *db){
    QString root = levels.first();
    levels.removeFirst();
    QStringList* sroot = libtree::extract(root);
    QString vars = sroot->join(", ");

    QSqlQuery rq;
    rq.prepare("SELECT DISTINCT "+vars+" FROM songs WHERE "+conditions+" ORDER BY "+sroot->first());
    rq.exec();
    while(rq.next()){
        QString name = root;
        sroot = libtree::extract(root);
        for(int i=sroot->length()-1; i>=0; i--){
            name.replace(sroot->at(i), rq.record().value(i).toString());
            sroot->replace(i, sroot->at(i)+"='"+rq.record().value(i).toString()+"'");
        }

        name.remove("#");

        parent->addChild(new QTreeWidgetItem(parent,QStringList(name)));

        QSqlQuery qpath;
        qpath.prepare("SELECT DISTINCT path FROM songs WHERE "+sroot->join(" AND "));
        qpath.exec();
        QStringList act;
        while(qpath.next()){
            act.append(qpath.record().value(0).toString());
        }
        parent->child(parent->childCount()-1)->setData(0, Qt::UserRole, act);

        if(levels.length()>=1){
            libtree::recurtree(parent->child(parent->childCount()-1), levels, conditions+" AND "+sroot->join(" AND "), db);
            parent->child(parent->childCount()-1)->setText(0, parent->child(parent->childCount()-1)->text(0)+" ("+QString::number(parent->child(parent->childCount()-1)->childCount())+")");

        }
    }
}

QStringList* libtree::extract(QString vars){
    QStringList *out = new QStringList();
    QRegularExpressionMatchIterator reg = QRegularExpression("#(\\w+)#").globalMatch(vars);
    while(reg.hasNext())
        out->append(reg.next().captured(1));

    return out;
}

void libtree::menu_items(const QPoint &pos){
    QMenu menu(this);

    const QIcon c = QIcon::fromTheme("gtk-convert");
    const QIcon t = QIcon::fromTheme("tag-symbolic");

    QAction *tagger = new QAction(t, "Mass Tag");
    QAction *conv = new QAction(c, "Transcode");
    QAction *prop = new QAction("Properties");


    //conv->
    //QAction *newAct = new QAction(
    //QAction *newAct = new QAction(
    menu.addAction(conv);
    menu.addAction(tagger);
    menu.addSeparator();
    menu.addAction(prop);

    QPoint pt(pos);
    menu.exec(tree->mapToGlobal(pos));
}

void libtree::populate(QSqlDatabase *db){
    this->tree->clear();

    QStringList order = QString("#artist#/[#year#] #album#/#track#. #title#").split("/");
    QString root = order.first();

    QSqlQuery *query = new QSqlQuery();
    query->prepare("SELECT DISTINCT "+libtree::extract(order.first())->join(", ")+" FROM songs ORDER BY "+libtree::extract(order.first())->join(", "));
    query->exec();
    //query->next();

    order.removeFirst();
    QList<QTreeWidgetItem*> items;

    while(query->next()){

        QStringList *caseq = libtree::extract(root);
        QString name = root;
        for(int i=0; i<caseq->length(); i++){
            name.replace(caseq->at(i), query->record().value(i).toString());
            caseq->replace(i, caseq->at(i) + "='"+query->record().value(i).toString()+"'");
        }
        name.remove("#");

        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(name)));
        QSqlQuery sidequery;
        sidequery.prepare("SELECT DISTINCT path FROM songs WHERE "+caseq->join(" AND "));
        sidequery.exec();
        sidequery.next();
        QStringList act;
        int z=0;
        while(sidequery.record().value(z).isValid()){//////////////////////////
            act.append(sidequery.record().value(z).toString());
            z++;
            sidequery.next();
        }

        qDebug() << act.last();
        items.last()->setData(0, Qt::UserRole, act.last());
        libtree::recurtree(items.last(), order, caseq->join(" AND "), db);
        caseq->~QStringList();
        items.last()->setText(0, items.last()->text(0)+" ("+QString::number(items.last()->childCount())+")");
    }

    this->tree->insertTopLevelItem(0, new QTreeWidgetItem((QTreeWidget*)0, QStringList("")));
    this->tree->topLevelItem(0)->addChildren(items);
    this->tree->topLevelItem(0)->setText(0, "All Media ("+QString::number(this->tree->topLevelItem(0)->childCount())+")");
    this->tree->expandItem(this->tree->topLevelItem(0));
}

libtree::~libtree(){

}

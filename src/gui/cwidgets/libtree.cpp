/*
 * The MIT License (MIT)
 * Copyright (c) 2018 Matheus Scattolin Anselmo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. In no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising from,
 * out of or in connection with the software or the use or other dealings in the
 * Software.
 */

#include "libtree.hpp"
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "src/gui/mwindow.hpp"
#include "src/vars.hpp"

libtree::libtree(mwindow* win, workbench* wb, vars* jag) : QWidget() {
    this->win = win;
    this->setObjectName("libtree");
    QGridLayout* grid = new QGridLayout(this);
    QLineEdit* filterbox = new QLineEdit(this);
    QPushButton* help = new QPushButton("?", this);
    QPushButton* config = new QPushButton(QIcon(":/internals/wrench"), "", this);
    QLabel* label = new QLabel("Filter", this);
    this->wb = wb;
    tree = new QTreeWidget(this);
    tree->setContextMenuPolicy(Qt::CustomContextMenu);

    /* init context menu */
    this->ctx = new QMenu();
    const QIcon c = QIcon::fromTheme("gtk-convert");
    const QIcon t = QIcon::fromTheme("tag-symbolic");
    QAction* tagger = new QAction(QIcon::fromTheme("amarok_playlist"), "Mass Tagger");
    QMenu* conv = new QMenu(this->ctx);
    conv->setIcon(c);
    conv->setTitle("Transcoder");

    QAction* append = new QAction(QIcon::fromTheme("document-new-from-template"), "Enqueue");
    QAction* rep = new QAction(QIcon::fromTheme("document-new"), "Add to New Queue");

    conv->addAction(append);
    conv->addAction(rep);
    conv->addSection("Quick Presets");
    conv->addAction(QIcon::fromTheme("quickopen"), "FakeFLAC");
    conv->addAction(QIcon::fromTheme("quickopen"), "FakeMP3");
    QAction* prop = new QAction(t, "Edit Tag");

    connect(prop, &QAction::triggered, this, &libtree::gatherselected);
    connect(append, &QAction::triggered, this, &libtree::transc_append);
    connect(rep, &QAction::triggered, this, &libtree::transc_replace);
    connect(this, &libtree::transc_dispatch, win, &mwindow::transcoder_spawn);
    connect(this, &libtree::dispatch, win, &mwindow::tageditor_spawn);

    ctx->addMenu(conv);
    ctx->addAction(tagger);
    ctx->addSeparator();
    ctx->addAction(prop);

    // this->sizePolicy().setHorizontalPolicy(QSizePolicy::Minimum);
    // this->setFrameShape(QFrame::StyledPanel);
    // this->setFrameShadow(QFrame::Sunken);
    this->layout()->setContentsMargins(1, 1, 1, 1);
    this->layout()->setSpacing(1);
    this->setMinimumWidth(250);
    tree->setHeaderHidden(true);
    help->setMaximumWidth(20);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setMinimumWidth(40);
    filterbox->setPlaceholderText("Enter a keyword or SQL statement to begin");

    grid->addWidget(tree, 0, 0, 1, 5);
    grid->addWidget(label, 1, 1);
    grid->addWidget(filterbox, 1, 2);
    grid->addWidget(help, 1, 3);
    grid->addWidget(config, 1, 4);

    this->setLayout(grid);
    // parent->window()->
    connect(tree, &QTreeWidget::customContextMenuRequested, this, &libtree::showctx);
    connect(tree, &QTreeWidget::itemDoubleClicked, this, &libtree::media_dispatch);
    connect(tree, &QTreeWidget::itemClicked, win, &mwindow::select);
    connect(win, &mwindow::libchanged, this, &libtree::populate);
}

/* take one and pass it ahead */
/* this fills each node with the corresponding path and name */
void libtree::recurtree(QTreeWidgetItem* parent, QStringList levels, QString conditions, QSqlDatabase* db) {
    QString root = levels.first();
    levels.removeFirst();
    QStringList* sroot = libtree::extract(root);
    QString vars = sroot->join(", ");

    QSqlQuery rq;
    rq.prepare("SELECT DISTINCT " + vars + " FROM songs WHERE " + conditions + " ORDER BY " + sroot->first());
    rq.exec();
    while(rq.next()) { /* query again to fetch paths */
        /* starting by generating condition string */
        QString name = root;
        sroot = libtree::extract(root);
        for(int i = sroot->length() - 1; i >= 0; i--) {
            name.replace(sroot->at(i), rq.record().value(i).toString());
            sroot->replace(i, sroot->at(i) + "=\"" + rq.record().value(i).toString() + "\"");
        }
        name.remove("#");

        /* then running the query itself */
        QStringList act;
        QTreeWidgetItem* child = new QTreeWidgetItem(parent, QStringList(name));
        QSqlQuery qpath;
        qpath.prepare("SELECT DISTINCT path FROM songs WHERE " + sroot->join(" AND ") + " ORDER BY track");
        qpath.exec();

        /* filling in the data */
        while(qpath.next())
            act.append(qpath.record().value(0).toString());
        child->setData(0, Qt::UserRole, act);

        /* and recurring down if so needed */
        if(levels.length() >= 1) {
            libtree::recurtree(child, levels, conditions + " AND " + sroot->join(" AND "), db);
            child->setText(0, child->text(0) + " (" + QString::number(child->childCount()) + ")");
        }

        parent->addChild(child);
    }
}

QStringList* libtree::extract(QString vars) {
    QStringList* out = new QStringList();
    QRegularExpressionMatchIterator reg = QRegularExpression("#(\\w+)#").globalMatch(vars);
    while(reg.hasNext())
        out->append(reg.next().captured(1));

    return out;
}

void libtree::transc_append(bool discard) {
    QStringList* sl = new QStringList(this->tree->selectedItems().first()->data(0, Qt::UserRole).toStringList());
    emit this->transc_dispatch(sl, discard);
    sl->~QStringList();
}

void libtree::transc_replace() {
    this->transc_append(true);
}

void libtree::media_dispatch(QTreeWidgetItem* item, int column) {
    emit this->win->player_stop();
    emit this->win->player_set(item->data(column, Qt::UserRole).toString());
    emit this->win->player_play();
}

void libtree::showctx(const QPoint& pos) {
    if(this->wb->islocked())
        this->ctx->exec(this->mapToGlobal(pos));
    else {
        this->wb->setlastctx(this);
        this->wb->ctx_req(this->mapTo(this->wb, pos));
    }
}

void libtree::populate(QSqlDatabase* db) {
    this->tree->clear();

    QStringList order = QString("#artist#/[#year#] #album#/#track#. #title#").split("/");
    QString root = order.first();

    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT DISTINCT " + libtree::extract(order.first())->join(", ") + " FROM songs ORDER BY "
                   + libtree::extract(order.first())->join(", "));
    query->exec();
    // query->next();

    order.removeFirst();
    QList<QTreeWidgetItem*> items;

    while(query->next()) {

        QStringList* caseq = libtree::extract(root);
        QString name = root;
        for(int i = 0; i < caseq->length(); i++) {
            name.replace(caseq->at(i), query->record().value(i).toString());
            caseq->replace(i, caseq->at(i) + "=\"" + query->record().value(i).toString() + "\"");
        }
        name.remove("#");

        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(name)));
        QSqlQuery sidequery;
        sidequery.prepare("SELECT DISTINCT path FROM songs WHERE " + caseq->join(" AND ") + " ORDER BY track");
        sidequery.exec();
        // sidequery.next();

        QStringList act;
        while(sidequery.next())
            act.append(sidequery.value(0).toString());

        /*(sidequery.record().value(z).isValid()){//////////////////////////
            qDebug() << sidequery.record().value(z).toString();
            act.append(sidequery.record().value(z).toString());
            z++;
            sidequery.next();
        }*/

        items.last()->setData(0, Qt::UserRole, act);
        libtree::recurtree(items.last(), order, caseq->join(" AND "), db);
        caseq->~QStringList();
        items.last()->setText(0, items.last()->text(0) + " (" + QString::number(items.last()->childCount()) + ")");
    }
    this->tree->insertTopLevelItem(0, new QTreeWidgetItem((QTreeWidget*)0, QStringList("")));
    this->tree->topLevelItem(0)->addChildren(items);
    this->tree->topLevelItem(0)->setText(0, "All Media (" + QString::number(this->tree->topLevelItem(0)->childCount()) + ")");
    this->tree->expandItem(this->tree->topLevelItem(0));
}

void libtree::gatherselected() {
}

libtree::~libtree() {
}

/*
 * The MIT License (MIT)
 * Copyright (c) 2018 Matheus Scattolin Anselmo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or implied,
 * including but not limited to the warranties of merchantability, fitness for a
 * particular purpose and noninfringement. In no event shall the authors or copyright
 * holders be liable for any claim, damages or other liability, whether in an action
 * of contract, tort or otherwise, arising from, out of or in connection with the
 * software or the use or other dealings in the Software.
 */

#include "playlistview.hpp"
#include "../mwindow.hpp"
#include "playlist.hpp"
#include "tabbutton.hpp"

#include <QDialogButtonBox>
#include <QFocusEvent>
#include <QGridLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLineEdit>
#include <QMap>
#include <QMenu>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QTabBar>
#include <QToolButton>
#include <taglib/tag.h>
#include <taglib/tlist.h>
#include <taglib/tpropertymap.h>

playlistview::playlistview(vars* jag, mwindow* win, workbench* wb) : QTabWidget() {
	// passthru pointers
    this->jag = jag;
    this->win = win;
    this->wb = wb;

    this->setContentsMargins(0, 0, 0, 0);
	this->setTabsClosable(true);
	//this->setStyleSheet("QTabBar::tab { width: 10px }");
	this->setObjectName("playlistview");
	this->setMovable(true);
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	// renamer dialog
	this->renamer = new QDialog(this);
	this->namebox = new QLineEdit();
	QGridLayout* grid = new QGridLayout();
	QDialogButtonBox* bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);

	this->renamer->setLayout(grid);
	this->renamer->setFixedSize(480, 48);
	this->renamer->setModal(true);
	this->namebox->setPlaceholderText("Enter a new name here");
    grid->addWidget(namebox, 0, 0, 1, 1);
    grid->addWidget(bb, 0, 1, 1, 1);

    connect(bb, &QDialogButtonBox::accepted, this, &playlistview::renamer_ok);
    connect(bb, &QDialogButtonBox::rejected, this, &playlistview::renamer_cancel);

	// fetches order from database
    this->order.clear();
    this->order.append(jag->fetchdbdata("playlist_columnorder").toString());

	// the tab context menu
    this->ctx = new QMenu();

    ctx->addAction("Rename playlist...");
    ctx->addSeparator();
    ctx->addAction(new QAction(QString("Clear playlist")));
    connect(ctx->actions().last(), &QAction::triggered, this, &playlistview::clearpl);
    ctx->addAction(new QAction(QString("Clear Selection")));
    connect(ctx->actions().last(), &QAction::triggered, this, &playlistview::clearsel);
    ctx->addAction(new QAction(QString("Export playlist")));
    connect(ctx->actions().last(), &QAction::triggered, this, &playlistview::exportpl);

	// then the header's
	this->headermenu = new QMenu();
	QList<QAction*> hact;

	hact.append(new QAction("#"));
	hact.last()->setCheckable(true);
    if(this->order.contains("#INDEX#"))
		hact.last()->toggle();
	connect(hact.last(), &QAction::toggled, this, &playlistview::toggler);

    foreach(QString s, this->jag->dumpkeys()) {
		hact.append(new QAction(this->jag->translate_key(s)));
		hact.last()->setCheckable(true);
		if(this->order.contains(s))
			hact.last()->toggle();
		connect(hact.last(), &QAction::toggled, this, &playlistview::toggler);
    }

	headermenu->addActions(hact);

	// insert newtab tab
	this->insertTab(this->count(), new QWidget, QIcon::fromTheme("list-add"), "");
	this->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0,0);
	this->tabBar()->tabButton(0, QTabBar::RightSide)->hide();

	// insert default empty playlist
	this->newplaylist();
	this->plactive = pl.first();
	this->setCurrentIndex(0);

	//this->tabBar()->setExpanding(false);
	//this->tabBar()->setShape(QTabBar::Shape::TriangularNorth);
	//QIcon(":/ui/cog").pixmap(QSize(256,256)).da

    /* set up new tab button*/
	//this->addbtn = new QToolButton();
	//this->addbtn->setText("+");
	//this->addbtn->setFixedSize(28, 28);
	//connect(this->addbtn, &QToolButton::clicked, this, &playlistview::newplaylist);
	//this->setCornerWidget(this->addbtn, Qt::Corner::TopRightCorner);

    connect(this, &QTabWidget::tabCloseRequested, this, &playlistview::tab_close);
    connect(this, &QTabWidget::customContextMenuRequested, this, &playlistview::showctx);
    connect(this, &QTabWidget::currentChanged, this, &playlistview::tab_switch);
    connect(win, &mwindow::playlist_append, this, &playlistview::playlist_append);
    connect(win, &mwindow::playlist_replace, this, &playlistview::playlist_replace);
    connect(win, &mwindow::playlist_next, this, &playlistview::next);
    connect(win, &mwindow::playlist_prev, this, &playlistview::prev);
    connect(win, &mwindow::mediastatus, this, &playlistview::medistatus);
}

void playlistview::showctx(const QPoint& pos) {
    if(this->wb->islocked())
        this->ctx->exec(this->mapToGlobal(pos));
    else {
        if(this->tabBar()->tabAt(pos) == -1) /* case there isnt a tab there */
            return;
        this->wb->setlastctx(this);
        this->wb->ctx_req(this->mapTo(this->wb, pos));
    }
}

void playlistview::newplaylist() {
    int i = 1;
    while(this->pl.keys().contains("New Playlist " + QString::number(i))) {
        i++;
    }
    QString name = "New Playlist " + QString::number(i);

	tabbutton* ren = new tabbutton(QIcon::fromTheme("edit-rename"), name);

    ren->setMaximumSize(18, 18);
	ren->setFlat(true);
    ren->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

	connect(ren, &tabbutton::idclicked, this, &playlistview::tabbutton_rename);

    pl.insert(name, new playlist(&this->order, this->headermenu, jag, win, this));
    connect(this, &playlistview::exportpl, pl.value(name), &playlist::exportpl);
    connect(this, &playlistview::clearsel, pl.value(name), &playlist::clearSelection);
    connect(this, &playlistview::clearpl, pl.value(name), &playlist::clearchildren);

	this->insertTab(this->count()-1, pl.value(name), name);
	this->tabBar()->setTabButton(this->count()-2, QTabBar::ButtonPosition::LeftSide, ren);
}


void playlistview::context(QPoint p) {
    p = QPoint(); /* erase me¨ */
    // QModelIndex index = this->indexAt(p);
    // if (index.isValid() && index.row() % 2 == 0) {
    // hctx->popup(this->viewport()->mapToGlobal(p));
    //}
}

void playlistview::playing(QString f) {
    f = "0"; /* erase this one¨ */
    /*foreach(QStringList *sl, this->pl->values()){
        if(sl->contains(f)){
            QTreeWidgetItem *x = this->c
            qobject_cast<QTreeWidget*>(this->children().at(sl->indexOf(f)))->setData(0, Qt::UserRole, this->playchar);//->

        }
    }*/
}

void playlistview::refreshpl() {
    foreach(playlist* p, this->pl) { p->rebuild_columns(); }
}

void playlistview::next() {
    this->plactive->next();
}

void playlistview::prev() {
    this->plactive->prev();
}

void playlistview::medistatus(int status) {
    // status = QMediaPlayer::MediaStatus::EndOfMedia; /* erase me¨ */
    // qDebug() << status;
}

void playlistview::swapitem(QTreeWidgetItem* item) {
    /* undo stuff */
    // qDebug() << "lel";
    this->currentitem = item;
    /* redo stuff */
}

void playlistview::toggler(bool checked) {
    this->order.clear();
    // this->order.append(this->playchar+QString(";"));

    foreach(QAction* a, this->headermenu->actions())
        if(a->isChecked())
            this->order.append(this->jag->translate_val(a->text()) + ";");
    qDebug() << this->order;

    this->order.chop(1);
    this->refreshpl();

    if(checked)
        return; /* erase me¨ */
}

void playlistview::tab_close(int index) {
	if(this->count() <= 2)
        return;
    if(index == 0)
		this->plactive = this->pl.value(this->tabText(index).remove("&"));
    else
		this->plactive = this->pl.value(this->tabText(index-1).remove("&"));

	this->setCurrentIndex(this->count() - 3); // minus offset, newtab tab and current to-be-removed tab
    this->pl.remove(this->tabText(index).remove("&"));
    this->removeTab(index);

}

void playlistview::tab_switch(int index) {
	if(index==this->count()-1){
		this->newplaylist();
		this->setCurrentIndex(this->count() - 2);
	}
	this->plactive = this->pl.value(this->tabText(index).remove("&"));
}

void playlistview::tabbutton_rename(QString key) {
    this->renamer->setWindowTitle("Renaming '" + key + "'");
    this->renamer->show();
    tmpkey = key;
}

void playlistview::renamer_ok() {
    this->pl.insert(this->namebox->text(), this->pl.value(this->tmpkey));
    this->pl.remove(tmpkey);

    /* just awful */
    for(int i = 0; i < this->tabBar()->count(); i++)
        if(this->tabBar()->tabText(i).remove("&") == tmpkey) {
            this->tabBar()->setTabText(i, this->namebox->text());
			qobject_cast<tabbutton*>(this->tabBar()->tabButton(i, QTabBar::ButtonPosition::LeftSide))
            ->set_key(this->namebox->text());
        }

    tmpkey.clear();
    this->renamer->close();
    this->namebox->clear();
}

void playlistview::renamer_cancel() {
    this->renamer->close();
    this->namebox->clear();
}

playlistview::~playlistview() {
}

void playlistview::playlist_append(const QStringList files, const int playlist){
    if(playlist == -1)
        this->plactive->append(files);
}

void playlistview::playlist_replace(const QStringList files, const int playlist){
    if(playlist == -1)
        this->plactive->replace(files);
}

// tab bar override
//void tabbar::paintEvent(QPaintEvent* event){
	//QStylePainter painter(this);
	//event->accept();

	/*QStyleOptionTab option;
	initStyleOption(&option, 0);
	//printf("tab text: %s\n", option.text.toLatin1().data());
	//painter.drawControl(QStyle::CE_TabBarTab, option);
	painter.drawItemPixmap(option.rect, 0, QIcon::fromTheme("list-add").pixmap(42,42));
	//painter.drawItemText(option.rect, 0, palette(), 1, option.text);*/
//}

﻿/*
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

#include "src/gui/cwidgets/libtree.hpp"
#include "src/gui/mwindow.hpp"
#include "src/vars.hpp"

libtree::libtree (mwindow* win, workbench* wb, vars* jag) : QFrame() {
	this->win = win;
	this->wb = wb;
	this->ctx = new QMenu();
	this->tree = new QTreeWidget (this);
	this->tagedit = new tageditor (jag, this);
	this->jag = jag;
	this->refresh_config();

	this->setObjectName ("libtree");
	// this->setMinimumWidth(250);
	this->setFrameShape (QFrame::StyledPanel);
	this->setFrameShadow (QFrame::Raised);

	this->init();
}

void libtree::init() {
	QGridLayout* grid = new QGridLayout (this);
	QLineEdit* filterbox = new QLineEdit (this);
	QPushButton* help = new QPushButton ("?", this);
	QLabel* label = new QLabel ("Filter:", this);

	this->tree->setContextMenuPolicy (Qt::CustomContextMenu);
	this->tree->setHeaderHidden (true);

	// the context menu
	const QIcon c = QIcon::fromTheme ("gtk-convert");
	const QIcon t = QIcon::fromTheme ("tag-symbolic");
	QAction* tagger = new QAction (QIcon::fromTheme ("amarok_playlist"), "Mass Tagger");
	QMenu* conv = new QMenu (this->ctx);
	conv->setIcon (c);
	conv->setTitle ("Transcoder");

	QAction* append = new QAction (QIcon::fromTheme ("document-new-from-template"), "Enqueue");
	QAction* rep = new QAction (QIcon::fromTheme ("document-new"), "Add to New Queue");

	conv->addAction (append);
	conv->addAction (rep);
	conv->addSection ("Quick Presets");
	conv->addAction (QIcon::fromTheme ("quickopen"), "FakeFLAC");
	conv->addAction (QIcon::fromTheme ("quickopen"), "FakeMP3");
	QAction* prop = new QAction (t, "Edit Tag");

	connect (prop, &QAction::triggered, this, &libtree::tageditor_spawn);
	connect (append, &QAction::triggered, this, &libtree::transc_append);
	connect (rep, &QAction::triggered, this, &libtree::transc_replace);
	connect (this, &libtree::transc_dispatch, win, &mwindow::transcoder_spawn);
	connect (this, &libtree::dispatch, win, &mwindow::tageditor_spawn);
	connect (this, &libtree::playlist_set, win, &mwindow::playlist_enqueue);
	ctx->addMenu (conv);
	ctx->addAction (tagger);
	ctx->addSeparator();
	ctx->addAction (prop);

	// the switches under the tree
	help->setMaximumWidth (20);
	label->setAlignment (Qt::AlignTrailing | Qt::AlignVCenter);
	label->setFixedWidth (50);
	filterbox->setPlaceholderText ("Type anything to begin");

	QComboBox* scheme = new QComboBox;
	QLabel* treelabel = new QLabel ("Layout:");
	treelabel->setAlignment (Qt::AlignTrailing | Qt::AlignVCenter);
	treelabel->setFixedWidth (50);

	QSqlQuery q (*this->jag->DB_REF);  // user saved schemes
	q.exec ("SELECT * FROM treeschemes");
	// int active = this->jag->fetchdbdata("libtree_activescheme").toInt();

	while (q.next()) { scheme->insertItem (scheme->count() + 1, q.value (0).toString()); }

	// the main layout
	this->setLayout (grid);
	grid->setContentsMargins (1, 1, 1, 1);
	grid->setSpacing (1);

	grid->addWidget (tree, 0, 0, 1, 5);
	grid->addWidget (label, 1, 0, 1, 1);
	grid->addWidget (filterbox, 1, 1, 1, 3);
	grid->addWidget (help, 1, 4, 1, 1);
	grid->addWidget (treelabel, 2, 0, 1, 1);
	grid->addWidget (scheme, 2, 1, 1, 4);

	connect (tree, &QTreeWidget::customContextMenuRequested, this, &libtree::showctx);
	connect (tree, &QTreeWidget::itemDoubleClicked, this, &libtree::doubleclick);
	connect (tree, &QTreeWidget::itemClicked, win, &mwindow::select);
	connect (win, &mwindow::libtree_refreshconfig, this, &libtree::refresh_config);
	connect (win, &mwindow::libchanged, this, &libtree::populate);
}

/* this fills each node with the corresponding path and name */
void libtree::recurtree (QTreeWidgetItem* parent, QStringList levels, QString conditions, QSqlDatabase* db) {
	QString root = levels.first();
	levels.removeFirst();
	QStringList sroot = libtree::extract (root);
	QString vars = sroot.join (", ");

	QSqlQuery rq;
	rq.prepare ("SELECT DISTINCT " + vars + " FROM songs WHERE " + conditions + " ORDER BY " + sroot.first());
	rq.exec();
	while (rq.next()) { /* query again to fetch paths */
		/* starting by generating condition string */
		QString name = root;
		sroot = libtree::extract (root);
		for (int i = sroot.length() - 1; i >= 0; i--) {
			name.replace (sroot.at (i), rq.record().value (i).toString());
			sroot.replace (i, sroot.at (i) + "=\"" + rq.record().value (i).toString() + "\"");
		}
		name.remove ("#");

		/* then running the query itself */
		QStringList act;
		act.clear();
		QTreeWidgetItem* child = new QTreeWidgetItem (parent, QStringList (name));
		QSqlQuery qpath;
		qpath.exec ("SELECT DISTINCT path FROM songs WHERE " + sroot.join (" AND ") + " AND " + conditions +
					" ORDER BY track");

		/* filling in the data */
		while (qpath.next()) act.append (qpath.record().value (0).toString());
		child->setData (0, Qt::UserRole, act);

		/* and recurring down if needed */
		if (levels.length() >= 1) {
			libtree::recurtree (child, levels, conditions + " AND " + sroot.join (" AND "), db);
			child->setText (0, child->text (0) + " (" + QString::number (child->childCount()) + ")");
		}

		parent->addChild (child);
	}
}

QStringList libtree::extract (QString vars) {
	QStringList out;
	QRegularExpressionMatchIterator reg = QRegularExpression ("#(\\w+)#").globalMatch (vars);
	while (reg.hasNext()) out.append (reg.next().captured (1));

	return out;
}

void libtree::tageditor_spawn() {
	QStringList files;
	QTreeWidgetItem* item = this->tree->selectedItems().first();
	this->listchildren (item, &files);
	files.removeAll (QString (""));

	this->tagedit->setup (files);
	this->tagedit->show();
}

void libtree::listchildren (QTreeWidgetItem* item, QStringList* children) {
	if (item->childCount() == 0)
		*children << item->data (0, Qt::UserRole).toString();
	for (int i = 0; i < item->childCount(); i++)
		if (item->child (i)->childCount() > 0)
			listchildren (item->child (i), children);
		else
			*children << item->child (i)->data (0, Qt::UserRole).toString();
}

void libtree::transc_append (bool discard) {
	QStringList* sl = new QStringList (this->tree->selectedItems().first()->data (0, Qt::UserRole).toStringList());
	emit this->transc_dispatch (sl, discard);
	sl->~QStringList();
}

void libtree::transc_replace() {
	this->transc_append (true);
}

void libtree::refresh_config() {
	this->dclick = CLICKBEHAVIOUR (this->jag->fetchdbdata ("libtree_doubleclick").toInt());
	this->mclick = CLICKBEHAVIOUR (this->jag->fetchdbdata ("libtree_middleclick").toInt());
}

void libtree::doubleclick (QTreeWidgetItem* item) {
	QStringList files;
	this->listchildren (item, &files);
	files.removeAll (QString (""));
	switch (this->dclick) {
	case CLICKBEHAVIOUR::REPLACE: emit this->win->playlist_replace (files); break;

	case CLICKBEHAVIOUR::REPLACE_AND_PLAY:
		emit this->win->player_stop();	// TODO GAPLESS
		emit this->win->player_load (files.front());
		emit this->win->player_play();
		emit this->win->playlist_replace (files);
		break;

	case CLICKBEHAVIOUR::APPEND_CUR: emit this->win->playlist_enqueue (files); break;

	case CLICKBEHAVIOUR::APPEND_AND_PLAY:
		emit this->win->player_stop();	// TODO GAPLESS
		emit this->win->player_load (files.front());
		emit this->win->player_play();
		emit this->win->playlist_enqueue (files);
		break;

	case CLICKBEHAVIOUR::APPEND_OTHER: break;

	case CLICKBEHAVIOUR::SEND_TRANSC: break;

	case CLICKBEHAVIOUR::EDIT_TAGS: break;
	}
}

void libtree::middleclick (QTreeWidgetItem* item) {
	QStringList files;
	this->listchildren (item, &files);
	files.removeAll (QString (""));

	switch (this->mclick) {
	case CLICKBEHAVIOUR::REPLACE: emit this->win->playlist_replace (files); break;

	case CLICKBEHAVIOUR::REPLACE_AND_PLAY:
		emit this->win->player_stop();	// TODO GAPLESS
		emit this->win->player_load (files.front().toUtf8());
		emit this->win->player_play();
		emit this->win->playlist_replace (files);
		break;

	case CLICKBEHAVIOUR::APPEND_CUR: emit this->win->playlist_enqueue (files); break;

	case CLICKBEHAVIOUR::APPEND_AND_PLAY:
		emit this->win->player_stop();	// TODO GAPLESS
		emit this->win->player_load (files.front().toUtf8());
		emit this->win->player_play();
		emit this->win->playlist_enqueue (files);
		break;

	case CLICKBEHAVIOUR::APPEND_OTHER: break;

	case CLICKBEHAVIOUR::SEND_TRANSC: break;

	case CLICKBEHAVIOUR::EDIT_TAGS: break;
	}
}

void libtree::showctx (const QPoint& pos) {
	if (this->wb->islocked())
		this->ctx->exec (this->mapToGlobal (pos));
	else {
		this->wb->setlastctx (this);
		this->wb->ctx_req (this->mapTo (this->wb, pos));
	}
}

void libtree::populate (QSqlDatabase* db) {
	this->tree->clear();

	QStringList order = QString ("#artist#/[#year#] #album#/#track#. #title#").split ("/");
	QString root = order.first();
	QString qtext ("SELECT DISTINCT :f FROM songs ORDER BY :f");
	qtext.replace (":f", libtree::extract (order.first()).join (", "));

	QSqlQuery query (*db);
	query.prepare (qtext);
	query.exec();

	order.removeFirst();
	QList<QTreeWidgetItem*> items;

	while (query.next()) {
		QStringList caseq = libtree::extract (root);
		QString name = root;
		for (int i = 0; i < caseq.length(); i++) {
			name.replace (caseq.at (i), query.record().value (i).toString());
			caseq.replace (i, caseq.at (i) + "=\"" + query.record().value (i).toString() + "\"");
		}
		name.remove ("#");

		items.append (new QTreeWidgetItem ((QTreeWidget*)0, QStringList (name)));
		QSqlQuery sidequery;
		sidequery.prepare ("SELECT DISTINCT path FROM songs WHERE " + caseq.join (" AND ") + " ORDER BY track");
		sidequery.exec();
		// sidequery.next();

		QStringList act;
		while (sidequery.next()) act.append (sidequery.value (0).toString());

		/*(sidequery.record().value(z).isValid()){//////////////////////////
			qDebug() << sidequery.record().value(z).toString();
			act.append(sidequery.record().value(z).toString());
			z++;
			sidequery.next();
		}*/

		items.last()->setData (0, Qt::UserRole, act);
		libtree::recurtree (items.last(), order, caseq.join (" AND "), db);
		items.last()->setText (0, items.last()->text (0) + " (" + QString::number (items.last()->childCount()) + ")");
	}
	this->tree->insertTopLevelItem (0, new QTreeWidgetItem ((QTreeWidget*)0, QStringList ("")));
	this->tree->topLevelItem (0)->addChildren (items);
	this->tree->topLevelItem (0)->setText (
		0, "All Media (" + QString::number (this->tree->topLevelItem (0)->childCount()) + ")");
	this->tree->expandItem (this->tree->topLevelItem (0));
}

libtree::~libtree() {}

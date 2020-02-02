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

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSizePolicy>
#include <QSqlRecord>
#include <QTabWidget>
#include <QTableWidget>
#include <QTreeWidget>

#include "settings.hpp"

#include "../../library.hpp"
#include "../../vars.hpp"
#include "../mwindow.hpp"

settings::settings (QWidget* win, vars* jag) : QDialog (win) {
	this->jag = jag;
	this->win = win;
	QVBoxLayout* l = new QVBoxLayout (this);
	QDialogButtonBox* bb = new QDialogButtonBox (QDialogButtonBox::Close);
	QTabWidget* tab = new QTabWidget (this);

	l->addWidget (tab);
	l->addWidget (bb);

	tab->addTab (this->spawn_maintab (win), "General");
	tab->addTab (this->spawn_dirtab (win), "Directories");
	tab->addTab (this->spawn_libtreetab (win), "Tree Library");
	tab->addTab (this->spawn_advanced (win), "Advanced");
	connect (bb, &QDialogButtonBox::accepted, this, &settings::accept);
	connect (bb, &QDialogButtonBox::rejected, this, &settings::reject);

	this->setWindowTitle (QString ("Settings Dialog :: OCELOT v") + jag->VERSION);
	this->setFixedSize (980, 560);
	this->setLayout (l);
}

settings::~settings() {}

QWidget* settings::spawn_maintab (QWidget* win) {
	QWidget* w = new QWidget (win);
	QGridLayout* grid = new QGridLayout();

	/* UI */
	QGroupBox* uibox = new QGroupBox ("UI and Colors");
	QFormLayout* uil = new QFormLayout();
	uil->setFormAlignment (Qt::AlignHCenter);

	uibox->setLayout (uil);

	QCheckBox* ui_remsize = new QCheckBox();
	uil->addRow ("Remember window size for next start", ui_remsize);

	QCheckBox* ui_rempos = new QCheckBox();
	uil->addRow ("Remember window position for next start", ui_rempos);

	/* and the last */
	QGroupBox* kbbox = new QGroupBox ("Keyboard Shortcuts");
	kbbox->setLayout (new QGridLayout);
	QTableWidget* sc = new QTableWidget (15, 2);
	sc->setHorizontalHeaderLabels (QStringList() << "Action"
												 << "Shortcut");
	sc->setColumnWidth (0, 130);
	sc->setColumnWidth (1, 218);
	sc->setSelectionMode (QAbstractItemView::NoSelection);
	sc->setFixedWidth (400);
	kbbox->layout()->addWidget (sc);
	kbbox->setMaximumWidth (420);

	/* entangle everything */
	grid->addWidget (uibox, 0, 1, 2, 1);
	grid->addWidget (kbbox, 1, 0, 1, 1);
	w->setLayout (grid);

	return w;
}

QWidget* settings::spawn_libtreetab (QWidget* win) {
	QWidget* w = new QWidget (win);
	QGridLayout* grid = new QGridLayout();

	QGroupBox* mgbox = new QGroupBox ("Click Behaviour");
	mgbox->setFixedWidth (440);
	QFormLayout* mform = new QFormLayout();
	mform->setFieldGrowthPolicy (QFormLayout::AllNonFixedFieldsGrow);

	QStringList opts = QStringList() << "Replace current playlist"
									 << "Replace current playlist and play"
									 << "Append to current playlist"
									 << "Append to current playlist and play"
									 << "Append to another playlist"
									 << "Send to transcoder queue"
									 << "Edit tags";

	QComboBox* dclick = new QComboBox();
	dclick->insertItems (0, opts);
	mform->addRow (new QLabel ("Double-click:", dclick), dclick);

	QComboBox* mclick = new QComboBox();
	mclick->insertItems (0, opts);
	mform->addRow (new QLabel ("Middle-click:", mclick), mclick);

	/* set selection from db */
	dclick->setCurrentIndex (this->jag->fetchdbdata ("libtree_doubleclick").toInt());
	mclick->setCurrentIndex (this->jag->fetchdbdata ("libtree_middleclick").toInt());

	connect (dclick, QOverload<int>::of (&QComboBox::currentIndexChanged), this, &settings::libtree_dclick);
	connect (mclick, QOverload<int>::of (&QComboBox::currentIndexChanged), this, &settings::libtree_mclick);
	mgbox->setLayout (mform);

	/* sorting scheme */
	QGroupBox* sbox = new QGroupBox ("Sorting Scheme");
	sbox->setLayout (new QGridLayout);
	sbox->setMinimumWidth (420);  // nice

	QFrame* control = new QFrame();
	control->setLayout (new QGridLayout);
	control->setFrameStyle (QFrame::StyledPanel);
	control->setFrameShadow (QFrame::Shadow::Sunken);
	control->setContentsMargins (1, 1, 1, 1);
	control->setAutoFillBackground (true);

	QLineEdit* nameline = new QLineEdit();
	nameline->setPlaceholderText ("Give it a name");
	QLineEdit* schemeline = new QLineEdit();
	schemeline->setPlaceholderText ("Write your scheme here");
	QPushButton* ins = new QPushButton();
	ins->setText ("INSERT...");
	QPushButton* help = new QPushButton();
	help->setText ("HELP");
	QPushButton* done = new QPushButton();
	done->setText ("OK");

	qobject_cast<QGridLayout*> (control->layout())->addWidget (nameline, 0, 0, 1, 1);
	qobject_cast<QGridLayout*> (control->layout())->addWidget (done, 0, 1, 1, 1);
	qobject_cast<QGridLayout*> (control->layout())->addWidget (help, 0, 2, 1, 1);
	qobject_cast<QGridLayout*> (control->layout())->addWidget (schemeline, 1, 0, 1, 2);
	qobject_cast<QGridLayout*> (control->layout())->addWidget (ins, 1, 2, 1, 1);

	qobject_cast<QGridLayout*> (sbox->layout())->addWidget (control, 0, 0, 1, 2);

	/* the middle separator */
	QFrame* nsep = new QFrame();
	nsep->setFrameStyle (QFrame::HLine);
	// qobject_cast<QGridLayout*>(sbox->layout())->addWidget(nsep,1,0,1,2);

	/* and the view */
	QTreeWidget* stree = new QTreeWidget();
	stree->setColumnCount (2);
	stree->setHeaderLabels (QStringList() << "Name"
										  << "Scheme");
	stree->setColumnWidth (0, 128);
	stree->setAlternatingRowColors (true);

	QSqlQuery q (*this->jag->DB_REF);
	q.exec ("SELECT * FROM treeschemes");
	while (q.next()) {
		stree->addTopLevelItem (
			new QTreeWidgetItem (stree, QStringList() << q.value (0).toString() << q.value (1).toString()));
	}
	stree->topLevelItem (stree->topLevelItemCount() - 1)->setSelected (true);

	QPushButton* edit = new QPushButton ("Edit");
	QPushButton* erase = new QPushButton ("Erase");

	if (stree->topLevelItemCount() == 1)
		erase->setEnabled (false);

	qobject_cast<QGridLayout*> (sbox->layout())->addWidget (stree, 2, 0, 1, 2);
	qobject_cast<QGridLayout*> (sbox->layout())->addWidget (edit, 3, 0, 1, 1);
	qobject_cast<QGridLayout*> (sbox->layout())->addWidget (erase, 3, 1, 1, 1);

	grid->addWidget (mgbox, 0, 0, 1, 1);
	grid->addWidget (sbox, 0, 1, 1, 1);

	w->setLayout (grid);

	return w;
}

QWidget* settings::spawn_dirtab (QWidget* win) {
	QWidget* w = new QWidget (win);
	QGridLayout* l = new QGridLayout;
	QTreeWidget* tree = new QTreeWidget();
	tree->setColumnCount (3);
	tree->setHeaderLabels (QStringList() << "Path"
										 << "Track#"
										 << "Total Filesize");

	libstree = tree;
	lib_treerefresh();
	tree->header()->setStretchLastSection (false);
	tree->header()->setSectionResizeMode (QHeaderView::ResizeToContents);
	tree->header()->setSectionResizeMode (0, QHeaderView::Stretch);

	QPushButton* fpicker = new QPushButton();
	fpicker->setText ("Add new...");
	fpicker->setMaximumWidth (160);
	connect (fpicker, &QPushButton::clicked, this, &settings::lib_dirpicker);

	QPushButton* rembtn = new QPushButton();
	rembtn->setText ("Remove selected");
	rembtn->setMaximumWidth (160);
	connect (rembtn, &QPushButton::clicked, this, &settings::lib_remdir);

	// qobject_cast<QGridLayout*>(tframe->layout())->addWidget(tree, 0, 0, 1, 3);
	// qobject_cast<QGridLayout*>(tframe->layout())->addWidget(fpicker, 1, 0, 1, 1);
	// qobject_cast<QGridLayout*>(tframe->layout())->addWidget(rembtn, 1, 1, 1, 1);

	l->addWidget (tree, 0, 0, 1, 4);
	l->addWidget (fpicker, 4, 0, 1, 1);
	l->addWidget (rembtn, 4, 1, 1, 1);

	l->setHorizontalSpacing (2);
	l->setVerticalSpacing (4);

	w->setLayout (l);

	return w;
}

QWidget* settings::spawn_advanced (QWidget* win) {
	QWidget* adv = new QWidget (win);

	QHBoxLayout* l = new QHBoxLayout;
	adv->setLayout (l);

	QTableWidget* vars = new QTableWidget (win);
	vars->setColumnCount (1);
	vars->horizontalHeader()->setSectionResizeMode (QHeaderView::Stretch);
	vars->setColumnWidth (0, 360);
	vars->setHorizontalHeaderLabels (QStringList() << "Value");
	vars->setFixedWidth (560);
	vars->setSelectionMode (QAbstractItemView::SelectionMode::SingleSelection);
	// vars->setRowCount(30);
	QSqlQuery q = this->jag->DB_REF->exec ("SELECT var, val FROM data");
	QStringList var;
	while (q.next()) {
		var << q.value (0).toString();
		vars->insertRow (vars->rowCount());
		vars->setItem (var.length() - 1, 0, new QTableWidgetItem (q.value (1).toString()));
	}
	vars->setVerticalHeaderLabels (var);
	l->addWidget (vars);

	QFrame* clicker = new QFrame (win);
	// clicker->setContentsMargins(0,0,0,0);
	clicker->setFrameStyle (QFrame::StyledPanel);
	clicker->setFrameShadow (QFrame::Shadow::Sunken);
	QFormLayout* f = new QFormLayout;
	clicker->setAutoFillBackground (true);
	clicker->setLayout (f);
	QComboBox* a = new QComboBox;
	a->sizePolicy().setHorizontalPolicy (QSizePolicy::Expanding);
	f->addWidget (a);
	l->addWidget (clicker);

	return adv;
}

void settings::lib_dirpicker() {
	QString dir =
		QFileDialog::getExistingDirectory (this, "Add new library", QDir::homePath(), QFileDialog::ShowDirsOnly);
	if (dir.isEmpty())
		return;

	qInfo() << "[INFO] Adding a new library through the settings dialog...";
	QThread* t = new QThread();
	library* l = new library (&dir, this->jag->DB_REF, this->jag->dumppaths());
	l->moveToThread (t);

	connect (l, &library::added, this, &settings::thread_newlib);
	connect (t, &QThread::started, l, &library::process);
	connect (l, &library::finished, t, &QThread::quit);
	connect (l, &library::finished, l, &library::deleteLater);
	connect (t, &QThread::finished, t, &QThread::deleteLater);
	connect (t, &QThread::finished, this, &settings::thread_libscan);

	t->start();

	this->libstree->addTopLevelItem (new QTreeWidgetItem (QStringList() << "Working..."));
}

void settings::lib_treerefresh() {
	this->libstree->clear();
	QStringList libs = this->jag->dumplibinfo();

	foreach (QString s, libs) {
		QStringList l = s.split (";");
		l.last().append (" MiB");
		this->libstree->addTopLevelItem (new QTreeWidgetItem (libstree, l));
	}

	qobject_cast<mwindow*> (this->win)->libchanged (this->jag->DB_REF);
}

void settings::lib_remdir() {
	if (this->jag->libs->length() == 0)
		return;

	foreach (QTreeWidgetItem* s, this->libstree->selectedItems()) {
		this->jag->remlibs (s->data (0, Qt::EditRole).toString());
	}

	lib_treerefresh();
}

/* combobox about double click behaviour */
void settings::libtree_mclick (int index) {
	this->jag->setdbdata ("libtree_middleclick", index);

	emit static_cast<mwindow*> (this->win)->libtree_refreshconfig();
}

/* combobox about middle click behaviour */
void settings::libtree_dclick (int index) {
	this->jag->setdbdata ("libtree_doubleclick", index);

	emit static_cast<mwindow*> (this->win)->libtree_refreshconfig();
}

void settings::thread_libscan() {
	this->lib_treerefresh();
}

void settings::thread_newlib (QString lib) {
	this->jag->libs->append (lib);
	this->lib_treerefresh();
}

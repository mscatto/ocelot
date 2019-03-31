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

#include "transcoder.hpp"

#include <QDialog>
#include <QGridLayout>
#include <QSplitter>

#include "src/mwindow.hpp"
#include "dialogs/transchelper.hpp"

transcoder::transcoder(vars *jag, QWidget *win) : QDialog(win){
    this->input = new QStringList();
    QGridLayout *l = new QGridLayout(this);
    QSplitter *s = new QSplitter(this);
    this->prog = new QProgressBar();
    this->prog->setRange(0,100);
    this->prog->setValue(0);
    this->helper = new transchelper(this, jag);
    //*// PRESETS DIALOG
        QDialog *pdiag = new QDialog();
        QWidget *pdwid = new QWidget();
        QGridLayout *pdlay = new QGridLayout;
        pdiag->setLayout(pdlay);
        pdiag->setFixedSize(300,600);
        pdlay->addWidget(pdwid);
    //*//

    s->setOrientation(Qt::Vertical);
    s->setChildrenCollapsible(false);

    this->outlist = new QTreeWidget();
    this->outlist->setColumnCount(2);
    this->outlist->setHeaderLabels(QStringList()<<"Output"<<"Format");
    this->outlist->setDragEnabled(false);
    this->outlist->header()->setStretchLastSection(false);
    this->outlist->header()->setSectionResizeMode(0,QHeaderView::ResizeMode::Stretch);
    this->outlist->header()->setSectionResizeMode(1,QHeaderView::ResizeMode::ResizeToContents);

    this->inlist = new QTreeWidget();
    this->inlist->setColumnCount(2);
    this->inlist->setHeaderLabels(QStringList()<<"Input files"<<"Format");
    this->inlist->setDragEnabled(true);
    this->inlist->header()->setStretchLastSection(false);
    this->inlist->header()->setSectionResizeMode(0,QHeaderView::ResizeMode::Stretch);
    this->inlist->header()->setSectionResizeMode(1,QHeaderView::ResizeMode::ResizeToContents);

    /*inlist->insertColumn(2);
    inlist->setVerticalHeaderLabels(QStringList()<<"#"<<"FILENAME"<<"SIZE"<<"PATH");
    inlist->setRowCount(20);*/
    //inlist->setDragDropMode(QTableWidget::DragDropMode::InternalMove);
    s->addWidget(this->inlist);
    s->addWidget(this->outlist);

    QWidget *bottom = new QWidget(this);
    QPushButton *go = new QPushButton("Start");
    QPushButton *clr = new QPushButton("Clear queue");

    QGridLayout *g = new QGridLayout;
    g->setContentsMargins(0,0,0,0);
    g->addWidget(this->prog,0,0,1,2);
    g->addWidget(clr,0,2,1,1);
    g->addWidget(go,0,3,1,1);
    bottom->setLayout(g);

    QWidget *panel = new QWidget;
    QVBoxLayout *hbl = new QVBoxLayout;
    hbl->setContentsMargins(0,0,0,0);
    panel->setLayout(hbl);

    QGroupBox *pres = new QGroupBox("Presets");
    QGridLayout *pgl = new QGridLayout;
    pres->setLayout(pgl);

    pres->sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
    this->preset_gbx = new QGroupBox();
    this->preset_cbx = new QComboBox();
    this->preset_cbx->addItems(QStringList()<<"MyPreset"<<"Cebolitos");
    this->preset_add = new QPushButton("Add preset");
    this->preset_rem = new QPushButton("Remove");
    this->preset_adv = new QPushButton(QIcon::fromTheme("go-down-symbolic"),"Custom");
    this->preset_adv->setCheckable(true);
    this->preset_adv->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    this->preset_adv->setFocus(Qt::FocusReason::ActiveWindowFocusReason);
    connect(this->preset_adv, &QPushButton::clicked, this, &transcoder::flip_qpreset);
    connect(this->preset_add, &QPushButton::clicked, this, &transcoder::spawnhelper);
    pgl->addWidget(this->preset_cbx,0,0,1,4);
    pgl->addWidget(this->preset_add,0,4,1,1);
    pgl->addWidget(this->preset_rem,0,5,1,1);
    pgl->addWidget(this->preset_adv,0,6,1,1);
    pgl->addWidget(this->preset_gbx,1,0,1,7);
    hbl->addWidget(pres);

    QFormLayout *gbxf = new QFormLayout; // is this tar or something
    this->preset_gbx->setLayout(gbxf);
    QHBoxLayout *b = new QHBoxLayout;
    QLineEdit *path = new QLineEdit();
    path->setPlaceholderText("Type or select a path");
    b->addWidget(path);
    QPushButton *pck = new QPushButton("...");
    pck->setFixedWidth(30);
    b->addWidget(pck);

    QHBoxLayout *bv = new QHBoxLayout;
    QLineEdit *args = new QLineEdit;
    args->setPlaceholderText("Follow the encoder instructions");
    QPushButton *chl = new QPushButton("Check");
    bv->addWidget(args);
    bv->addWidget(chl);

    gbxf->addRow(new QLabel("Encoder:"),b);
    gbxf->addRow(new QLabel("Arguments:"),bv);
    this->preset_gbx->setEnabled(this->qpreset);
    QGroupBox *output = new QGroupBox("Output");
    output->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    hbl->addWidget(output);

    QGroupBox *tw = new QGroupBox("Tweaks");
    tw->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    hbl->addWidget(tw);


    l->addWidget(s,0,0,1,1);
    l->addWidget(panel,0,1,1,1);
    l->addWidget(bottom,1,0,2,2);

    this->setLayout(l);
    this->setWindowTitle(QString("Transcoder :: OCELOT v")+jag->VERSION);
    this->setMinimumSize(780,580);
    this->setModal(false);

    connect(qobject_cast<mwindow*>(win), &mwindow::convhandler, this, &transcoder::append);
}

void transcoder::clear(){
    this->outlist->clear();
    this->inlist->clear();
    this->input->clear();
}

void transcoder::flip_qpreset(){
    this->qpreset = !this->qpreset;
    this->preset_add->setEnabled(!this->preset_add->isEnabled());
    this->preset_rem->setEnabled(!this->preset_rem->isEnabled());
    this->preset_cbx->setEnabled(!this->preset_cbx->isEnabled());
    this->preset_gbx->setEnabled(!this->preset_gbx->isEnabled());
}

void transcoder::refresh_output(){
    this->outlist->clear();
    QTreeWidgetItem *ni;
    foreach(QString s, *this->input){
        ni = new QTreeWidgetItem(this->outlist);
        ni->setText(0, QFileInfo(s).filePath());
        ni->setText(1, QMimeDatabase().mimeTypeForFile(s).name());
        this->outlist->addTopLevelItem(ni);
    }
}

void transcoder::append(QStringList *files){
    this->input->append(*files);
    QTreeWidgetItem *ni;
    foreach(QString s, *files){
        ni = new QTreeWidgetItem(this->inlist);
        ni->setText(0, QFileInfo(s).fileName());
        ni->setText(1, QMimeDatabase().mimeTypeForFile(s).name());
        this->inlist->addTopLevelItem(ni);
    }

    this->refresh_output();
}

void transcoder::replace(QStringList *files){
    this->input->clear();
    this->append(files);
}

void transcoder::spawnhelper(){
    this->helper->show();
}

transcoder::~transcoder(){

}

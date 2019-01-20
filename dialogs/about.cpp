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

#include "about.hpp"

#include <QDialog>
#include <QGridLayout>
#include <QSplitter>

about::about(QWidget *parent, vars *jag) : QDialog(parent){
    this->jag = jag;
    QGridLayout *l = new QGridLayout(this);
    QSplitter *s = new QSplitter(this);
    l->addWidget(s);

    this->setLayout(l);
    this->setWindowTitle("About OCELOT");
    this->setFixedSize(800,300);
    this->setModal(true);

    QTabWidget *maintabs = new QTabWidget();
    this->layout()->addWidget(maintabs);

    maintabs->addTab(about::mktab_ocelot(), "About Ocelot");
    maintabs->addTab(new QWidget(), "About Qt");
}

about::~about(){

}

QWidget *about::mktab_ocelot(){
    QWidget *o = new QWidget();
    QHBoxLayout *l = new QHBoxLayout();
    o->sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);
    /* info pane */
    QLabel *info = new QLabel(QString("<h1>OCELOT v")+
          this->jag->VERSION+
          "</h1>A music player and library manager written in C++ using the Qt Frameworks<br><br>"+
          "<a href=\"https://github.com/mscatto/ocelot\">https://github.com/mscatto/ocelot</a><br><br>"+
          "2018 - 2019 Matheus Scattolin Anselmo");
    info->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    info->setOpenExternalLinks(true);
    info->setTextInteractionFlags(Qt::TextBrowserInteraction);
    info->setWordWrap(true);
    info->setFixedWidth(300);

    /* libraries pane */
    QWidget *lp_w = new QWidget();
    QFrame *lp_iw = new QFrame();
    lp_iw->setAutoFillBackground(true);
    lp_iw->sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);
    lp_iw->setFrameStyle(QFrame::Sunken);
    lp_iw->setFrameShape(QFrame::StyledPanel);
    lp_iw->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *lp_l = new QVBoxLayout();
    lp_l->setSpacing(0);
    lp_l->setContentsMargins(0,0,0,0);
    QLabel *lp_tlbl = new QLabel("<h4>Ocelot depends on</h4>");
    lp_tlbl->sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
    lp_tlbl->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    QFontMetrics fm(lp_tlbl->fontMetrics());
    lp_tlbl->setFixedHeight(fm.height());
    lp_l->insertWidget(0, lp_tlbl);
    lp_l->insertWidget(1, lp_iw);
    lp_l->setAlignment(Qt::AlignHCenter | Qt::AlignAbsolute);
    //lp_l->set

    QFormLayout *lp_il = new QFormLayout();
    lp_il->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    lp_il->addRow("TagLib", new QLabel("<a href=\"https://taglib.org/\">Website</a>"));
    lp_il->addRow("ProjectM", new QLabel("<a href=\"https://github.com/projectM-visualizer/projectm\">Github Repository</a>"));

    lp_iw->setLayout(lp_il);

    /* contributors pane */
    QTextEdit *contribs = new QTextEdit();
    contribs->setAutoFillBackground(true);
    contribs->setEnabled(false);
    contribs->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    contribs->setText("<center>Can you hear the crickets?");
    contribs->sizePolicy().setVerticalPolicy(QSizePolicy::Maximum);

    QWidget *w = new QWidget();
    QGridLayout *cl = new QGridLayout();
    QLabel *lbl = new QLabel("<h4>Contributor's Hall of Fame</h4>");
    lbl->sizePolicy().setVerticalPolicy(QSizePolicy::Minimum);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    cl->addWidget(lbl,0,0);
    cl->addWidget(contribs,1,0);
    cl->setAlignment(Qt::AlignTop);
    cl->setSpacing(0);
    cl->setContentsMargins(0,0,0,0);

    /* then hook everything thogether */
    lp_w->setLayout(lp_l);
    w->setLayout(cl);


    l->insertWidget(0, info);
    l->insertWidget(1, lp_w);
    l->insertWidget(2, w);

    o->setLayout(l);
    return o;
}

QWidget *about::mktab_qt(){

}


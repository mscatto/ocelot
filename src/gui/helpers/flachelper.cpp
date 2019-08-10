#include "flachelper.hpp"
#include <QFormLayout>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QFileDialog>
#include <QToolTip>
#include <QPoint>

// fancy some pasta?
flachelper::flachelper(QWidget *parent) : QWidget(parent){
    QGridLayout *ml = new QGridLayout;
    this->setLayout(ml);
    ml->setContentsMargins(0,0,0,0);
    QGroupBox *main = new QGroupBox;
    ml->addWidget(main);
    QFormLayout *l = new QFormLayout;
    l->setAlignment(Qt::AlignVCenter);
    l->setLabelAlignment(Qt::AlignVCenter | Qt::AlignTrailing);
    main->setLayout(l);

    QLineEdit *pname = new QLineEdit();
    l->addRow("Preset Name:", pname);
    QVBoxLayout *v = new QVBoxLayout;
    QFrame *sep = new QFrame;
    sep->setFrameShadow(QFrame::Raised);
    //sep->setFrameShape(QFrame::Shape::StyledPanel);
    //sep->setFixedHeight(2);
    sep->setFrameShape(QFrame::HLine);
    v->addWidget(sep);
    l->addRow(v);

    /* level slider */
    /***/
    QSlider *encslider = new QSlider(this);
    encslider->setRange(0,8);
    encslider->setTickInterval(1);
    encslider->setValue(8);
    encslider->setTickPosition(QSlider::TicksBothSides);
    encslider->setOrientation(Qt::Horizontal);
    connect(encslider, &QSlider::valueChanged, this, &flachelper::refresh_enclevel);

    this->enclevel = new QLabel("8");
    this->enclevel->setFixedWidth(16);
    this->enclevel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->enclevel->setFrameShape(QFrame::Shape::StyledPanel);
    this->enclevel->setFrameShadow(QFrame::Shadow::Sunken);

    QHBoxLayout *lvlayout = new QHBoxLayout;
    lvlayout->addWidget(encslider);
    lvlayout->addWidget(this->enclevel);
    l->addRow("Compression Level:", lvlayout);

    /* sampling rate  row */
    /**/
    QCheckBox *keepcheckb = new QCheckBox("Keep");
    keepcheckb->setChecked(true);
    connect(keepcheckb, &QCheckBox::clicked, this, &flachelper::flip_encsrate);

    QComboBox *brcombob = new QComboBox;
    brcombob->addItems(QStringList()<<"44kHz"<<"96kHz");

    QLabel *spr = new QLabel("Override:");
    spr->setAlignment(Qt::AlignTrailing);

    QHBoxLayout *brset = new QHBoxLayout;
    brset->setContentsMargins(0,0,0,0);
    brset->addWidget(spr);
    brset->addWidget(brcombob);

    this->encsrate = new QFrame(this);
    this->encsrate->setEnabled(false);
    this->encsrate->setFrameShape(QFrame::Shape::StyledPanel);
    this->encsrate->setFrameShadow(QFrame::Shadow::Raised);
    this->encsrate->setContentsMargins(0,0,0,0);
    this->encsrate->setLayout(brset);

    QHBoxLayout *spl = new QHBoxLayout;
    spl->setContentsMargins(0,0,0,0);
    spl->addWidget(keepcheckb);
    spl->addWidget(this->encsrate,Qt::AlignRight);
    l->addRow("Sampling Rate:", spl);

    /* bit depth row */
    /**/
    QCheckBox *bdh = new QCheckBox;
    bdh->setText("Keep");
    bdh->setChecked(true);
    connect(bdh, &QCheckBox::clicked, this, &flachelper::flip_encbdepth);

    QComboBox *bdcombob = new QComboBox;
    bdcombob->addItems(QStringList()<<"16bits"<<"24bits");

    QLabel *spd = new QLabel("Override:");
    spd->setAlignment(Qt::AlignTrailing);

    QHBoxLayout *bdset = new QHBoxLayout;
    bdset->setContentsMargins(0,0,0,0);
    //bdset->setSpacing(0);
    bdset->addWidget(spd);
    bdset->addWidget(bdcombob);

    this->encbdepth = new QFrame(this);
    this->encbdepth->setEnabled(false);
    this->encbdepth->setFrameShape(QFrame::Shape::StyledPanel);
    this->encbdepth->setFrameShadow(QFrame::Shadow::Raised);
    this->encbdepth->setContentsMargins(0,0,0,0);
    this->encbdepth->setLayout(bdset);

    QHBoxLayout *bdl = new QHBoxLayout;
    spl->setContentsMargins(0,0,0,0);
    bdl->addWidget(bdh);
    bdl->addWidget(this->encbdepth,Qt::AlignRight);
    l->addRow("Bit Depth:", bdl);

    /* encoder row */
    /**/
    QCheckBox *enc = new QCheckBox;
    enc->setText("Set");
    enc->setChecked(false);
    connect(enc, &QCheckBox::clicked, this, &flachelper::flip_encpath);

    this->encline = new QLineEdit;
    this->encline->setPlaceholderText("/path/to/flachelper");

    QHBoxLayout *encset = new QHBoxLayout;
    encset->setContentsMargins(0,0,0,0);
    encset->addWidget(this->encline);
    QPushButton *pck = new QPushButton("...");
    encset->addWidget(pck);
    encset->setSpacing(0);
    pck->setFixedWidth(30);
    connect(pck, &QPushButton::clicked, this, &flachelper::pick_enc);

    this->encpath = new QFrame(this);
    this->encpath->setEnabled(false);
    this->encpath->setFrameShape(QFrame::Shape::StyledPanel);
    this->encpath->setFrameShadow(QFrame::Shadow::Raised);
    this->encpath->setContentsMargins(0,0,0,0);
    this->encpath->setLayout(encset);

    QHBoxLayout *pl = new QHBoxLayout;
    pl->addWidget(enc);
    pl->addWidget(this->encpath,Qt::AlignRight);
    l->addRow("Custom Encoder:", pl);

    /* extension field */
    /**/
    QLineEdit *ext = new QLineEdit;
    ext->setText("flachelper");
    ext->setFixedWidth(60);
    l->addRow("File Extension:", ext);

    /* args field */
    /**/
    QHBoxLayout *argl = new QHBoxLayout;
    argl->setSpacing(0);
    argl->setContentsMargins(0,0,0,0);
    QLineEdit *args = new QLineEdit;

    argl->addWidget(args);
    l->addRow("Additional Paramaters:", argl);

    QLabel *lbl = new QLabel();
    lbl->setText("<b>Note:</b><br><i>Setting these parameters to values higher"
                 " than those from the input files likely won't increase the "
                 "quality, but will increase the filesize.</i>");
    lbl->setFrameShape(QFrame::Shape::StyledPanel);
    lbl->setWordWrap(true);
    l->addRow(lbl);
}

flachelper::~flachelper(){}

void flachelper::refresh_enclevel(int value){
    this->enclevel->setText(QString::number(value));
}

void flachelper::flip_encsrate(){
    this->encsrate->setEnabled(!this->encsrate->isEnabled());
}

void flachelper::flip_encbdepth(){
    this->encbdepth->setEnabled(!this->encbdepth->isEnabled());
}

void flachelper::flip_encpath(){
    this->encpath->setEnabled(!this->encpath->isEnabled());
    this->encline->clear();
}

void flachelper::pick_enc(){
    QUrl enc = QFileDialog::getOpenFileUrl(
        this,
        "Path to custom flachelper Encoder",
        QDir::homePath()
    );

    if(enc.isEmpty() | !enc.isValid())
        return;

    this->encline->setText(enc.path());

}

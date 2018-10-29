#include <QGridLayout>
#include <QSqlRecord>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSizePolicy>
#include <QTableWidget>
#include <QTabWidget>
#include <QTreeWidget>
#include <QFileDialog>
#include <QComboBox>
#include <QFormLayout>

#include "src/vars.hpp"
#include "settings.hpp"
#include "src/library.hpp"
#include "src/mwindow.hpp"

settings::settings(QWidget *win, vars *jag) : QDialog(win){
    this->jag = jag;
    this->win = win;
    QVBoxLayout *l = new QVBoxLayout(this);
    QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Close);
    QTabWidget *tab = new QTabWidget(this);

    l->addWidget(tab);
    l->addWidget(bb);

    tab->addTab(this->spawn_maintab(win),"General");
    tab->addTab(this->spawn_libtab(win),"Library");
    connect(bb, &QDialogButtonBox::accepted, this, &settings::accept);
    connect(bb, &QDialogButtonBox::rejected, this, &settings::reject);

    this->setWindowTitle(QString("Settings Dialog :: OCELOT v")+jag->VERSION);
    this->setFixedSize(960, 540);
    this->setLayout(l);

}

settings::~settings(){

}

QWidget* settings::spawn_maintab(QWidget *win){
    QWidget *w = new QWidget(win);
    QGridLayout *grid = new QGridLayout();

    /* mouse behaviour groupbox */
    QGroupBox *mgbox = new QGroupBox("Mouse Behaviour");
    QFormLayout *mform = new QFormLayout();
    mform->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    QComboBox *dclick = new QComboBox();
    dclick->insertItems(0,QStringList()<<"Append to current playlist"
        <<"Replace current playlist"<<"Do nothing");
    mform->addRow(new QLabel("Double Click:", dclick), dclick);

    QComboBox *mclick = new QComboBox();
    mclick->insertItems(0,QStringList()<<"Append to current playlist"
        <<"Replace current playlist"<<"Do nothing");
    mform->addRow(new QLabel("Middle Click:", mclick), mclick);

    QComboBox *append = new QComboBox();
    append->insertItems(0, QStringList()<<"Start playing right away"
        <<"Play if there's nothing playing"<<"Just append");
    mform->addRow(new QLabel("When appending:"), append);

    /* set selection from db */
    dclick->setCurrentIndex(this->jag->fetchdata(new QString("general_doubleclick")).toInt());
    mclick->setCurrentIndex(this->jag->fetchdata(new QString("general_middleclick")).toInt());
    append->setCurrentIndex(this->jag->fetchdata(new QString("general_appendbehaviour")).toInt());

    connect(dclick, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &settings::gen_mdclick);
    connect(mclick, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &settings::gen_mmclick);
    connect(append, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &settings::gen_mappend);
    mgbox->setLayout(mform);

    /* other 1 */
    QGroupBox *lelbox = new QGroupBox("the other one");

    /* and another */

    QGroupBox *kekbox = new QGroupBox("the last one");

    /* and the last */
    QGroupBox *kbbox = new QGroupBox("Keyboard Shortcuts");
    kbbox->setLayout(new QGridLayout);
    QTableWidget *sc = new QTableWidget(15,2);
    sc->setHorizontalHeaderLabels(QStringList()<<"Action"<<"Shortcut");
    sc->setColumnWidth(0,130);
    sc->setColumnWidth(1,218);
    sc->setSelectionMode(QAbstractItemView::NoSelection);
    sc->setFixedWidth(400);
    kbbox->layout()->addWidget(sc);
    kbbox->setMaximumWidth(420);

    /* entangle everything */
    grid->addWidget(mgbox,0,0,1,1);
    grid->addWidget(lelbox,0,1,2,1);
    grid->addWidget(kekbox,0,2,2,1);
    grid->addWidget(kbbox,1,0,1,1);
    w->setLayout(grid);

    return w;
    /*QFormLayout *main_layout = new QFormLayout;

    QGroupBox *pl = new QGroupBox("Mouse Behaviour");
    QFormLayout *form = new QFormLayout;

    QGroupBox *plbox = new QGroupBox("on playlist");
    plbox->setAutoFillBackground(true);
    QFormLayout *plform = new QFormLayout;
    //plform->set

    QComboBox *dclick = new QComboBox();
    dclick->insertItems(0,QStringList()<<"Append to current playlist"
        <<"Append to new playlist"<<"Replace current playlist");
    plform->addRow(new QLabel("Double Click", dclick), dclick);

    QComboBox *mclick = new QComboBox();
    mclick->insertItems(0,QStringList()<<"Append to current playlist"
                       <<"Append to new playlist"<<"Replace current playlist");
    plform->addRow(new QLabel("Middle Click", mclick), mclick);

    QCheckBox *ck = new QCheckBox("Play on append");
    plform->addRow(ck);

    pl->setLayout(form);
    plbox->setLayout(plform);
    form->addWidget(plbox);
    main_layout->addWidget(pl);

    QComboBox *main_selbehaviour = new QComboBox(w);
    main_selbehaviour->insertItems(0, QStringList()<<"current selection"<<"current media playback");
    main_layout->addRow(new QLabel("UI contents follow:", w), main_selbehaviour);
    w->setLayout(main_layout);

    return w;*/
}

QWidget* settings::spawn_libtab(QWidget *win){
    QWidget *w = new QWidget(win);
    QGroupBox *fbox = new QGroupBox("Directories");
    fbox->setLayout(new QGridLayout);
    QGroupBox *sbox = new QGroupBox("Sorting Scheme");
    sbox->setLayout(new QGridLayout);
    sbox->setMinimumWidth(420); //nice

    /* setting up the directories group */
    QFrame *tframe = new QFrame();
    tframe->setLayout(new QGridLayout);
    tframe->setFrameStyle(QFrame::StyledPanel);
    tframe->setFrameShadow(QFrame::Shadow::Sunken);
    tframe->setContentsMargins(1,1,1,1);
    tframe->setAutoFillBackground(true);

    QCheckBox *ch1 = new QCheckBox("Watch for directory changes");
    ch1->setCheckState(Qt::CheckState::Checked);
    QCheckBox *ch2 = new QCheckBox("Scan subdirectories");
    ch2->setCheckState(Qt::CheckState::Checked);
    QTreeWidget *tree = new QTreeWidget();
    tree->setColumnCount(3);
    tree->setHeaderLabels(QStringList()<<"Path"<<"Track#"<<"Total Filesize");
    tree->setColumnWidth(0, 200);
    libstree = tree;
    lib_treerefresh();

    QPushButton *fpicker = new QPushButton();
    fpicker->setText("Add new...");
    fpicker->setMaximumWidth(120);
    connect(fpicker, &QPushButton::clicked, this, &settings::lib_dirpicker);

    QPushButton *rembtn = new QPushButton();
    rembtn->setText("Remove selected");
    rembtn->setMaximumWidth(120);
    connect(rembtn, &QPushButton::clicked, this, &settings::lib_remdir);

    qobject_cast<QGridLayout*>(tframe->layout())->addWidget(tree,0,0,1,3);
    qobject_cast<QGridLayout*>(tframe->layout())->addWidget(fpicker,1,0,1,1);
    qobject_cast<QGridLayout*>(tframe->layout())->addWidget(rembtn,1,1,1,1);

    qobject_cast<QGridLayout*>(fbox->layout())->addWidget(ch1,0,0,1,1);
    qobject_cast<QGridLayout*>(fbox->layout())->addWidget(ch2,0,1,1,1);
    qobject_cast<QGridLayout*>(fbox->layout())->addWidget(tframe,1,0,1,4);

    qobject_cast<QGridLayout*>(fbox->layout())->setHorizontalSpacing(2);
    qobject_cast<QGridLayout*>(fbox->layout())->setVerticalSpacing(4);

    /* then the sorting group */
    /* starting by the frame */
    QFrame *control = new QFrame();
    control->setLayout(new QGridLayout);
    control->setFrameStyle(QFrame::StyledPanel);
    control->setFrameShadow(QFrame::Shadow::Sunken);
    control->setContentsMargins(1,1,1,1);
    control->setAutoFillBackground(true);

    QLineEdit *nameline = new QLineEdit();
    nameline->setPlaceholderText("Give it a name");
    QLineEdit *schemeline = new QLineEdit();
    schemeline->setPlaceholderText("Write your scheme here");
    QPushButton *ins = new QPushButton();
    ins->setText("INSERT...");
    QPushButton *help = new QPushButton();
    help->setText("HELP");
    QPushButton *done = new QPushButton();
    done->setText("OK");

    qobject_cast<QGridLayout*>(control->layout())->addWidget(nameline,0,0,1,1);
    qobject_cast<QGridLayout*>(control->layout())->addWidget(done,0,1,1,1);
    qobject_cast<QGridLayout*>(control->layout())->addWidget(help,0,2,1,1);
    qobject_cast<QGridLayout*>(control->layout())->addWidget(schemeline,1,0,1,2);
    qobject_cast<QGridLayout*>(control->layout())->addWidget(ins,1,2,1,1);

    qobject_cast<QGridLayout*>(sbox->layout())->addWidget(control,0,0,1,2);

    /* the middle separator */
    QFrame *nsep = new QFrame();
    nsep->setFrameStyle(QFrame::HLine);
    //qobject_cast<QGridLayout*>(sbox->layout())->addWidget(nsep,1,0,1,2);

    /* and the view */
    QTreeWidget *stree = new QTreeWidget();
    stree->setColumnCount(2);
    stree->setHeaderLabels(QStringList() << "Name" << "Expression");
    stree->setColumnWidth(0, 128);
    stree->setAlternatingRowColors(true);
    stree->addTopLevelItem(new QTreeWidgetItem(stree,QStringList()<<"Default"<<"#artist#/[#year#] #album#/#track#. #title#"));
    qobject_cast<QGridLayout*>(sbox->layout())->addWidget(stree,2,0,1,2);

    w->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));
    w->layout()->addWidget(fbox);
    w->layout()->addWidget(sbox);

    return w;
}

void settings::lib_dirpicker(){
    QString dir = QFileDialog::getExistingDirectory(
        this,
        "Add new library",
        QDir::homePath(),
        QFileDialog::ShowDirsOnly
    );
    if(dir.isEmpty())
        return;

    qInfo() << "[INFO] Adding new library through settings dialog...";
    this->jag->libs->append(new library(dir,this->jag->DB_REF));
    QString st = this->jag->libs->last()->dumpinfo()->split(";").first();
    this->jag->DB_REF->exec("INSERT INTO libs VALUES ('"+st+"')");
    qInfo() << "[INFO] Done.";

    lib_treerefresh();
}

void settings::lib_treerefresh(){
    libstree->clear();

    for(int i=jag->libs->length()-1; i>=0; i--){
        QStringList s = jag->libs->at(i)->dumpinfo()->split(";");

        s.last().append(" MiB");
        libstree->addTopLevelItem(new QTreeWidgetItem(libstree, s));
    }

    qobject_cast<mwindow*>(this->win)->libchanged(this->jag->DB_REF);
}

void settings::lib_remdir(){
    if(this->jag->libs->length()==0)
        return;

    foreach(QTreeWidgetItem *s, this->libstree->selectedItems()){
        this->jag->remlibs(s->data(0, Qt::EditRole).toString());
    }

    lib_treerefresh();
}

/* combobox about double click behaviour */
void settings::gen_mdclick(int index){
    if(static_cast<uint>(index)==this->APPEND){

    }else if(static_cast<uint>(index)==this->REPLACE){

    }else{ /* do nothing */

    }
}

/* combobox about middle click behaviour */
void settings::gen_mmclick(int index){
    if(static_cast<uint>(index)==this->APPEND){

    }else if(static_cast<uint>(index)==this->REPLACE){

    }else{ /* do nothing */

    }
}

/* combobox about append behaviour */
void settings::gen_mappend(int index){
    if(static_cast<uint>(index)==this->PLAY){

    }else if(static_cast<uint>(index)==this->IFIDLE){

    }else{ /* don't play */

    }
}

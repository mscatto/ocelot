#include <QGridLayout>
#include <QSqlRecord>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSizePolicy>
#include <QTabWidget>
#include <QTreeWidget>
#include <QFileDialog>

#include "src/vars.hpp"
#include "settings.hpp"
#include "src/library.hpp"
#include "src/mwindow.hpp"

settings::settings(QWidget *win, vars *jag) : QDialog(win){
    this->jag = jag;
    QVBoxLayout *l = new QVBoxLayout(this);
    QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel|QDialogButtonBox::Apply);
    QTabWidget *tab = new QTabWidget(this);

    l->addWidget(tab);
    l->addWidget(bb);

    tab->addTab(this->spawn_maintab(win),"General");
    tab->addTab(this->spawn_libtab(win, jag->DB_REF),"Library");
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


    return w;
}

QWidget* settings::spawn_libtab(QWidget *win, QSqlDatabase *db){
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

    this->jag->libs->append(new library(dir,this->jag->DB_REF));
    QString st = this->jag->libs->last()->dumpinfo()->split(";").join("', '");
    this->jag->DB_REF->exec("INSERT INTO libs VALUES ('"+st+"')");

    lib_treerefresh();
}

void settings::lib_treerefresh(){
    if(this->jag->libs->length()==0)
        return;

    for(int i=jag->libs->length(); i>0; i--){
        QStringList s = jag->libs->at(i-1)->dumpinfo()->split(";");
        s.last().append(" GB");
        libstree->addTopLevelItem(new QTreeWidgetItem(libstree, s));
    }
}

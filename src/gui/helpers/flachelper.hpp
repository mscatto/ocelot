#ifndef FLACHELPER_HPP
#define FLACHELPER_HPP

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

class flachelper : public QWidget
{
    Q_OBJECT
public:
    flachelper(QWidget *parent = nullptr);
    ~flachelper();
private:
    QLabel *enclevel;
    QFrame *encsrate;
    QFrame *encbdepth;
    QFrame *encpath;
    QLineEdit *encline;
private slots:
    void refresh_enclevel(int value);
    void flip_encsrate();
    void flip_encbdepth();
    void flip_encpath();
    void pick_enc();
signals:

public slots:
};

#endif // FLACHELPER_HPP

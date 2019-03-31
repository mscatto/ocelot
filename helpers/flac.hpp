#ifndef FLAC_HPP
#define FLAC_HPP

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

class flac : public QWidget
{
    Q_OBJECT
public:
    flac(QWidget *parent = nullptr);
    ~flac();
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

#endif // FLAC_HPP

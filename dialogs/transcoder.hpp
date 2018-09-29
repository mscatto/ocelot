#ifndef TRANSCODER_HPP
#define TRANSCODER_HPP

#include <QObject>
#include <QWidget>
#include <QDialog>

#include "src/vars.hpp"

class transcoder : public QDialog
{
public:
    transcoder(vars *jag, QWidget *win);
    ~transcoder();
public slots:
    void handle(QStringList files);
};

#endif // TRANSCODER_HPP

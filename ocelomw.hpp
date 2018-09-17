#ifndef OCELOMW_HPP
#define OCELOMW_HPP

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class ocelomw : public QMainWindow
{
    Q_OBJECT
public:
    explicit ocelomw(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // OCELOMW_HPP
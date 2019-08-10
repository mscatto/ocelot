#ifndef SPLITTER_HPP
#define SPLITTER_HPP

#include <QWidget>
#include <QSplitter>

class splitter : public QSplitter
{
    Q_OBJECT
public:
    splitter(Qt::Orientation o, QWidget *parent = nullptr);
    ~splitter();
signals:

public slots:
};

#endif // SPLITTER_HPP

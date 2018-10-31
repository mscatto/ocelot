#ifndef WORKER_HPP
#define WORKER_HPP

#include<QObject>
#include<QCoreApplication>

class worker : public QObject {
    Q_OBJECT
public:
    worker();
    ~worker();
public slots:
    void process();
signals:
    void finished();
    void error(QString err);
private:
    // add your variables here
};

#endif // WORKER_HPP

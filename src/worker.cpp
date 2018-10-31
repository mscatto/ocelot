#include "worker.hpp"
#include <QString>

worker::worker(){ // Constructor
    //app = new QCoreApplication(argc,&argv);

    // you could copy data from constructor arguments to internal variables here.
}

worker::~worker(){

}

void worker::process(){ // Process. Start processing data.
    //app->exec();
    // allocate resources using new here
    //qDebug("Hello World!");
    emit finished();
}

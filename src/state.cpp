#include "state.hpp"
#include <QStringList>
#include <QDir>
#include <QMap>

state::state(){
    QDir().mkdir(cfpath);
    if(!QDir(cfpath).exists()){ // check if cfpath exists, and if not, create it
        qDebug() << "unable to create config directory in '"+cfpath+"' !";
        exit(1);
    }

    conf.setFileName(cfpath+"/"+cfname);
    if(!conf.open(QIODevice::ReadWrite)){
        qDebug() << "not allowed to open config file in rw-mode!";
        exit(1);
    }

    QTextStream in(&conf);
    while (!in.atEnd()){
        QString line = in.readLine();
        if(line.at(0)!="#" && line.contains("=")){ /*if its not a comment and a valid expression*/
            QStringList opt = line.split("=");
            if(state::checkvar(opt.first(), opt.last()))
                state::vars.insertMulti(opt.first(), opt.last());
            else
                qDebug() << "Error parsinag line ["+line+"] from user config! :: No such thing as '"+opt.first()+"'!";
        }
    }

}


QStringList *state::poll(QString key){
    return new QStringList(vars.values(key).toSet().toList());
}

void state::initvars(){
    //vars.insert(QString("library"), (cfpath+"/"+cfname));
}

bool state::checkvar(QString var, QString val){
    return state::opts.contains(var, Qt::CaseInsensitive);//should also ensure type validity
}

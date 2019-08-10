/*
 * The MIT License (MIT)
 * Copyright (c) 2018 Matheus Scattolin Anselmo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. In no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising from,
 * out of or in connection with the software or the use or other dealings in the
 * Software.
 */

#include <QApplication>
#include <QDebug>
#include <QMessageLogger>

#include "gui/dialogs/wizard.hpp"
#include "gui/mwindow.hpp"
#include "vars.hpp"

bool showwizard(vars* jag, mwindow* mw);

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Babousoft");
    QCoreApplication::setApplicationName("ocelot");

    vars* jag = new vars();
    mwindow* mw = new mwindow(jag);
    if(!showwizard(jag, mw))
        mw->show();

    return a.exec();
}

bool showwizard(vars* jag, mwindow* mw) {
    if(jag->fetchdbdata("general_runwizard") == 1) { /* first run */
        wizard* w = new wizard(jag);
        QObject::connect(w, &wizard::show_mwindow, mw, &mwindow::show);
        w->show();

        return true;
    }

    return false;
}

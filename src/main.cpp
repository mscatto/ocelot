/*
 * Ocelot Music Manager: music player and library manager built using Qt
 * Copyright (C) 2018 Matheus Scattolin Anselmo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QMessageLogger>
#include <QDebug>
#include <QStandardPaths>
#include <QSqlQueryModel>
#include <QToolBar>
#include <QStatusBar>
#include <QMainWindow>
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>

#include "mwindow.hpp"
#include "library.hpp"
#include "vars.hpp"
#include "dialogs/wizard.h"

void checkwizard(vars *jag);

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    vars *jag = new vars();
    checkwizard(jag);

    mwindow *mw = new mwindow(jag);
    mw->show();

    return a.exec();
}

void checkwizard(vars *jag){
    QSqlQuery q = jag->DB_REF->exec("SELECT val FROM data WHERE var='general_runwizard'");
    q.next();
    if(q.value(0)==1){
        wizard *w = new wizard(jag);
        w->show();
    }
}

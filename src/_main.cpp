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

#include "gui/dialogs/wizard.hpp"
#include "src/gui/dialogs/settings.hpp"
#include "gui/mwindow.hpp"
#include "src/libs/QSingleInstance/qsingleinstance.h"
#include "vars.hpp"
#include <QApplication>
#include <QDebug>
#include <QMessageLogger>

int main (int argc, char* argv[]) {
	QApplication a (argc, argv);
	QCoreApplication::setOrganizationName ("Babousoft");
	QCoreApplication::setApplicationName ("ocelot");
	QSingleInstance instance;
	instance.setGlobal (true);

	if (!instance.process()) {
		std::cout << ("[WARNING] Ocelot is already running! Stepping down.\n"
					  "Any arguments will be delivered to the master instance!\n");
		return EXIT_SUCCESS;
	}

	vars* jag = new vars();
	mwindow* mw = new mwindow (jag);
	bool firstrun = (jag->fetchdbdata ("general_runwizard") == 1);
	QObject::connect (&instance, &QSingleInstance::instanceMessage, mw, &mwindow::args_act);

	if (argc > 1 && !firstrun)	// so it won't start playing whenever there is no database
		mw->args_act (a.arguments());

	if (firstrun) {
		wizard* w = new wizard (mw, jag);
		QObject::connect (w, &wizard::show_mwindow, mw, &mwindow::show);
		w->show();
	} else
		mw->show();

	return a.exec();
}

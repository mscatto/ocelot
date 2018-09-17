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
#ifndef STATE_HPP
#define STATE_HPP
#include <QString>
#include <QFile>
#include <QDebug>
#include <QStandardPaths>

class state
{
    const QString cfname = "ocelot.conf";
    const QStringList opts = {"dbsrc"};

    QString cfpath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/ocelot";
    QFile conf;
    QMap<QString, QString> vars;
    void initvars();
    bool checkvar(QString var, QString val);
public:
    state();
    QStringList* poll(QString key);
};

#endif // STATE_HPP

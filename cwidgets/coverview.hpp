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
#ifndef COVERVIEW_HPP
#define COVERVIEW_HPP

#include<QLabel>
#include"mwindow.hpp"

class coverview : public QLabel
{
private:
    QPixmap *cover;
public:
    coverview(mwindow *win);
    ~coverview();
    //virtual void paintEvent(QPaintEvent *p);
public slots:
    void coverchanged(QPixmap *nc);
};

#endif // COVERVIEW_HPP

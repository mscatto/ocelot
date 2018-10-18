/*
 * Ocelot Music Manager: A music player and library manager built using Qt
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
#include "renamepbtn.hpp"
#include <QtDebug>

void renamepbtn::set_key(QString nkey){
    this->mapkey = nkey;
}

renamepbtn::renamepbtn(const QIcon &icon, QString key) : QPushButton(icon,""){
    this->mapkey = key;
    connect(this, &QPushButton::clicked, this, &renamepbtn::slot_clicked);
}

renamepbtn::~renamepbtn(){

}

void renamepbtn::slot_clicked(bool checked){
    this->idclicked(this->mapkey);
}
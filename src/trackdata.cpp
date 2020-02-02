#include "trackdata.hpp"
#include <QDebug>
#include <QMimeDatabase>

// trackdata::filetype trackdata::ftresolver(){ QMimeDatabase }

trackdata::trackdata (const char* filepath) {
	this->filepath = filepath;
	this->fref = new TagLib::FileRef (filepath);
	this->ft = filetype::FLAC;
}

trackdata::~trackdata() {
	this->fref->~FileRef();
}

QString trackdata::title() {
	return QString (this->fref->tag()->title().toCString (true));
}

QString trackdata::artist() {
	return QString (this->fref->tag()->artist().toCString (true));
}

QString trackdata::album() {
	return QString (this->fref->tag()->album().toCString (true));
}

uint trackdata::year() {
	return this->fref->tag()->year();
}

int trackdata::bitrate() {
	return this->fref->audioProperties()->bitrate();
}

int trackdata::samples() {
	return this->fref->audioProperties()->sampleRate();
}

uint trackdata::length_sec() {
	return static_cast<uint> (this->fref->audioProperties()->lengthInSeconds());
}

QPixmap trackdata::cover() {
	switch (this->ft) {
	case FLAC:
		TagLib::FLAC::File x (this->filepath);
		if (x.pictureList().front() != nullptr) {
			QPixmap np;
			np.loadFromData ((const uchar*)x.pictureList().front()->data().data(),
							 x.pictureList().front()->data().size());
			np.scaled (200, 200, Qt::KeepAspectRatio);
			return np;
		}
	}
	return QPixmap();
}

/* This file contains definitions for the LoadingLabel class (derived from QLabel)
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include <QMovie>

//Local
#include "LoadingLabel.h"
#include "util.h"


LoadingLabel::LoadingLabel(QWidget* parent)
    : QLabel(parent),
    StyleWidget(this){
    set_icon();
}

void LoadingLabel::set_status_active() {
    stop_movie();
    set_movie();
}

void LoadingLabel::set_status_inactive() {
    stop_movie();
    set_icon();
}

void LoadingLabel::set_icon() {
    QPixmap pixmap(STATIC_ICON);
    setPixmap(pixmap.scaled(this->size(), Qt::KeepAspectRatio));
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    show();
}

void LoadingLabel::set_movie() {
    QMovie *movie = new QMovie(MOVING_ICON);
    int height = this->size().height();
    QSize size(height, height);
    movie->setScaledSize(size);
    setMovie(movie);
    movie->start();
}

void LoadingLabel::stop_movie() {
    auto old_movie = movie();
    if(old_movie == nullptr) {
        delete old_movie;
    }
}


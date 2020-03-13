//---------------------------------------------------------------------------------------------------------------------
//  Rosex Hidraulica Sl
//---------------------------------------------------------------------------------------------------------------------
//
//  Copyright 2020 Rosex Hidraulica Sl  - All Rights Reserved
//
//  Unauthorized copying of this file, via any medium is strictly prohibited Proprietary and confidential.
//
//  All information contained herein is, and remains the property of Vertical Engineering Solutions.  The
//  intellectual and technical concepts contained herein are proprietary to Vertical Engineering Solutions
//  and its suppliers and may be covered by UE and Foreign Patents, patents in process, and are protected
//  by trade secret or copyright law. Dissemination of this information or reproduction of this material is
//  strictly forbidden unless prior written permission is obtained from Adobe Systems Incorporated.
//
//---------------------------------------------------------------------------------------------------------------------
//
//  Maintainer: pabramsor@gmail.com
//
//---------------------------------------------------------------------------------------------------------------------


#include <boat_display/CompassWidget.h>

#include <QImageReader>

namespace rosex{
    Compass::Compass(QWidget *parent) : QMainWindow(parent) {
        this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
        this->setWindowState(Qt::WindowFullScreen);
        this->showFullScreen();

        waterBg_ = loadImage("/home/bardo91/programming/boat_display/resources/water_bg.jpg");
        compass_ = loadImage("/home/bardo91/programming/boat_display/resources/compass.png");
        arrow_ = loadImage("/home/bardo91/programming/boat_display/resources/arrow.png");
    }

    void Compass::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);
        // p.drawImage(QPoint(0,0), waterBg_);

        p.drawImage(QPoint(0,0), compass_);

        p.drawImage(QPoint(winSize[1]/2 - 50,winSize[1]/2- 50), arrow_);

        //p.save();
        //p.translate(rect.center());
        //p.rotate(orientation_);
        //p.drawRect(QRect(-50, -25, 100, 50));
        //p.restore();
    }


    QImage Compass::loadImage(const QString &_fileName){
        QImageReader reader(_fileName);
        reader.setAutoTransform(true);
        QImage image = reader.read();
        return image;
    }

    void Compass::updateOrientation(float _ori){
        orientation_ = _ori;
        this->update();
    }
}
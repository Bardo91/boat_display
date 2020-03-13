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


        std::string userDir(getenv("USER"));
        std::string resourcesDir = "/home/"+userDir+"/.boat_display/resources/";
        waterBg_ = loadImage(resourcesDir+"water_bg.png");
        compass_ = loadImage(resourcesDir+"compass.png");
        arrow_ = loadImage(resourcesDir+"arrow.png");
    }

    float counter = 0;
    void Compass::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
        QPainter p(this);
        p.setFont(QFont( "Arial", 50 ));
        p.setRenderHint(QPainter::Antialiasing, true);
        p.drawImage(QPoint(0,0), waterBg_);
        // p.setOpacity(0.2);

        p.drawImage(QPoint(10,10), compass_);

        QMatrix matrix;
        matrix.rotate(counter+=1);
        QImage arrowRot = arrow_.transformed(matrix);

        p.drawImage(QPoint( winSize[1]/2 - arrowRot.rect().center().x(),
                            winSize[1]/2 - arrowRot.rect().center().y()), arrowRot);


        const QRect rectangle = QRect(winSize[1]+ 50, 50, 240, 100);
        QRect boundingRect;
        p.drawText( rectangle, 0, std::to_string(counter).c_str(),&boundingRect);

        //p.save();
        //p.translate(rect.center());
        //p.rotate(orientation_);
        //p.drawRect(QRect(-50, -25, 100, 50));
        //p.restore();
    }


    QImage Compass::loadImage(const std::string &_fileName){
        QImageReader reader(_fileName.c_str());
        reader.setAutoTransform(true);
        QImage image = reader.read();
        return image;
    }

    void Compass::updateOrientation(float _ori){
        orientation_ = _ori;
        this->update();
    }
}
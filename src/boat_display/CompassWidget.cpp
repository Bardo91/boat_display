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


namespace rosex{
    Compass::Compass(QWidget *parent) : QWidget(parent) {
        rect = QRect(0, 0, 300, 300);
    }

    QSize Compass::sizeHint() const {
        return rect.size();
    }

    void Compass::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.drawEllipse(rect);

        p.save();
        p.translate(rect.center());
        p.rotate(orientation_);
        p.drawRect(QRect(-50, -25, 100, 50));
        p.restore();
    }

    void Compass::updateOrientation(float _ori){
        orientation_ = _ori;
        this->update();
    }
}
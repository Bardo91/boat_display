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

#ifndef BOATDISPLAT_CONFIGDIALOG_H_
#define BOATDISPLAT_CONFIGDIALOG_H_

#include <QDialog>
#include <QLabel>
#include <QSpinBox>

namespace rosex{
    class ConfigDialog: public QDialog{
        Q_OBJECT
    public:
        explicit ConfigDialog(std::atomic<int> &_signal, QWidget *_parent = 0);

	~ConfigDialog();    

        int minVal() const;
        int maxVal() const;
        int directionVal() const;
        int divisionVal() const;
        int offset() const;

    private:
	QTimer *signalTimer_;
        QLabel *valLabel_;
        QSpinBox *minSpin_, *maxSpin_, *divisionSpin_, *directionSpin_, *offsetSpin_;
        std::atomic<int> &signal_;
    };
}


#endif


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

#include <boat_display/ConfigDialog.h>

#include <QVBoxLayout>
#include <QHBoxLayout>

namespace rosex{

    ConfigDialog::ConfigDialog(std::atomic<int> &_signal, QWidget *_parent): signal_(_signal){
        QVBoxLayout *mainLayout = new QVBoxLayout();
        this->setLayout(mainLayout);

        //-------------------------------------------------------------------------------------------------------------
        QHBoxLayout *currentValLayout = new QHBoxLayout();
        mainLayout->addLayout(currentValLayout);
        QLabel *valLabelName = new QLabel("Current Value: ");
        valLabel_ = new QLabel();
        currentValLayout->addWidget(valLabelName);
        currentValLayout->addWidget(valLabel_);

        //-------------------------------------------------------------------------------------------------------------
        QHBoxLayout *minValLayout = new QHBoxLayout();
        mainLayout->addLayout(minValLayout);
        QLabel *minLabelName = new QLabel("Min value: ");
        minSpin_ = new QSpinBox();
        minValLayout->addWidget(minLabelName);
        minValLayout->addWidget(minSpin_);

        //-------------------------------------------------------------------------------------------------------------
        QHBoxLayout *maxValLayout = new QHBoxLayout();
        mainLayout->addLayout(maxValLayout);
        QLabel *maxLabelName = new QLabel("Max value: ");
        maxSpin_ = new QSpinBox();
        maxValLayout->addWidget(maxLabelName);
        maxValLayout->addWidget(maxSpin_);

        //-------------------------------------------------------------------------------------------------------------
        QHBoxLayout *directionLayout = new QHBoxLayout();
        mainLayout->addLayout(directionLayout);
        QLabel *directionLabelName = new QLabel("Direction value: ");
        directionSpin_ = new QSpinBox();
        directionLayout->addWidget(directionLabelName);
        directionLayout->addWidget(directionSpin_);

        //-------------------------------------------------------------------------------------------------------------
        QHBoxLayout *divisionFactorLayout = new QHBoxLayout();
        QLabel *divisionLabelName = new QLabel("Divisions value: ");
        mainLayout->addLayout(divisionFactorLayout);
        divisionSpin_ = new QSpinBox();
        divisionFactorLayout->addWidget(divisionLabelName);
        divisionFactorLayout->addWidget(divisionSpin_);
    }

}
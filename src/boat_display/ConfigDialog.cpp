
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
#include <QTimer>
#include <cassert>

#include <yaml-cpp/yaml.h>

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
        minSpin_->setMaximum(1025);
        minValLayout->addWidget(minLabelName);
        minValLayout->addWidget(minSpin_);

        //-------------------------------------------------------------------------------------------------------------
        QHBoxLayout *maxValLayout = new QHBoxLayout();
        mainLayout->addLayout(maxValLayout);
        QLabel *maxLabelName = new QLabel("Max value: ");
        maxSpin_ = new QSpinBox();
        maxSpin_->setMaximum(1025);
        maxValLayout->addWidget(maxLabelName);
        maxValLayout->addWidget(maxSpin_);

        //-------------------------------------------------------------------------------------------------------------
        QHBoxLayout *directionLayout = new QHBoxLayout();
        mainLayout->addLayout(directionLayout);
        QLabel *directionLabelName = new QLabel("Direction value: ");
        directionSpin_ = new QSpinBox();
        directionSpin_->setMaximum(1);
        directionSpin_->setMinimum(-1);
        directionLayout->addWidget(directionLabelName);
        directionLayout->addWidget(directionSpin_);

        //-------------------------------------------------------------------------------------------------------------
        QHBoxLayout *divisionFactorLayout = new QHBoxLayout();
        QLabel *divisionLabelName = new QLabel("Divisions value: ");
        mainLayout->addLayout(divisionFactorLayout);
        divisionSpin_ = new QSpinBox();
        divisionSpin_->setMaximum(10);
        divisionSpin_->setMinimum(1);
        divisionFactorLayout->addWidget(divisionLabelName);
        divisionFactorLayout->addWidget(divisionSpin_);
    

        //-------------------------------------------------------------------------------------------------------------
        // Load defaults
        std::string userDir(getenv("USER"));
        YAML::Node paramFile;
        try{
            paramFile = YAML::LoadFile("/home/"+userDir+"/.boat_display/config/config.yml");
        }catch(YAML::ParserException _e){
            assert(false);
        }

        minSpin_->setValue(paramFile["min_val"].as<int>());
        maxSpin_->setValue(paramFile["max_val"].as<int>());
        directionSpin_->setValue(paramFile["direction"].as<int>());
        divisionSpin_->setValue(paramFile["division_factor"].as<int>());

	signalTimer_ = new QTimer();
	
	connect(signalTimer_, &QTimer::timeout, [&](){
		valLabel_->setText(std::to_string(_signal).c_str());
	});
	signalTimer_->start(100);
    }

	ConfigDialog::~ConfigDialog(){
		signalTimer_->stop();
	}

    //-------------------------------------------------------------------------------------------------------------
    int ConfigDialog::minVal() const{
        return minSpin_->value();
    }

    //-------------------------------------------------------------------------------------------------------------
    int ConfigDialog::maxVal() const{
        return maxSpin_->value();
    }

    //-------------------------------------------------------------------------------------------------------------
    int ConfigDialog::directionVal() const{
        int dir = directionSpin_->value();
        if(dir == 0)
            dir = 1;

        return dir;
    }
    
    //-------------------------------------------------------------------------------------------------------------
    int ConfigDialog::divisionVal() const{
        return divisionSpin_->value();
    }


}

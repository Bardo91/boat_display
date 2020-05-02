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

#include <iostream>
#include <fstream>
#include <cassert>

#include <QImageReader>
#include <algorithm>

#include <boat_display/ConfigDialog.h>

#include <yaml-cpp/yaml.h>


#include <filesystem>
namespace fs = std::filesystem;

namespace rosex{
    Compass::Compass(QWidget *parent) : QMainWindow(parent) {
        this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
        this->setWindowState(Qt::WindowFullScreen);
        this->showFullScreen();

	logFile_.open(std::string("/home/")+getenv("USER")+"/.boat_display/Log_"+std::to_string(time(NULL))+".txt");
	logFile_ << "Initializing HMI" << std::endl;
	logFile_.flush();
        std::string userDir(getenv("USER"));
        std::string resourcesDir = "/home/"+userDir+"/.boat_display/resources/";
        waterBg_ = loadImage(resourcesDir+"water_bg.png");
        compass_ = loadImage(resourcesDir+"compass.png");
        arrow_ = loadImage(resourcesDir+"arrow.png");


	logFile_ << "Loading configuration" << std::endl;
	logFile_.flush();
        // Load defaults
        YAML::Node paramFile;
        try{
            paramFile = YAML::LoadFile("/home/"+userDir+"/.boat_display/config/config.yml");
        }catch(YAML::ParserException _e){
            assert(false);
        }

        MIN_VAL = paramFile["min_val"].as<int>();
        MAX_VAL = paramFile["max_val"].as<int>();
        DIVISION_FACTOR = paramFile["division_factor"].as<int>();
        DIRECTION = paramFile["direction"].as<int>();

        lastSignal_ = MIN_VAL;
	
        // initI2C();
	logFile_ << "Initializing read thread" << std::endl;
	logFile_.flush();
        run_ = true;

        readingThread_ = std::thread([&](){
		  openSerialPort();
		  while(run_){
		        try{
		            std::string line = serialPort_->readline();
				logMutex_.lock();
				logFile_ << "[SERIAL_THREAD] Read\"" << line << "\" from serial port "<< std::endl;
				logFile_.flush();
				logMutex_.unlock();
		            lastSignal_ = atoi(line.c_str());
			}catch(serial::IOException _e){
		            	logMutex_.lock();
				logFile_ << "[SERIAL_THREAD] Failed Reading thread" << std::endl;
				logFile_ << _e.what() <<std::endl;;
				logFile_.flush();
				logMutex_.unlock();
		            serialPort_ = nullptr;
			    openSerialPort();
		        } catch(serial::SerialException _e){
		            	logMutex_.lock();
				logFile_ << "[SERIAL_THREAD] Failed Reading thread"<< std::endl;
				logFile_ << _e.what() <<std::endl;;
				logFile_.flush();
				logMutex_.unlock();
		            serialPort_ = nullptr;
			    openSerialPort();
		        }
		    }
		});


    }

    bool Compass::openSerialPort(){
	while(serialPort_ == nullptr){
		std::string path = "/dev/";
		std::vector<std::string> candidatePorts;

		for (const auto & entry : fs::directory_iterator(path)){
			if(entry.path().string().find("ACM") != std::string::npos){
				candidatePorts.push_back(entry.path());
			} else if(entry.path().string().find("USB") != std::string::npos){
				candidatePorts.push_back(entry.path());
			}
		}
		logMutex_.lock();
		logFile_ << "[SERIAL_THREAD] Found " << candidatePorts.size() << " ports." << std::endl;
		logFile_.flush();
		logMutex_.unlock();


		for(auto portPath: candidatePorts){
			try{
				serialPort_ = new serial::Serial(portPath, 115200, serial::Timeout::simpleTimeout(1000));
				logMutex_.lock();
				logFile_ << "[SERIAL_THREAD] connected to :" << portPath << std::endl;
				logFile_.flush();
				logMutex_.unlock();
				return true;
			}catch(serial::IOException _e){
				logMutex_.lock();
				logFile_ << "[SERIAL_THREAD] Failed to connected to :" << portPath << std::endl;
				logFile_ << _e.what() <<std::endl;;
				logFile_.flush();
				logMutex_.unlock();
				serialPort_ = nullptr;
			}
		}
	}
    }

    Compass::~Compass(){
        run_ = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if(serialPort_ != nullptr && serialPort_->isOpen()){
            serialPort_->close();
        }

        if(readingThread_.joinable())
            readingThread_.join();
    }


static bool switchLabelCheck_ = false;
    void Compass::paintEvent(QPaintEvent *event) {

        // uint16_t signal = sensorHandler_.Measure_SingleEnded(0);
        float signalF = lastSignal_;

        signalF = (MAX_VAL-signalF)/(MAX_VAL-MIN_VAL)*360;
        
        queueValues_.push_back(signalF);
        if(queueValues_.size() > maxQueueSize_){
            queueValues_.erase(queueValues_.begin());
        }

        int signal;
        if(queueValues_.size() == maxQueueSize_){
            float sm = 0, cm = 0;
            for(unsigned i = 0; i < maxQueueSize_; i++){
                sm += sin(queueValues_[i]*M_PI/180.0f);
                cm += cos(queueValues_[i]*M_PI/180.0f);
            }
            sm /= maxQueueSize_;
            cm /= maxQueueSize_;

            float medAngle = atan2(sm,cm)*180.0f/M_PI;
            
            signal = int(medAngle);
        }else{
            signal = int(signalF);
        }

        signal = signal / DIVISION_FACTOR * DIVISION_FACTOR;

        if(DIRECTION == -1){
            signal = 360 - signal;
        }

        signal = (signal + OFFSET)%360;


        Q_UNUSED(event);
        QPainter p(this);
        p.setFont(QFont( "Arial", 50 ));
        p.setRenderHint(QPainter::Antialiasing, true);
        p.drawImage(QPoint(0,0), waterBg_);
        // p.setOpacity(0.2);

        p.drawImage(QPoint( this->width()/2-compass_.width()/2,
                            this->height()/2-compass_.height()/2), 
                    compass_);

        QMatrix matrix;
        matrix.rotate(signal);
        QImage arrowRot = arrow_.transformed(matrix);

        p.drawImage(QPoint( this->width()/2 - arrowRot.width()/2,
                            this->height()/2 - arrowRot.height()/2), arrowRot);

	if(switchLabelCheck_){
		p.setPen(QPen(QColor(255, 0,0, 255), 3));	
	} else{
		p.setPen(QPen(QColor(0,255,0, 255), 3));
	}
	switchLabelCheck_ = !switchLabelCheck_;
	p.drawEllipse(QRectF(0,0,50,50));

	logMutex_.lock();
	logFile_ << "[PAINT_THREAD] Updated HMI with val\"" << signal << "\""<< std::endl;
	logFile_.flush();
	logMutex_.unlock();
        // const QRect rectangle = QRect(winSize[1]+ 50, 50, 240, 100);
        // QRect boundingRect;
        // p.drawText( rectangle, 0, std::to_string(signal).c_str(),&boundingRect);

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

    void Compass::initI2C(){

        sensorHandler_.getAddr_ADS1115(ADS1115_DEFAULT_ADDRESS);   // 0x48, 1001 000 (ADDR = GND)

        // The ADC gain (PGA), Device operating mode, Data rate can be changed via the following functions
        sensorHandler_.setGain(GAIN_TWO);          // 2x gain   +/- 2.048V  1 bit = 0.0625mV (default)
        // sensorHandler_.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 0.1875mV
        // sensorHandler_.setGain(GAIN_ONE);       // 1x gain   +/- 4.096V  1 bit = 0.125mV
        // sensorHandler_.setGain(GAIN_FOUR);      // 4x gain   +/- 1.024V  1 bit = 0.03125mV
        // sensorHandler_.setGain(GAIN_EIGHT);     // 8x gain   +/- 0.512V  1 bit = 0.015625mV
        // sensorHandler_.setGain(GAIN_SIXTEEN);   // 16x gain  +/- 0.256V  1 bit = 0.0078125mV

        sensorHandler_.setMode(MODE_CONTIN);       // Continuous conversion mode
        // sensorHandler_.setMode(MODE_SINGLE);    // Power-down single-shot mode (default)

        sensorHandler_.setRate(RATE_128);          // 128SPS (default)
        // sensorHandler_.setRate(RATE_8);         // 8SPS
        // sensorHandler_.setRate(RATE_16);        // 16SPS
        // sensorHandler_.setRate(RATE_32);        // 32SPS
        // sensorHandler_.setRate(RATE_64);        // 64SPS
        // sensorHandler_.setRate(RATE_250);       // 250SPS
        // sensorHandler_.setRate(RATE_475);       // 475SPS
        // sensorHandler_.setRate(RATE_860);       // 860SPS

        sensorHandler_.setOSMode(OSMODE_SINGLE);   // Set to start a single-conversion

        sensorHandler_.begin();
    }


    void Compass::keyPressEvent(QKeyEvent *_event){
        if (_event->type()==QEvent::KeyPress) {
            QKeyEvent* key = static_cast<QKeyEvent*>(_event);
            if ((key->key() == Qt::Key_O)  &&  (key->modifiers() & Qt::ControlModifier) ) {
                // ctrl+c pressed
                // std::cout <<"pressed ctrl+o" << std::endl;
                ConfigDialog configDialog(lastSignal_);
                configDialog.exec();

                MIN_VAL         = configDialog.minVal();
                MAX_VAL         = configDialog.maxVal();
                DIVISION_FACTOR = configDialog.divisionVal();
                DIRECTION       = configDialog.directionVal();
                OFFSET          = configDialog.offset();

                // Save defaults
                std::string userDir(getenv("USER"));
                std::ofstream file("/home/"+userDir+"/.boat_display/config/config.yml");
                
                YAML::Node paramFile;
                paramFile["min_val"]            = MIN_VAL;
                paramFile["max_val"]            = MAX_VAL;
                paramFile["division_factor"]    = DIVISION_FACTOR;
                paramFile["direction"]          = DIRECTION;
                paramFile["offset"]             = OFFSET;
                file << paramFile;
                file.flush();
                file.close();
            } 
        }
    }

}

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

#ifndef ROSEX_BOATDISPLAY_COMPASSWIDGET_H_
#define ROSEX_BOATDISPLAY_COMPASSWIDGET_H_

#include <QtGui>
#include <QMainWindow>
#include <QTimer>
#include <boat_display/ADS1115.h>

#include <serial/serial.h>

#include <atomic>
#include <thread>
#include <mutex>

#include <fstream>

namespace rosex{

    class Compass : public QMainWindow {
        Q_OBJECT

        public:
            explicit Compass(QWidget *parent = 0);
        
            ~Compass();
            // QSize sizeHint() const;
        
            void updateOrientation(float _ori);

        protected:
            void paintEvent(QPaintEvent *event);

            QImage loadImage(const std::string &_filename);

            void initI2C();

            void keyPressEvent(QKeyEvent *_event) override;

	    bool openSerialPort();

        private:
            QRect rect;
            float orientation_;

            QImage waterBg_, compass_, arrow_;
            const int winSize[2] = {800, 480};

            ADS1115 sensorHandler_;

            serial::Serial *serialPort_ = nullptr;
            bool run_ = false;
            std::thread readingThread_;

            std::atomic<int> lastSignal_;

            std::vector<float> queueValues_;
            int maxQueueSize_ = 20;

            QTimer *paintTimer_;

	    std::ofstream logFile_;
		std::mutex logMutex_;
	
            std::chrono::steady_clock::time_point t0;
      
            float MIN_VAL = 170;
            float MAX_VAL = 850;
            int DIVISION_FACTOR = 5;
            int DIRECTION = -1;
            int OFFSET = 180;

    };

}

#endif

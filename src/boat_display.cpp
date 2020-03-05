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
#include <QApplication>

#include <thread>
#include <chrono>

int main(int _argc, char** _argv){
    QApplication a(_argc, _argv);
    rosex::Compass c;
    c.show();

    std::thread th([&](){
        float orientation = 0;
        float orientationSpeed = 1;
        while(true){
            orientation += orientationSpeed;
            c.updateOrientation(orientation);
            std::this_thread::sleep_for(std::chrono::milliseconds(30));    

            if(float(rand())/RAND_MAX < 0.01){
                orientationSpeed*=-1;
            }
        }
    });

    return a.exec();
}
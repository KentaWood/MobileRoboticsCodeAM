#include <QMC5883LCompass.h>

class Compass {
    QMC5883LCompass compass;

    public:
        void setup() {
            compass.init();
            compass.setSmoothing(10,true);
            compass.setCalibrationOffsets(-438.00, 126.00, -14.00);
            compass.setCalibrationScales(0.99, 1.05, 0.97);
        }



        void loopStep() {
            compass.read();

        }

        float getTheta(){
            return compass.getAzimuth();
        }
};
#include "nnxt.h"
#include "customMovement.h"

/**
 * @brief makes the robot drive forward for a specific distance
 * @param distanceInCm  how far should the bot go (in cm)
 * @param direction in which direction should it go
 */
void drive_forward(int distanceInCm, bool direction){
    Motor_Drive(ENG_L, direction, 45*ENG_L_FINEADJ);
    Motor_Drive(ENG_R, direction, 45*ENG_R_FINEADJ);

    //Kalibriert auf 5cm. Keine Rücksicht auf Beschleunigungszeit
    Delay(120*distanceInCm);
    Motor_Stop(ENG_L, Motor_stop_float);
    Motor_Stop(ENG_R, Motor_stop_float);
}

/**
 * @brief makes the robot spin on the spot for a specific amount of degrees
 * @param direction in which direction should the robot turn
 * @param degrees   how far should the robot spin
 */
void turn(bool direction, int degrees){
     Motor_Drive(ENG_L, FORWARD^direction, 45*ENG_L_FINEADJ);
     Motor_Drive(ENG_R, BACKWARD^direction, 45*ENG_R_FINEADJ);
     Delay(20*degrees);
     Motor_Stop(ENG_L, Motor_stop_float);
     Motor_Stop(ENG_R, Motor_stop_float);
}

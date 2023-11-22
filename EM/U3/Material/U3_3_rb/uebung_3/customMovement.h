#ifndef CUSTOMMOVEMENT_H_INCLUDED
#define CUSTOMMOVEMENT_H_INCLUDED

//Engines are not always the same speed, so they can be fine tuned here
#define ENG_L_FINEADJ 1
#define ENG_R_FINEADJ 0.94

//Engines spin backwards on this specific robot, so they are turned around here
#define FORWARD Motor_dir_backward
#define BACKWARD Motor_dir_forward
//forward = 0
//backward = 1

#define ENG_L Port_B
#define ENG_R Port_A
#define SONIC Port_1
#define TASTER_R Port_0
#define TASTER_L Port_2

#define CLOCKWISE 0
#define ANTICLOCKWISE 1

typedef enum{
false,
true
} bool;

void drive_forward(int distanceInCm, bool direction);
void turn(bool direction, int degrees);

#endif /* CUSTOMMOVEMENT_H_INCLUDED */

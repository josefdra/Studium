#include "nnxt.h"
#include "customMovement.h"
#include "stdio.h"
#include "timer.h"

#define EV_TASTER_L 0x1
#define EV_TASTER_R 0x2
#define EV_RETURN 0x4

void checkTasterL(){
    //Initialize Ports
    SensorConfig(TASTER_L, SensorTouch);

    //Initialize Variables
    sensor_touch_clicked_t button;

    while(1){
        //Update Button press
        Touch_Clicked(TASTER_L, &button);

        //Set Button to pressed, if pressed
        if(button == SensorTouch_clicked){
            setEvent(EV_TASTER_L);
            //Wait for button release
            while(button == SensorTouch_clicked){
                Touch_Clicked(TASTER_L, &button);
                Delay(100);
            }
        }

        //Slow down task a bit
        Delay(100);
    }
}

void checkTasterR(){
    //Initialize Ports
    SensorConfig(TASTER_R, SensorTouch);
    //Initialize Variables
    sensor_touch_clicked_t button;

    while(1){
        //Update Button press
        Touch_Clicked(TASTER_R, &button);

        //Set Button to pressed, if pressed
        if(button == SensorTouch_clicked){
            setEvent(EV_TASTER_R);
            //Wait for button release
            while(button == SensorTouch_clicked){
                Touch_Clicked(TASTER_R, &button);
                Delay(100);
            }
        }

        //Slow down task a bit
        Delay(100);
    }
}

void controlBot(){
    //Initialize Ports
    MotorPortInit(ENG_R);
    MotorPortInit(ENG_L);
    setTimer(0, 5000, EV_RETURN);
    char position = 'd';
    char direction = 'o';
    while(1){

        NNXT_LCD_DisplayChar(10, 5, position);
        NNXT_LCD_DisplayChar(10, 55, direction);

        if(eventIsSet(EV_TASTER_L)){
            clearEvent(EV_TASTER_L);

            cancelTimer(0);
            if(position != 'a'){
                    if(direction == 'l'){
                    drive_forward(10,FORWARD);
                }else if(direction == 'r'){
                    turn(CLOCKWISE,180);
                    drive_forward(10,FORWARD);
                }else if(direction == 'o'){
                    turn(ANTICLOCKWISE,90);
                    drive_forward(10,FORWARD);
                }
            }
            startTimer(0); //reset timer

            switch(position){
                case 'b':
                    position = 'a';
                    break;
                case 'c':
                    position = 'b';
                    break;
                case 'd':
                    position = 'c';
                    break;
                case 'e':
                    position = 'd';
                    break;
                case 'f':
                    position = 'e';
                    break;
                case 'g':
                    position = 'f';
                    break;
            }
            direction = 'l';
        }

        if(eventIsSet(EV_TASTER_R)){
            clearEvent(EV_TASTER_R);
            cancelTimer(0);
            if(position != 'g'){
                    if(direction == 'r'){
                    drive_forward(10,FORWARD);
                }else if(direction == 'l'){
                    turn(CLOCKWISE,180);
                    drive_forward(10,FORWARD);
                }else if(direction == 'o'){
                    turn(CLOCKWISE,90);
                    drive_forward(10,FORWARD);
                }
            }
            startTimer(0); //reset timer


            switch(position){
                case 'a':
                    position = 'b';
                    break;
                case 'b':
                    position = 'c';
                    break;
                case 'c':
                    position = 'd';
                    break;
                case 'd':
                    position = 'e';
                    break;
                case 'e':
                    position = 'f';
                    break;
                case 'f':
                    position = 'g';
                    break;
            }


            direction = 'r';
        }

        if(eventIsSet(EV_RETURN)){
            clearEvent(EV_RETURN);
            switch(position){
                case 'a':
                    turn(CLOCKWISE,180);
                    drive_forward(30,FORWARD);
                    turn(ANTICLOCKWISE,90);
                    break;
                case 'b':
                    if(direction == 'l') turn(CLOCKWISE,180);
                    drive_forward(20,FORWARD);
                    turn(ANTICLOCKWISE,90);
                    break;
                case 'c':
                    if(direction == 'l') turn(CLOCKWISE,180);
                    drive_forward(10,FORWARD);
                    turn(ANTICLOCKWISE,90);
                    break;
                case 'd':
                    if(direction == 'r'){
                        turn(ANTICLOCKWISE,90);
                    }else if(direction = 'l'){
                        turn(CLOCKWISE,90);
                    }
                    break;
                case 'e':
                    if(direction == 'r') turn(CLOCKWISE,180);
                    drive_forward(10,FORWARD);
                    turn(CLOCKWISE,90);
                    break;
                case 'f':
                    if(direction == 'r') turn(CLOCKWISE,180);
                    drive_forward(20,FORWARD);
                    turn(CLOCKWISE,90);
                    break;
                case 'g':
                    turn(CLOCKWISE,180);
                    drive_forward(30,FORWARD);
                    turn(CLOCKWISE,90);
                    break;

            }
            direction = 'o';
            position = 'd';
            Delay(100);
        }

    }
}


int main(){

    //Tasks
    CreateAndStartTask(checkTimers);
    CreateAndStartTask(checkTasterL);
    CreateAndStartTask(checkTasterR);
    CreateAndStartTask(controlBot);
    //Start OS
    StartScheduler();
   	return 0;
}

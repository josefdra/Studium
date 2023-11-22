/*
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
    int zustand = 0;
    while(1){
        switch(zustand){
        case 0:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                } else if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 1:
                if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 2:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                } else if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 3:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                } else if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 4:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                } else if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 5:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                } else if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 6:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                } else if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 7:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 8:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                } else if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 9:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                } else if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 10:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                } else if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 11:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                } else if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;
        case 12:
                if(eventIsSet(EV_TASTER_L)){
                    clearEvent(EV_TASTER_L);
                    cancelTimer(0);

                    startTimer(0);
                } else if(eventIsSet(EV_TASTER_R)){
                    clearEvent(EV_TASTER_R);
                    cancelTimer(0);

                    startTimer(0);
                }
        break;

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

*/

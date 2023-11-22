#include "nnxt.h"
#include "timer.h"

//Global variables
volatile EventType events = 0;
volatile s_timer timers[5] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};


//Functions
void setEvent(EventType ev){
    taskENTER_CRITICAL();
        events = events | ev;
    taskEXIT_CRITICAL();
}

void clearEvent(EventType ev){
    taskENTER_CRITICAL();
        events = events & ~ev;
    taskEXIT_CRITICAL();
}

int eventIsSet(EventType ev){
    return (events & ev);
}

void setTimer(int timer, int time, EventType ev){
    timers[timer].time = time;
    timers[timer].ev_type = ev;
}

void startTimer(int timer){
    timers[timer].endTime = GetSysTime()+timers[timer].time;
}

void cancelTimer(int timer){
    timers[timer].endTime = 0;
}

//Tasks
void checkTimers(){
    while(1){
        uint32_t systime = GetSysTime();
        for(int i = 0; i<5; i++){
            if (timers[i].endTime == 0) continue;
            if (timers[i].endTime <= systime){
                setEvent(timers[i].ev_type);
                timers[i].endTime = 0;
            }
        }
        Delay(10);
    }
}

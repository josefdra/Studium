#include "nnxt.h"
#include "customMovement.h"
#include "stdio.h"

typedef int16_t EventType;

//Global variables
volatile EventType events = 0;

struct s_timer{
    EventType ev_type;
    int time;
};

void setEvent(EventType ev){
    events = events | ev;
}

void clearEvent(EventType ev){
    events = events & ~ev;
}

bool eventIsSet(EventType ev){
    if(events & ev == ev){
        return true;
    }else{
        return false;
    }
}

void setTimer(s_timer* timer, int time, EventType ev){
    timer = malloc(sizeof(s_timer));
    timer->time = time;
    timer->ev_type = ev;
}

void startTimer(TimerType timer){

}

void cancelTimer(s_timer* timer){
    free(timer);
}

int main(){

    s_timer* a;
    setTimer(a,1000,4);

    //Tasks
    setEvent(0b0000100000000000);
    setEvent(0b0000000000000001);
    printf("%b",events);
    //Start OS
    //StartScheduler();
   	return 0;
}

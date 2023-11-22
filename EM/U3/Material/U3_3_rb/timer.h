#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

typedef int16_t EventType;

typedef struct{
    EventType ev_type;
    int time;
    uint32_t endTime;
} s_timer;


void setEvent(EventType ev);
void clearEvent(EventType ev);
int eventIsSet(EventType ev);
void setTimer(int timer, int time, EventType ev);
void startTimer(int timer);
void cancelTimer(int timer);
void checkTimers();

#endif /* TIMER_H_INCLUDED */

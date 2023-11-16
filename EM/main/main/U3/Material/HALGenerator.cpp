#include <iostream>
#include<getopt.h>
#include<unistd.h>
using namespace std;


/** HALGenerator
    Optionen:
             -t    : schaltet Verwendung von Timer an
             -d %f : gibt die Dauer in ms an, Werte müssen im Intervall [0.004,10000.0] liegen
             -c    : schaltet Verwendung von Callbacks frei und Events ab
             -e    : schaltet Verwendung von Events frei und Callbacks ab
*/

int main(int argc, char *argv[])
{
    bool timerIsUsed = false;
    bool callback = false;
    unsigned int TCNT0_Value = 0;
    unsigned int Prescaler_Value = 0x03;
    unsigned int TimerDuration = 0;
    double duration_Value=0.0;
    FILE *TimerHAL_cfg_Handle = NULL;

    int opt;
    while((opt=getopt(argc,argv,"td:ce"))!=-1){
        switch (opt) {
            case 't' : timerIsUsed = true;
                       break;
            case 'd' : sscanf(optarg,"%lf",&duration_Value);
                       break;
            case 'c' : callback = true;
                       break;
            case 'e' : callback = false;
                       break;
            default:   break;
        }
    }
    TimerHAL_cfg_Handle = fopen("TimerHAL_cfg.h","w");
    fprintf(TimerHAL_cfg_Handle,"#ifndef TIMERHAL_CFG_H_\n#define TIMERHAL_CFG_H_\n\n");
    if (timerIsUsed){
        fprintf(TimerHAL_cfg_Handle,"#define TIMER_IS_USED\n");
        if (callback)
            fprintf(TimerHAL_cfg_Handle,"#define TIMER_USE_CALLBACK\n");
        else
            fprintf(TimerHAL_cfg_Handle,"#define TIMER_USE_EVENT\n");
        fprintf(TimerHAL_cfg_Handle,"#define PRESCALER_BITMASK %d\n",Prescaler_Value);
        if (duration_Value>=0.004 && duration_Value<=10000.0) {
            fprintf(TimerHAL_cfg_Handle,"#define TIMER_RESOLUTION_MS\n");
        }
        /** TCNT und TIMER_LIMIT ausrechnen. So, dass TCNT möglichst groß ist
            Algorithmus: gehe vom Maximalwert für TCNT0 aus und nimm ersten Wert, der Teiler von
                         den Anzahl der Ticks der gegebenen Dauer des Timers ist
                         Berechne anschließend TCNT0 über die Formel 256-ticks
                         Berechne anschließend den Wert für den Counter mittels Anzahl der Ticks für
                           die angegebene Dauer durch ticks
        */
        unsigned int duration_ticks = (unsigned long)(duration_Value*1000)/4;
        int ticks = 255;
        for(ticks=255;ticks>=0;ticks--){
            if (duration_ticks % ticks == 0)
                break;
        }
        TCNT0_Value = 256-ticks;
        TimerDuration = duration_ticks/ticks;
        fprintf(TimerHAL_cfg_Handle,"#define TIMER_COUNTER_START_VALUE %d\n",TCNT0_Value);
        fprintf(TimerHAL_cfg_Handle,"#define TIMER_LIMIT %d\n",TimerDuration);
    }
    fprintf(TimerHAL_cfg_Handle,"#endif\n");
    fclose(TimerHAL_cfg_Handle);

    return 0;
}

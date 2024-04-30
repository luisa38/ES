#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>

enum Modos{manual=1, scheduled};

class power_data{

    public:

        int minute;
        int hour;
        int minute_dur;
        int hour_dur;
        float power_W;
        float power_cost;
        float custo_kWh;
        Modos modo;
};

/*struct SmartPlug_SM : public stateMachine {
    typedef enum SmartPlug_state_t{
        ON,
        OFF
    } SmartPlug_state_t;
};*/


//also missing control variables
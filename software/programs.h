#ifndef PROGRAMS_H
#define PROGRAMS_H

void setup();
void loop();
void update_state(int input);
void handle_program(price_data& price_data, power_data& power_data);

enum plug_state {
  OFF,
  ON,
  STANDBY,
  ERROR
};

enum program{
    program0,
    program1,
    program2,
    program3,
    program4,
    program5,
    program6,
    program7,
};

enum Modos{manual=1, scheduled};

enum Regimes{normal=1, bi};

class price_data{
    
    public:

        Regimes regime;
        float price_1;
        float price_2;
        float price_difference;
        float best_price;
        float worst_price;
        int best_schedule;
        
};

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

#endif //PROGRAMS_H
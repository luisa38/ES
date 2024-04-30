#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>

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

//missing control variables
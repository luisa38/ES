#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <ctime>
#include "consumo.h"
#include "eficiencia.h"
#include "programs.h"

int main(){

    price_data pessoa;
    int guarda_regime;
    power_data dados;
    int guarda_modo;
    //smartplug smarplug_sm;
    plug plug1;
    state_machine m1;
    m1.state = 0;

    std::cout << "\nSeleciona o tipo de modo utilizado, colocando 1 para regime manual e 2 para marcar horário: ";
    std::cin >> guarda_modo;

    std::cout << "Seleciona o tipo de regime utilizado, colocando 1 para regime normal e 2 para bi-diário:";
    std::cin >> guarda_regime;

    if(guarda_modo==1)
        dados.modo=manual;

    else if(guarda_modo==2)
        dados.modo=scheduled;

    else
    {
        std::cout << "\nModo Inválido\n";
        return 0;
    }

    if(guarda_regime==1)
        pessoa.regime=normal;

    else if(guarda_regime==2)
        pessoa.regime=bi;
    
    else
    {
        std::cout << "\nRegime Inválido\n";
        return 0;
    }

    if (pessoa.regime==normal)
    {
        std::cout << "\nQual é o preço em euros por kWh:";
        std::cin >> pessoa.price_1;
        dados.custo_kWh = pessoa.price_1;
        pessoa.price_2=100;
    }
    
    if (pessoa.regime==bi)
    {
        std::cout << "\nQual é o preço €/kWh do primeiro horário:";
        std::cin >> pessoa.price_1;
        std::cout << "\nQual é o preço em €/kWh do segundo horário:";
        std::cin >> pessoa.price_2;
        
        if (pessoa.price_1 > pessoa.price_2)
        {
            pessoa.price_difference = pessoa.price_1 - pessoa.price_2;
            pessoa.best_price = pessoa.price_2;
            pessoa.best_schedule = 2;
        }
        
        else
        {
            pessoa.price_difference = pessoa.price_2 - pessoa.price_1;
            pessoa.best_price = pessoa.price_1;
            pessoa.best_schedule = 1;
        }

        dados.custo_kWh = pessoa.best_price;
               
        std::cout << "\nA diferença de preço ente horários é: ";
        std::cout << pessoa.price_difference << " €/kWh";
        std::cout << "\nO melhor preço é: ";
        std::cout << pessoa.best_price << " €/kWh";
        std::cout << "\nO melhor horário é: ";
        std::cout << pessoa.best_schedule << "\n";
    }

    if (dados.modo==scheduled)
    {
        std::cout << "\nSeleciona as horas: ";
        std::cin >> dados.hour;
        std::cout << "\nSeleciona os minutos: ";
        std::cin >> dados.minute;
        std::cout << "\nO seu horário é: " << dados.hour << ":" << dados.minute << "\n";
        std::cout << "\nSeleciona as horas de duração: ";
        std::cin >> dados.hour_dur;
        std::cout << "\nSeleciona os minutos de duração: ";
        std::cin >> dados.minute_dur;
        std::cout << "\nO tempo de duração é: " << dados.hour_dur << ":" << dados.minute_dur << "\n";
        std::cout << "\nQual é o consumo em Watt: ";
        std::cin >> dados.power_W;
        dados.power_cost = dados.power_W/1000* (float(dados.hour_dur) + float(dados.minute_dur)/60.0) * dados.custo_kWh;
        printf("\nO custo total foi aproximadamente: %.2f\n", dados.power_cost);
    }
}
#include "eficiencia.h"

int main(){
    
    price_data pessoa;
    int guarda_regime;

    std::cout << "Seleciona o tipo de regime utilizado, colocando 1 para regime normal e 2 para bi-diário:";
    std::cin >> guarda_regime;

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
               
        std::cout << "\nA diferença de preço ente horários é: ";
        std::cout << pessoa.price_difference << " €/kWh";
        std::cout << "\nO melhor preço é: ";
        std::cout << pessoa.best_price << " €/kWh";
        std::cout << "\nO melhor horário é: ";
        std::cout << pessoa.best_schedule << "\n";
    }

    
}
#include "consumo.h"
#include "eficiencia.h"

int main(){

    power_data dados;
    int guarda_modo;

    std::cout << "\nSeleciona o tipo de modo utilizado, colocando 1 para regime manual e 2 para marcar horário: ";
    std::cin >> guarda_modo;

    if(guarda_modo==1)
        dados.modo=manual;

    else if(guarda_modo==2)
        dados.modo=scheduled;

    else
    {
        std::cout << "\nModo Inválido\n";
        return 0;
    }

    if (dados.modo==scheduled)
    {
        std::cout << "\nSeleciona as horas: ";
        std::cin >> dados.hour;
        std::cout << "\nSeleciona os minutos: ";
        std::cin >> dados.minute;
        std::cout << "\nO seu horário é: " << dados.hour << ":" << dados.minute << "\n";  
    }
    
}
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ACS712.h>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <ctime>
#include "programs.h"


volatile int STOP=false;

uint8_t website_inputs;
uint8_t output;

/*
for inputs:
-3 = ERROR (plugged off etc)
-2 = program interrupted (manually switched off)
-1 = just finished scheduled program
0 = connected to power and wifi is on /PLUG IS OFF/program0
1 = program1
2 = program2
3 = program3
4 = program4
5 = program5
6 =
7 =
*/

//Functions for assigning input variables:
//will lose viability when parsing from database
price_data setup_price_data(){
  price_data pessoa;
  int guarda_regime;

  std::cout << "Seleciona o tipo de regime utilizado, colocando 1 para regime normal e 2 para bi-diário:";
  std::cin >> guarda_regime;

  if(guarda_regime==1)
    pessoa.regime=simples;

  else if(guarda_regime==2)
    pessoa.regime=bi;
    
  else
  {
    std::cout << "\nRegime Inválido\n";
    return 0;
  }

  if (pessoa.regime==simples)
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

  return pessoa;
}

float setup_power_data(){
  power_data dados;
  int guarda_modo;

  std::cout << "\nSeleciona o tipo de modo utilizado, colocando 1 para regime manual e 2 para marcar horário: ";
  std::cin >> guarda_modo;

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

  return dados;
}


// Initialize hardware and setup
void setup() {
  pinMode(output_pin, OUTPUT);

  Serial.begin(115200);
  plug_state plug0_state = OFF;
  program plug0_program = program0;
}

void loop() {
  // Read inputs or events
  setup_power_data();
  setup_price_data();
  int input; //variable to receive database input for each program, int for now for debugging purposes
  std::cout << "Input?\n";
  std::cin >> input;
  // Update state machine based on inputs/events
  update_state(input);
  handle_program(pessoa, dados);
}

//Function to update state machine
void update_state(int input) {
  switch (plug0_state) {
    case OFF:
      if (input == 0) {
        plug0_state = STANDBY;
        plug0_program = program0;
        printf("plug is on standby\n");
      }
      break;
    case ON:
      if (input == -1) {
        plug0_state = OFF;
        plug0_program = program0;
        printf("plug is off, program finished\n");
      }
      if (input == -2) {
        plug0_state = OFF;
        plug0_program = program0;
        printf("plug is off, program interrupted\n");
      }
      if (input == -3) {
        plug0_state = ERROR;
        printf("plug is off, error\n");
      }
      break;
    case STANDBY:
      if (input == 1) {
        plug0_state = ON;
        plug0_program = program1;
        printf("plug has turned ON\n");
      }
      if (input == 2) {
        plug0_state = ON;
        plug0_program = program2;
        printf("plug has turned ON\n");
      }
      if (input == 3) {
        plug0_state = ON;
        plug0_program = program3;
        printf("plug has turned ON\n");
      }
      if (input == 4) {
        plug0_state = ON;
        plug0_program = program4;
        printf("plug has turned ON\n");
      }
      if (input == 5) {
        plug0_state = ON;
        plug0_program = program5;
        printf("plug has turned ON\n");
      }
      if (input == 6) {
        plug0_state = ON;
        plug0_program = program6;
        printf("plug has turned ON\n");
      }
      if (input == 7) {
        plug0_state = ON;
        plug0_program = program7;
        printf("plug has turned ON\n");
      }
      break;
    case ERROR:
      if (input == -1) {
        plug0_state = OFF;
        printf("plug is off\n");
      }
      break;
  }
}

// Function to handle specific functionalities for each program
// for now each program only turns plug ON according to conditions, later we might want to add variables to save the consumption/eficiency output
void handle_program(price_data& price_data, power_data& power_data) {
  switch (plug0_program) {
    case program0:
      //default, does nothing (plug is OFF)
      output = 0;
      digitalWrite(output_pin, output);//update PINmode
      break;
    case program1:
      while (plug0_state == 1){ //ON manually
        output = 1;
        digitalWrite(output_pin, output);
      }
      break;
    case program2:
        while (plug0_state == 1){ 
            while(price_data.best_schedule == 1){ //ON according to schedule, should be bool -> while condition true
              output = 1;
              digitalWrite(output_pin, output);
            }
            plug0_state = OFF;
        }
      break;
    case program3:
        while(plug0_state == 1){
          while(price_data.current_price >= set_price){
            output = 1;
            digitalWrite(output_pin, output);
          }
          plug0_state = OFF;
        }
      break;
    case program4:
        while(plug0_state == 1){
          while(price_data.combined_price >= set_comb_price){
            output = 1;
            digitalWrite(output_pin, output);
          }
          plug0_state = OFF;
        }
      break;
    case program5:
        while(plug0_state == 1){
          while(price_data.power_W < set_consumption){
            output = 1;
            digitalWrite(output_pin, output);
          }
          plug0_state = OFF;
        }
      break;
    /*case program6:
      
      break;*/
  }
}
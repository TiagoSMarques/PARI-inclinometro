//bootloader speed 57600
#include <LiquidCrystal.h>
#include <math.h>

//Definir entradas do LCD
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

//Definir A0,A1,A2,A3 como entradas analogicas
float sensor1Pin = A0;
float sensor2Pin = A1;
float sensor3Pin = A2;
float sensor4Pin = A3;

//Definir D8 como entrada digital
int cali = 8;

//Definir variavel de leitura de distÃ¢ncia
float sensor1Val = 0;
float sensor2Val = 0;
float sensor3Val = 0;
float sensor4Val = 0;

//Definir variavel de leitura de distÃ¢ncia
float sensor1Vall = 0;
float sensor2Vall = 0;
float sensor3Vall = 0;
float sensor4Vall = 0;

//Definir variavel de leitura de distÃ¢ncia
float sensor1Vallc = 0;
float sensor2Vallc = 0;
float sensor3Vallc = 0;
float sensor4Vallc = 0;

//Definir variavel de leitura de distÃ¢ncia
float sensor1Vallcc = 0;
float sensor2Vallcc = 0;
float sensor3Vallcc = 0;
float sensor4Vallcc = 0;

//Valor maximo gama ADC
float declive1 = 0;
float declive2 = 0;
float declive3 = 0;
float declive4 = 0;

//Definir variavel intermÃ©dio para calibraÃ§Ã£o
float distance1 = 0;
float distance2 = 0;
float distance3 = 0;
float distance4 = 0;

//Definir variavel para valor de distancia apÃ³s calibraÃ§Ã£o
float distance1_cut = 0;
float distance2_cut = 0;
float distance3_cut = 0;
float distance4_cut = 0;

//Definir variavais para o pitch e o roll
float pitch_1 = 0;
float pitch_2 = 0;
float roll_1 = 0;
float roll_2 = 0;
float pitch = 0;
float roll = 0;

//Definir distancia entre sensores
float dist_1_3 = 3375;
float dist_2_4 = 3375;
float dist_1_2 = 1055;
float dist_3_4 = 875;

//Definir variavel de calibraÃ§Ã£o
int calival = 0;

//Definir varavel de reÃ§esÃ£o bluetooth

int Received=0;

//Contador para envio dados app mit

int counter=0;
int counter1=0;


//**************************************************************************************
void setup() {

  Serial.begin(9600);

  lcd.begin(16, 2);

  lcd.print("Inclinometro");
  lcd.setCursor(0, 1);
  lcd.print("Proj. Automacao");
  delay(2000);
  lcd.clear();

  pinMode(cali, INPUT);

}
//**************************************************************************************
void loop() {

//recesÃ£o da ordem de calibraÃ§Ã£o por bluetooth
   if(Serial.available()>0) //send data only when you recive data
 { 
    Received = Serial.read(); //arrumar o caracter que chegou
    //-.-...........................................................................................................................................................................................................................................................................               Serial.print("Char recebido: ");
    //Serial.println(Received);
 }

  //Ler e guardar valor da entrada digital
  calival = digitalRead(cali);

  

  //Ler e guardar valor das entradas analogicas
  sensor1Val = analogRead(sensor1Pin);
  sensor2Val = analogRead(sensor2Pin);
  sensor3Val = analogRead(sensor3Pin);
  sensor4Val = analogRead(sensor4Pin);

  sensor1Vallc = sensor1Vallc + sensor1Val;
  sensor2Vallc = sensor2Vallc + sensor2Val;
  sensor3Vallc = sensor3Vallc + sensor3Val;
  sensor4Vallc = sensor4Vallc + sensor4Val;

  counter1 = counter1 + 1;

  //if (counter1 == 6)
 // {
  sensor1Vallcc = sensor1Vallc /counter1;
  sensor2Vallcc = sensor2Vallc /counter1;
  sensor3Vallcc = sensor3Vallc /counter1;
  sensor4Vallcc = sensor4Vallc /counter1;

  sensor1Vallc = 0;
  sensor2Vallc = 0;
  sensor3Vallc = 0;
  sensor4Vallc = 0;



  sensor1Vall = sensor1Val * 0.977520;
  sensor2Vall = sensor2Val * 0.977520;
  sensor3Vall = sensor3Val * 0.977520;
  sensor4Vall = sensor4Val * 0.977520;

  sensor1Vallcc = 0;
  sensor2Vallcc = 0;
  sensor3Vallcc = 0;
  sensor4Vallcc = 0;

  sensor1Vall = (0.613800 * sensor1Vall) + 75.801000;
  sensor2Vall = (0.615400 * sensor2Vall) + 75.692000;
  sensor3Vall = (0.613600 * sensor3Vall) + 75.042000;
  sensor4Vall = (0.616300 * sensor4Vall) + 75.373000;

   // counter1 = 0;
   // }

  //IntroduÃ§Ã£o do valor de calibraÃ§Ã£o****
  if (calival == 1 || Received == 'C')
  {
    distance1 = sensor1Vall;
    distance2 = sensor2Vall;
    distance3 = sensor3Vall;
    distance4 = sensor4Vall;

    Received = 0;
  }

  //Efectuar calibraÃ§Ã£o******************
  distance1_cut = sensor1Vall - distance1;
  distance2_cut = sensor2Vall - distance2;
  distance3_cut = sensor3Vall - distance3;
  distance4_cut = sensor4Vall - distance4;

  //calculo do pich e do roll

  //InclinaÃ§Ã£o para a frente***********************************
  if ((distance1_cut <= 0) && (distance3_cut >= 0))
  {
    pitch_1 = (asin((-distance1_cut + distance3_cut) / dist_1_3))*57.295700;
  }
  //InclinaÃ§Ã£o para a trÃ¡s
  else
  {
    pitch_1 = (asin((-distance1_cut + distance3_cut) / dist_1_3))*57.295700;
  }

  //InclinaÃ§Ã£o para a frente**********************************
  if ((distance2_cut <= 0) && (distance4_cut >= 0))
  {
    pitch_2 = (asin((-distance2_cut + distance4_cut) / dist_2_4))*57.295700;
  }
  //InclinaÃ§Ã£o para a trÃ¡s
  else
  {
    pitch_2 = (asin((-distance2_cut + distance4_cut) / dist_2_4))*57.295700;
  }

  //InclinaÃ§Ã£o para a direita********************************
  if ((distance1_cut <= 0) && (distance2_cut >= 0))
  {
    roll_1 = (asin((-distance1_cut + distance2_cut) / dist_1_2))*57.295700;
  }
  //InclinaÃ§Ã£o para a esquerda
  else
  {
    roll_1 = (asin((-distance1_cut + distance2_cut) / dist_1_2))*57.295700;
  }

  //InclinaÃ§Ã£o para a direita********************************
  if ((distance3_cut <= 0) && (distance4_cut >= 0))
  {
    roll_2 = (asin((-distance3_cut + distance4_cut) / dist_3_4))*57.295700;
  }
  //InclinaÃ§Ã£o para a esquerda
  else
  {
    roll_2 = (asin((-distance3_cut + distance4_cut) / dist_3_4))*57.295700;
  }
//Calculo do Pitch********************************************************

pitch=(pitch_1+pitch_2)/2;

roll=(roll_1+roll_2)/2;

  //Print no serial monitor**************
 // Serial.print("Pitch = ");
 // Serial.println(pitch);

 // Serial.print("Roll = ");
 // Serial.println(roll);


  //Print mit app**************
 // counter = counter + 1;
 // if (counter = 40)
 // {
 // Serial.print(pitch, 2);
 // Serial.print("%");
 // Serial.println(roll, 2);
 // counter = 0;
 // }


 Serial.print(sensor1Val, 5);
 Serial.print('\t');
 Serial.print(sensor1Vall, 5);
 Serial.print('\t');
 Serial.print(sensor2Val, 5);
 Serial.print('\t');
 Serial.print(sensor2Vall, 5);
 Serial.print('\t');
 Serial.print(sensor3Val, 5);
 Serial.print('\t');
 Serial.print(sensor3Vall, 5);
 Serial.print('\t');
 Serial.print(sensor4Val, 5);
 Serial.print('\t');
 Serial.print(sensor4Vall, 5);
 Serial.print('\t');
 Serial.print(pitch, 5);
 Serial.print('\t');
 Serial.print(roll, 5);
 Serial.print('\n');



  lcd.clear();
  lcd.setCursor(0, 0);
  //*************************
  //**************************
  lcd.print("Pitch:");
  lcd.print(pitch, 5);
//****************************
  lcd.setCursor(0, 1);
//****************************
  lcd.print("Roll:");
  lcd.print(roll, 5);

  delay(5);
}

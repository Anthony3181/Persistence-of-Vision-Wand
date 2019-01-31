



void setup(void){
  Serial.begin(9600);

}


int pot_pin;
int display_pin;


float Vin = 0; //voltage value to be read from divider
float value_out = 0; //value to be passed to display

void loop(){

  pot_pin = analogRead(/*pin number */);
  display_pin = analogWrite(

}

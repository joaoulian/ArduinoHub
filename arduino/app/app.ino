const int inputPlaca0[4] = {0, 1, 2, 3}; //endereco(mais significativo) endereco dado(mais significativo) dado
// o input mostra o dado que 
const int outputPlaca0[4] = {24, 26};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(inputPlaca0[0], INPUT);
  pinMode(inputPlaca0[1], INPUT);
  pinMode(inputPlaca0[2], INPUT);
  pinMode(inputPlaca0[3], INPUT);

  pinMode(outputPlaca0[0], OUTPUT);
  pinMode(outputPlaca0[1], OUTPUT);
  
  analogReference(INTERNAL1V1);
  //Se estiver usando Arduino Mega, use INTERNAL1V1
  //se estiver usando Arduino Leonardo, remova esta linha pois o Leonardo não aceita
  //este comando
}


void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(inputPlaca0[3]));
  Serial.print(" ");
  Serial.print(analogRead(inputPlaca0[2]));
  Serial.print(" ");
  Serial.print(analogRead(inputPlaca0[1]));
  Serial.print(" ");
  Serial.println(analogRead(inputPlaca0[0]));

  digitalWrite(outputPlaca0[0], LOW);
  digitalWrite(outputPlaca0[1], LOW);
  
}

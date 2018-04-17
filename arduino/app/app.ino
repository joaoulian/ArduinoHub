const int input[4] = {0, 1, 2, 3};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(input[0], INPUT);
  pinMode(input[1], INPUT);
  pinMode(input[2], INPUT);
  pinMode(input[3], INPUT);

  analogReference(INTERNAL1V1);
  //Se estiver usando Arduino Mega, use INTERNAL1V1
  //se estiver usando Arduino Leonardo, remova esta linha pois o Leonardo não aceita
  //este comando
}


void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(input[3]));
  Serial.print(" ");
  Serial.print(analogRead(input[2]));
  Serial.print(" ");
  Serial.print(analogRead(input[1]));
  Serial.print(" ");
  Serial.println(analogRead(input[0]));
}

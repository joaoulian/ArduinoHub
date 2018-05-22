const int inputPinClient0[4] = {0, 1, 2, 3}; // address[3:2], data[1:0]
const int outputPinClient0[2] = {24, 26};
const int posQueue = 0;


typedef struct {
  int receiver[2];
  int sender[2];
  int data[2];
  int status; // 0: req concluida, 1: aguardando envio, 2: enviado, aguardando confirmacao 
} package;

package queue[4]; // as posicoes representam os senders


void setup() {
  Serial.begin(9600);

  /* client 0 pins setup */
  pinMode(inputPinClient0[0], INPUT);
  pinMode(inputPinClient0[1], INPUT);
  pinMode(inputPinClient0[2], INPUT);
  pinMode(inputPinClient0[3], INPUT);

  pinMode(outputPinClient0[0], OUTPUT);
  pinMode(outputPinClient0[1], OUTPUT);

  analogReference(INTERNAL1V1); // setup for using analog ports
}


void loop() {
  /*
  Serial.print(analogRead(inputPinClient0[3]));
  Serial.print(" ");
  Serial.print(analogRead(inputPinClient0[2]));
  Serial.print(" ");
  Serial.print(analogRead(inputPinClient0[1]));
  Serial.print(" ");
  Serial.println(analogRead(inputPinClient0[0]));*/

  if (queue[posQueue].status == 0) {
      //todo: confirmacao de recebimento
      
      //envio de dados
      queue[posQueue].data[1] = inputPinClient0[1];
      queue[posQueue].data[0] = inputPinClient0[0];

      queue[posQueue].receiver[1] = inputPinClient0[3];
      queue[posQueue].receiver[0] = inputPinClient0[2];

      queue[posQueue].sender[1] = 0;
      queue[posQueue].sender[0] = 0;
      queue[posQueue].status = 1;
  }

  if (queue[posQueue].status == 1) {
    if (analogRead(queue[posQueue].data[0]) >= 800)
      digitalWrite(outputPinClient0[0], HIGH);
    else
      digitalWrite(outputPinClient0[0], LOW);
    if (analogRead(queue[posQueue].data[1]) >= 800)
      digitalWrite(outputPinClient0[1], HIGH);
    else
      digitalWrite(outputPinClient0[1], LOW);
      
    queue[posQueue].status = 2;
  }

  if (queue[posQueue].status == 2) {
    if (inputPinClient0[1] == queue[posQueue].data[1] && inputPinClient0[0] == queue[posQueue].data[0]){
      Serial.print(" ");
      Serial.print(analogRead(inputPinClient0[0]));
      Serial.print(" ");
      Serial.println(analogRead(inputPinClient0[1]));
      queue[posQueue].status = 0;
      //posQueue++;
    }
  }
  
  



}

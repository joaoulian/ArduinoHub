const int inputPinClient0[4] = {0, 1, 2, 3}; // address[3:2], data[1:0]
const int outputPinClient0[4] = {24, 26};

typedef struct {
  int receiver[2];
  int sender[2];
  int data[2];
  int status;
} package;

package queue[4];


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
  Serial.print(analogRead(inputPinClient0[3]));
  Serial.print(" ");
  Serial.print(analogRead(inputPinClient0[2]));
  Serial.print(" ");
  Serial.print(analogRead(inputPinClient0[1]));
  Serial.print(" ");
  Serial.println(analogRead(inputPinClient0[0]));

  if (inputPinClient0[3] != 0 && inputPinClient0[2] != 0 && inputPinClient0[1] != 0 && inputPinClient0[0] != 0) {
      queue[0].data[1] = inputPinClient0[1];
      queue[0].data[0] = inputPinClient0[0];

      queue[0].receiver[1] = inputPinClient0[3];
      queue[0].receiver[0] = inputPinClient0[2];

      queue[0].sender[1] = 0;
      queue[0].sender[0] = 0;

      queue[0].status = 1;
  } else {
      queue[0].status = 0;
  }

  digitalWrite(outputPinClient0[0], LOW);
  digitalWrite(outputPinClient0[1], LOW);

}

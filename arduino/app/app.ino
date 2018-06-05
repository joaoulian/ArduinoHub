//inicializacao da fpga: o endereço de envio de dado deve ser igual ao endereço dado para a placa
//o codigo parte do pressuposto que se o valor do endereço for igual o da placa, então não está enviando dado

const int inputPinClient0[4] = {0, 1, 2, 3}; // data[1:0] address[3:2]
const int outputPinClient0[2] = {13, 12};

const int inputPinClient1[4] = {0, 1, 2, 3}; // data[1:0] address[3:2]
const int outputPinClient1[2] = {13, 12};

const int inputPinClient2[4] = {0, 1, 2, 3}; // data[1:0] address[3:2]
const int outputPinClient2[2] = {13, 12};

const int inputPinClient3[4] = {0, 1, 2, 3}; // data[1:0] address[3:2]
const int outputPinClient3[2] = {13, 12};

const int posQueue = 0;


typedef struct {
  int receiver[2]; //quem vai receber
  int sender[2]; //quem esta enviando
  int data[2]; //dado
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

  pinMode(inputPinClient1[0], INPUT);
  pinMode(inputPinClient1[1], INPUT);
  pinMode(inputPinClient1[2], INPUT);
  pinMode(inputPinClient1[3], INPUT);

  pinMode(inputPinClient2[0], INPUT);
  pinMode(inputPinClient2[1], INPUT);
  pinMode(inputPinClient2[2], INPUT);
  pinMode(inputPinClient2[3], INPUT);

  pinMode(inputPinClient3[0], INPUT);
  pinMode(inputPinClient3[1], INPUT);
  pinMode(inputPinClient3[2], INPUT);
  pinMode(inputPinClient3[3], INPUT);

  pinMode(outputPinClient0[0], OUTPUT);
  pinMode(outputPinClient0[1], OUTPUT);

  pinMode(outputPinClient1[0], OUTPUT);
  pinMode(outputPinClient1[1], OUTPUT);

  pinMode(outputPinClient2[0], OUTPUT);
  pinMode(outputPinClient2[1], OUTPUT);

  pinMode(outputPinClient3[0], OUTPUT);
  pinMode(outputPinClient3[1], OUTPUT);

  analogReference(INTERNAL); // setup for using analog ports
}


void loop() {
  if (queue[posQueue].status == 0) {
      //todo: confirmacao de recebimento
      // pegando o dado e colocando na fila para enviar

      

      //verificar se algum dado está sendo enviado
      //verificando placa 0
      int isSender = 0;
      int data0, data1, receiver0, receiver1, sender0, sender1;
      if(convertNumber(inputPinClient0[0],inputPinClient0[1]) != 0){
        //a placa 0 está enviando um dado
        isSender = 1;
        data0 = inputPinClient0[0];
        data1 = inputPinClient0[1];
        receiver0 = inputPinClient0[2];
        receiver1 = inputPinClient0[3];
        sender0 = 0;
        sender1 = 0;
      }
      else if(convertNumber(inputPinClient1[0],inputPinClient1[1]) != 1){
        //a placa 1 está enviando um dado
        isSender = 1;
        data0 = inputPinClient1[0];
        data1 = inputPinClient1[1];
        receiver0 = inputPinClient1[2];
        receiver1 = inputPinClient1[3];
        sender0 = 1;
        sender1 = 0;
      }

      else if(convertNumber(inputPinClient1[0],inputPinClient1[1]) != 2){
        //a placa 1 está enviando um dado
        isSender = 1;
        data0 = inputPinClient2[0];
        data1 = inputPinClient2[1];
        receiver0 = inputPinClient2[2];
        receiver1 = inputPinClient2[3];
        sender0 = 1;
        sender1 = 0;
      }

      else if(convertNumber(inputPinClient1[0],inputPinClient1[1]) != 3){
        //a placa 1 está enviando um dado
        isSender = 1;
        data0 = inputPinClient3[0];
        data1 = inputPinClient3[1];
        receiver0 = inputPinClient3[2];
        receiver1 = inputPinClient3[3];
        sender0 = 1;
        sender1 = 0;
      } 

      if(isSender == 1){

        Serial.print(" ");
        Serial.print(analogRead(data0));
        Serial.print(" ");
        Serial.println(analogRead(data1));
        
        //envio de dados
        queue[posQueue].data[0] = data0;
        queue[posQueue].data[1] = data1;
  
        queue[posQueue].receiver[0] = receiver0;
        queue[posQueue].receiver[1] = receiver1;
  
        //setando que a placa zero esta enviando
        queue[posQueue].sender[1] = sender0;
        queue[posQueue].sender[0] = sender1;
  
        //alterando o status da fila
        queue[posQueue].status = 1;
      }
      
      
  }

  if (queue[posQueue].status == 1) { //enviando o dado
    int addressNumber = convertNumber(queue[posQueue].receiver[0], queue[posQueue].receiver[1]);
    int auxOutput0 = outputPinClient0[0];
    int auxOutput1 = outputPinClient0[1];
    if(addressNumber == 1){
      auxOutput0 = outputPinClient1[0];
      auxOutput1 = outputPinClient1[1];
    }
    else if(addressNumber == 2){
      auxOutput0 = outputPinClient2[0];
      auxOutput1 = outputPinClient2[1];
    }
    else if(addressNumber == 3){
      auxOutput0 = outputPinClient3[0];
      auxOutput1 = outputPinClient3[1];
    }

    //logica para enviar o dado
    if (analogRead(queue[posQueue].data[0]) >= 800)
      digitalWrite(auxOutput0, HIGH);
    else
      digitalWrite(auxOutput0, LOW);
    if (analogRead(queue[posQueue].data[1]) >= 800)
      digitalWrite(auxOutput1, HIGH);
    else
      digitalWrite(auxOutput1, LOW);
      
    queue[posQueue].status = 2;
  }

  if (queue[posQueue].status == 2) {
    if (inputPinClient0[1] == queue[posQueue].data[1] && inputPinClient0[0] == queue[posQueue].data[0]){
      Serial.print(" ");
      Serial.print(analogRead(inputPinClient0[1]));
      Serial.print(" ");
      Serial.println(analogRead(inputPinClient0[0]));
      queue[posQueue].status = 0;
      //posQueue++;
    }
  }
}

/*
This function convert binary to decimal
*/
int convertNumber(int pos0, int pos1){
  int address = 0;
  if(pos0 >= 800)
    address = address + 1;
  if(pos1 >= 800)
    address = address + 2;
  return address;
}

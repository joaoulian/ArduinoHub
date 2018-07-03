//inicializacao da fpga: o endereço de envio de dado deve ser igual ao endereço dado para a placa
//o codigo parte do pressuposto que se o valor do endereço for igual o da placa, então não está enviando dado
//as confirmaçoes são todas endereço

const int inputPinClient1[4] = {0, 1, 2, 3}; // data[1:0] address[3:2]
const int outputPinClient1[2] = {22, 24};

const int inputPinClient2[4] = {4, 5, 6, 7}; // data[1:0] address[3:2]
const int outputPinClient2[2] = {26, 28};

const int inputPinClient3[4] = {0, 1, 2, 3}; // data[1:0] address[3:2]
const int outputPinClient3[2] = {13, 12};

const int posQueue = 0;

int contador = 0;


typedef struct {
  int receiver[2]; //quem vai receber
  int sender; //quem esta enviando
  int data[2]; //dado
  int status; // 0: req concluida, 1: aguardando envio, 2: enviado, aguardando confirmacao
} package;

package queue[4]; // as posicoes representam os senders


void setup() {
  Serial.begin(9600);

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

  pinMode(outputPinClient1[0], OUTPUT);
  pinMode(outputPinClient1[1], OUTPUT);

  pinMode(outputPinClient2[0], OUTPUT);
  pinMode(outputPinClient2[1], OUTPUT);

  pinMode(outputPinClient3[0], OUTPUT);
  pinMode(outputPinClient3[1], OUTPUT);

  analogReference(INTERNAL1V1); // setup for using analog ports
  queue[posQueue].status = 0;
}


void loop() {

  
  if (queue[posQueue].status == 2){
  }
  //Serial.println(queue[posQueue].status);
  if (queue[posQueue].status == 0) {

      contador = 0;
      // pegando o dado e colocando na fila para enviar
      //verificar se algum dado está sendo enviado

      int isSender = 0;
      isSender = verificaEnvio(inputPinClient1, outputPinClient1, 1);
      if(isSender == 0){
        isSender = verificaEnvio(inputPinClient2, outputPinClient2, 2);
      }

  }

  if (queue[posQueue].status == 1) { //enviando o dado
    contador = 0;
    int addressNumber = convertNumber(queue[posQueue].receiver[0], queue[posQueue].receiver[1]);
    enviaDadoParaPlaca(addressNumber);
  }

  if (queue[posQueue].status == 2) {
    //todo: confirmacao de recebimento



    if(contador >= 5){
        queue[posQueue].status = 1;
        contador = 0;
    }

    contador++;
    verificaConfirmacao(convertNumber(queue[posQueue].receiver[0], queue[posQueue].receiver[1]));
  }

}

void verificaConfirmacao(int endereco){
  int auxPin [4];
  if(endereco == 1){
    auxPin[0] = inputPinClient1[0];
    auxPin[1] = inputPinClient1[1];
    auxPin[2] = inputPinClient1[2];
    auxPin[3] = inputPinClient1[3];
  }
  else if(endereco == 2){
    auxPin[0] = inputPinClient2[0];
    auxPin[1] = inputPinClient2[1];
    auxPin[2] = inputPinClient2[2];
    auxPin[3] = inputPinClient2[3];
  }
  else if(endereco == 3){
    auxPin[0] = inputPinClient3[0];
    auxPin[1] = inputPinClient3[1];
    auxPin[2] = inputPinClient3[2];
    auxPin[3] = inputPinClient3[3];
  }

  if(convertNumber(analogRead(auxPin[2]),analogRead(auxPin[3])) == endereco){
    Serial.print("DADO CONFIRMADO COM SUCESSO -> ");
    Serial.println(endereco);
    resetStatus();
  }
}

void enviaDadoParaPlaca(int endereco){

  int auxOutput0 = outputPinClient1[0];
  int auxOutput1 = outputPinClient1[1];

  if(endereco == 2){
    auxOutput0 = outputPinClient2[0];
    auxOutput1 = outputPinClient2[1];
  }
  else if(endereco == 3){
    auxOutput0 = outputPinClient3[0];
    auxOutput1 = outputPinClient3[1];
  }
  
  Serial.print("PLACA A RECEBER O DADO:");
  Serial.print(endereco);
  Serial.print(" -> ");
  Serial.println(convertNumber(queue[posQueue].data[0],queue[posQueue].data[1]));
  
  //logica para enviar o dado
  if (queue[posQueue].data[0] >= 950)
    digitalWrite(auxOutput0, HIGH);
  else
    digitalWrite(auxOutput0, LOW);
  if (queue[posQueue].data[1] >= 950)
    digitalWrite(auxOutput1, HIGH);
  else
    digitalWrite(auxOutput1, LOW);

  queue[posQueue].status = 2;
}



void decimalBinarioEscritor(const int outputPin[], int numPlaca){
  if (numPlaca == 0){
    digitalWrite(outputPin[0], LOW);
    digitalWrite(outputPin[1], LOW);
  }
  else if(numPlaca == 1){
    digitalWrite(outputPin[0], HIGH);
    digitalWrite(outputPin[1], LOW);
  }
  else if (numPlaca == 2){
    digitalWrite(outputPin[0], LOW);
    digitalWrite(outputPin[1], HIGH);
  }
  else if (numPlaca == 3){
    digitalWrite(outputPin[0], HIGH);
    digitalWrite(outputPin[1], HIGH);
  }
}

//funcao que verifica qual placa está enviando e armazena os valores no queue
int verificaEnvio(const int inputPin [], const int outputPin [], int numPlaca){
 
  int numConverted = convertNumber(analogRead(inputPin[2]),analogRead(inputPin[3]));
  if((numConverted != 0) && (numConverted != numPlaca)){
    Serial.print("PLACA ENVIANDO O DADO:");
    Serial.print(numPlaca);
    Serial.print(" -> ");
    Serial.print(numConverted);
    Serial.print(" -> ");
    Serial.println(convertNumber(analogRead(inputPin[0]),analogRead(inputPin[1])));
  
    //fazendo a placa parar de enviar dado
    decimalBinarioEscritor(outputPin, numPlaca);

    //envio de dados
    queue[posQueue].data[0] = analogRead(inputPin[0]);
    queue[posQueue].data[1] = analogRead(inputPin[1]);

    queue[posQueue].receiver[0] = analogRead(inputPin[2]);
    queue[posQueue].receiver[1] = analogRead(inputPin[3]);

    queue[posQueue].sender = numPlaca;

    //alterando o status da fila
    queue[posQueue].status = 1;

    return 1;
  }
  return 0;
}

void resetStatus(){

  queue[posQueue].status = 0;
  digitalWrite(outputPinClient1[0], LOW);
  digitalWrite(outputPinClient1[1], LOW);

  digitalWrite(outputPinClient2[0], LOW);
  digitalWrite(outputPinClient2[1], LOW);

  digitalWrite(outputPinClient3[0], LOW);
  digitalWrite(outputPinClient3[1], LOW);
}

/*
Converte binario em decimal - pos0 é o menos significativo
*/
int convertNumber(int pos0, int pos1){
  int address = 0;
  if(pos0 >= 950)
    address = address + 1;
  if(pos1 >= 950)
    address = address + 2;
  return address;
}

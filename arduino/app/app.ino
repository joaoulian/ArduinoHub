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
  int sender[2]; //quem esta enviando
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

      //variaveis auxiliares
      int isSender = 0;
      int data0, data1, receiver0, receiver1, sender0, sender1;
      int inputReceiver;
      
      int numConverted = convertNumber(analogRead(inputPinClient1[2]),analogRead(inputPinClient1[3]));

      if((numConverted != 0) && (numConverted != 1)){
        Serial.println("PLACA 1 ENVIANDO DADO");
        
        //a placa 1 está enviando um dado
        isSender = 1;
        data0 = analogRead(inputPinClient1[0]);
        data1 = analogRead(inputPinClient1[1]);
        receiver0 = analogRead(inputPinClient1[2]);
        receiver1 = analogRead(inputPinClient1[3]);
        sender0 = 1;
        sender1 = 0;

        //fazendo a placa parar de enviar dado
        digitalWrite(outputPinClient1[0], HIGH);
        digitalWrite(outputPinClient1[1], LOW);
      }
      

      if(isSender == 1){
        //envio de dados
        queue[posQueue].data[0] = data0;
        queue[posQueue].data[1] = data1;
  
        queue[posQueue].receiver[0] = receiver0;
        queue[posQueue].receiver[1] = receiver1;
  

        queue[posQueue].sender[0] = sender0;
        queue[posQueue].sender[1] = sender1;
        
        //alterando o status da fila
        queue[posQueue].status = 1;
      }
      
      
  }

  if (queue[posQueue].status == 1) { //enviando o dado
    Serial.println("- STATUS ENVIANDO O DADO");
    int addressNumber = convertNumber(queue[posQueue].receiver[0], queue[posQueue].receiver[1]);
    int auxOutput0 = outputPinClient1[0];
    int auxOutput1 = outputPinClient1[1];
    
    if(addressNumber == 2){
      Serial.println("RECEBENDO NA PLACA 2");
      auxOutput0 = outputPinClient2[0];
      auxOutput1 = outputPinClient2[1];
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
    //todo: confirmacao de recebimento
    Serial.println("- STATUS CONFIRMANDO RECEBIMENTO");
    //Serial.println("- PLACA QUE ESTA RECEBENDO: ");
    //Serial.println(convertNumber(queue[posQueue].receiver[0], queue[posQueue].receiver[1]));

    if(contador >= 5){
        queue[posQueue].status = 1;
        contador = 0;
    }
      
    if(convertNumber(queue[posQueue].receiver[0], queue[posQueue].receiver[1]) == 2){
      Serial.println("ESPERANDO VIR 2");
      Serial.println(convertNumber(analogRead(inputPinClient2[2]),analogRead(inputPinClient2[3])));
      if(convertNumber(analogRead(inputPinClient2[2]),analogRead(inputPinClient2[3])) == 2){
        /*Serial.print(analogRead(inputPinClient2[3])); //endereco
        Serial.print(" ");
        Serial.print(analogRead(inputPinClient2[2])); //endereco
        Serial.print(" ");
        Serial.print(analogRead(inputPinClient2[1])); //dado
        Serial.print(" ");
        Serial.println(analogRead(inputPinClient2[0])); //dado
        */
        resetStatus();
      } 
    }
    contador++;
  }
  
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

int rx = 4;
int tx = 8;
char received[4]; // limited to 4 bits for testing
char message[4] = {1, 0, 1, 0}; // limited to 1010 for testing
int delayTime = 5;

int buttonSend = 7;

// 0 -> IDLE / WAITING
// 1 -> TRANSMIT
int stateMachine;

void setup() {
  Serial.begin(9600);
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);

  pinMode(buttonSend, INPUT);

  digitalWrite(tx, HIGH); 
  stateMachine = 0;
}

void loop() {
  
  switch(stateMachine){
    case 0:
    // start bit detected
    digitalWrite(tx, LOW);
    if(digitalRead(rx) == LOW)
    {
      Serial.println("LOW detected SM0");
      receiveMessage(received);
      printMessage(received);
    }
    
    if(buttonDebounce())
    {
      stateMachine = 1;
      if(digitalRead(rx) == LOW)
    {
      Serial.println("LOW detected SM1");
    }
    }
    break;

    case 1:
    digitalWrite(tx, LOW);
    transmitMessage(message);
    stateMachine = 0;
    break;

    default:
    stateMachine = 0;
    break;
  }
}

void transmitMessage(char *message){
  digitalWrite(tx, LOW);
  Serial.println("LOW");
  delay(delayTime);
  for(int i = 0; i < 4; i++)
  {
    if(message[i] == 1)
    {
      digitalWrite(tx, HIGH);
      Serial.println("HIGH");
      delay(delayTime);
    }
    else if(message[i] == 0)
    {
      digitalWrite(tx, LOW);
      Serial.println("LOW");
      delay(delayTime);
    }

    if(i == 3)
    {
      digitalWrite(tx, HIGH);
      Serial.println("HIGH");
    }
  }
}



int receiveMessage(char *received){
  Serial.println("receiving...");
  delay(delayTime); // called after start bit
  for(int i = 0; i < 4; i++)
  {
    if(digitalRead(rx) == HIGH){
      received[i] = 1;
      delay(delayTime);
    }
    else if(digitalRead(rx) == LOW){
      received[i] = 0;
      delay(delayTime);
    }
    
  }
}

void printMessage(char *message)
{
  for(int i = 0; i > 4; i++)
  {
    Serial.println(received[i]);
  }
}

bool buttonDebounce()
{
  int buttonState = digitalRead(buttonSend);
  if(buttonState == HIGH)
  {
    delay(100);
    if(buttonState == HIGH)
    {
      Serial.println("Button pressed");
      return true;
    }
  }
  return false;
}

#define echoPin 2
#define trigPin 3
#define led 4
#define led2 5
#define led3 6
#define led4 7
#define led5 8
#define led6 9
#define led7 10
#define led8 11
#define led9 12
#define led10 13

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT); 
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);
  pinMode(led10, OUTPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
 
  
  if (distance < 165) digitalWrite(led, HIGH);
  else digitalWrite(led,LOW);

  if (distance < 150) digitalWrite(led2, HIGH);
  else digitalWrite(led2, LOW);

  if (distance < 135) digitalWrite(led3, HIGH);
  else digitalWrite(led3, LOW);
  
  if (distance < 120) digitalWrite(led4, HIGH);
  else digitalWrite(led4,LOW);
  
  if (distance < 105) digitalWrite(led5, HIGH);
  else digitalWrite(led5,LOW);

  if (distance < 90) digitalWrite(led6, HIGH);
  else digitalWrite(led6,LOW);
    
  if (distance < 75) digitalWrite(led7, HIGH);
  else digitalWrite(led7,LOW);
   
  if (distance < 60) digitalWrite(led8, HIGH);
  else digitalWrite(led8,LOW);
  
  if (distance < 45) digitalWrite(led9, HIGH);
  else digitalWrite(led9,LOW);
  
  if (distance < 30) digitalWrite(led10, HIGH);
  else digitalWrite(led10,LOW);
 
  delay(500);
}

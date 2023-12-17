// https://www.youtube.com/watch?v=pIflB4FQpNE&t=369s
// mowertacho
double revolutions=0;
double rpm=0; // max value 32,767 16 bit
long  startTime=0;
long  elapsedTime;
 
void setup() 
{
    Serial.begin(115200);
    pinMode(26, INPUT_PULLUP);           // set pin to input
}
 

double maxNum(int xxx, double yyy){
  xxx = (double)xxx;
  if(xxx >= yyy)
    return xxx;
  return yyy;
}
void loop() {
  revolutions=0; rpm=0;
  startTime=millis();         
  attachInterrupt(digitalPinToInterrupt(26),interruptFunction,RISING);
  delay(1000);
  detachInterrupt(26);                

  //now let's see how many counts we've had from the hall effect sensor and calc the RPM
  elapsedTime=millis()-startTime;     //finds the time, should be very close to 1 sec

  if(revolutions>0){
    rpm = (        ((maxNum(1, revolutions)) * 60000) / elapsedTime      )/2;        //calculates rpm
    //rpm = (maxNum(1, revolutions)/elapsedTime) * 60000;
  }
    
    
  String outMsg = String("RPM :") + rpm;
  Serial.println(outMsg);
}

void interruptFunction() //interrupt service routine
{  
  revolutions++;
}

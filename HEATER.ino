/*
temperature parameters:
  Goal: Keep inside temperature at 20 Celsius
  if any single sensore below 10, turn on heat for 30 seconds OR until all sensores are >= 20
  if average is <= 18 turn on heat for 30 seconds OR untill average is 22
  else do nothing
*/


//TMP36 Pin Variables

int tmp0 = 0;
int tmp1 = 1;
int tmp2 = 2;
int tmp3 = 3;

unsigned long startMillis = 0;
unsigned long curmill = 0;

float averageTemp;

boolean heat = false;

float temp0; float temp1; float temp2; float temp3;
float read0; float read1; float read2; float read3;
 
/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor

  pinMode(10, OUTPUT);

  startMillis = millis();
}
 
void loop()                     // run over and over again
{
  read0 = analogRead(tmp0); delay(10);
  analogRead(tmp1); delay(10);
  read1 = analogRead(tmp1); delay(10);
  analogRead(tmp2);delay(10);
  read2 = analogRead(tmp2); delay(10);
  analogRead(tmp3);delay(10);
  read3 = analogRead(tmp3);
  
  temp0 = tempConvert(read0);
  temp1 = tempConvert(read1);
  temp2 = tempConvert(read2);
  temp3 = tempConvert(read3);

  printTemp(temp0, tmp0);
  printTemp(temp1, tmp1);
  printTemp(temp2, tmp2);
  printTemp(temp3, tmp3);

  averageTemp = averager(temp0, temp1, temp2, temp3);
  printAvg(averageTemp);

Serial.print("Heating = "); Serial.println(heat);

int counter;

  if((temp0 <= 10 || temp1 <= 10 || temp2 <= 10 || temp3 <= 10 || averageTemp <= 18) && !heat)
  {
    digitalWrite(10, HIGH);
    heat = true;
    startMillis = millis();
    curmill = millis();
  }
  else if(heat)
  {
    curmill = millis();
    counter = (curmill-startMillis)/1000;
    Serial.println(counter);
    if( ((curmill - startMillis >= 30000)) || ((averageTemp >= 22)) || ((temp0 >= 20 || temp1 >= 20
	|| temp2 >= 20 || temp3 >= 20)) )
    {
      digitalWrite(10, LOW);
      heat = false;
      curmill = 0;  
    }
  }

  delay(1000);                                     //waiting 1 second
}

float tempConvert(float reading)
{
  //Serial.println(reading);
  float voltage = reading * 5.0;
  voltage /= 1023.0;

  float temp = (voltage - 0.5)*100;
  //Serial.println(temp);
  return temp;
}

float averager(float temp0, float temp1, float temp2, float temp3)
{
  float avg = (temp0 + temp1 + temp2 + temp3)/4;
  return avg;
}

void printAvg(float avg)
{
  Serial.print("Average temperature: ");Serial.print(avg);Serial.println(" Celsius");
}

void printTemp(float temp, int sensor)
{
  Serial.print(temp); Serial.print(" Celsius, Sensor #"); Serial.println(sensor);
}

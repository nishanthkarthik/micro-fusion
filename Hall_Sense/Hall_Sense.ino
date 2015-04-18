#define LINE_COUNT 50
#define HALL_SENSOR 2
#define HALL_SENSOR_INT 0

volatile long oldTimeStamp;

void Int_Hall()
{
  int currTimeStamp = millis();
  Serial.println((float)60000 / (currTimeStamp - oldTimeStamp));
  oldTimeStamp = currTimeStamp;
}

void setup() 
{
  digitalWrite(HALL_SENSOR, HIGH);
  attachInterrupt(HALL_SENSOR_INT, Int_Hall, RISING);
  Serial.begin(115200);
}

void loop() 
{
  
}

int returnLineCount(int value)
{
  return (float)value / 255 * LINE_COUNT;
}

void printLines(int value)
{
  for (int i = 0; i < value; ++i)
  {
    Serial.print('|');
  }
  Serial.print('\n');
}

#define LINE_COUNT 50
#define HALL_SENSOR A4

int oldPinValue;
int nowPinValue;
long oldTimeStamp;
long nowTimeStamp;

void setup() 
{
  Serial.begin(115200);
  oldTimeStamp = millis();
  oldPinValue = returnLineCount(analogRead(HALL_SENSOR));
}

void loop() 
{
  nowPinValue = returnLineCount(analogRead(HALL_SENSOR));
  if (nowPinValue - oldPinValue >= 2)
  {
    nowTimeStamp = millis();
    Serial.println(nowTimeStamp - oldTimeStamp);
    oldTimeStamp = nowTimeStamp;
  }
  oldPinValue = nowPinValue;
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

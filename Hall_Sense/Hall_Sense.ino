#define LINE_COUNT 50

void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  printLines(returnLineCount(analogRead(A4)));
}

int returnLineCount(int value)
{
  return value / 1024 * LINE_COUNT;
}

void printLines(int value)
{
  for (int i = 0; i < value; ++i)
  {
    Serial.print('|');
  }
  Serial.print('\n');
}

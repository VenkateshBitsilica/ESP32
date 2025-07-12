//Controlling LED brightness using PWM
const int channel = 0;
const int freq = 500;
const int resolution = 8;
const int max_duty_cycle = (1 << resolution)-1;

void setup() {
  ledcSetup(channel, freq, resolution);

  ledcAttachPin(2,channel);
}

void loop() {
    for(int i=0; i<max_duty_cycle; i++)
    {
      ledcWrite(channel,i);
      delay(10);
    }
    for(int i=max_duty_cycle; i>=0; i--)
    {
      ledcWrite(channel, i);
      delay(10);

    }
}


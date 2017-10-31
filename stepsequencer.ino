// has this change worked? github test
int buttonPin [4] = {32, 31, 30, 29,};
int buttonPinUp = 33;
int buttonPinDown = 34;
int ledPin[4] = {2, 3, 4, 5};
int i = 0;
int currentSequence = 0;
boolean buttonStateUp = LOW;
boolean lastButtonStateUp = LOW;
boolean buttonStateDown = LOW;
boolean lastButtonStateDown = LOW;
boolean lastButtonState[4] = {LOW, LOW, LOW, LOW};
boolean buttonState[4] = {LOW, LOW, LOW, LOW};
boolean stepState[3][4] = {
  {false, true, false, true},
  {true, false, false, true},
  {true, true, true, true},
};
int totalLeds = 4;
int tempo = 0;
int currentStep = 0;
unsigned long lastStepTime = 0;
int midiNote[3] = {60, 62, 64};


void setup() {
  pinMode(buttonPinUp, INPUT);
  pinMode(buttonPinDown, INPUT);
  for (int i = 0; i < totalLeds; i++) {
    pinMode(buttonPin[i], INPUT);
    pinMode(ledPin[i], OUTPUT);
  }
}

void loop() {
  checkButtons();
  sequence();
  setLeds();

}

void checkButtons() {

  lastButtonStateUp = buttonStateUp;
  lastButtonStateDown = buttonStateDown;
  buttonStateUp = digitalRead(buttonPinUp);
  buttonStateDown = digitalRead(buttonPinDown);

  if (buttonStateUp != lastButtonStateUp)
  {
    if (buttonStateUp == HIGH)
    {
      if (currentSequence == 2)
      {
        currentSequence = -1;
      }
      currentSequence++;
    }
  }

  if (buttonStateDown != lastButtonStateDown)
  {
    if (buttonStateDown == HIGH)
    {
      if (currentSequence == 0)
      {
        currentSequence = 3;
      }
      currentSequence--;
    }
  }

  for (int i = 0; i < totalLeds; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPin[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (stepState[currentSequence][i] == false) {
        stepState[currentSequence][i] = true;
      } else if (stepState[currentSequence][i] == true) {
        stepState[currentSequence][i] = false;
      }
    }
  }
}

void setLeds() {
  for (int i = 0; i < totalLeds; i++) {

    if (i == currentStep) {
      analogWrite(ledPin[i], 255);
    } else if (stepState[currentSequence][i] == true) {
      analogWrite(ledPin[i], 50);
    }
    else {
      analogWrite(ledPin[i], 0);
    }
  }
}

void sequence() {

  tempo = analogRead(A5);

  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
    //digitalWrite(ledPin[currentStep], LOW);  //turn off the current led

    currentStep = currentStep + 1;         //increment to the next step
    if (currentStep > 3) {
      currentStep = 0;
    }
    //digitalWrite(ledPin[currentStep], HIGH); //turn on the new led
    note();
    lastStepTime = millis();               //set lastStepTime to the current time
  }
}

void note() {
  for (int i = 0; i < 3; i++) {
    if (stepState[i][currentStep] == true) {
      usbMIDI.sendNoteOn(midiNote[i], 127, 1);
    }
    else {
      usbMIDI.sendNoteOff(midiNote[i], 127, 1);
    }
  }
}

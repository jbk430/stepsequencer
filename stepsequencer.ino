boolean lastButtonState[6] = {LOW, LOW, LOW, LOW, LOW, LOW};
boolean buttonState[6] = {LOW, LOW, LOW, LOW, LOW, LOW};
boolean stepState[3][4] = {{LOW, LOW, LOW, LOW},
                          {LOW, LOW, LOW, LOW},
                          {HIGH, LOW, LOW, LOW}
                          };
int buttonPin[6] = {7, 8, 9, 10, 11, 12};
int ledPin[4] = {2, 3, 4, 5};
int tempo = 0;
int currentStep = 0;
int currentChannel = 0;
unsigned long lastStepTime = 0;
int pot = A13;
int midiNote[3] = {60, 62, 64};
//adding comment

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  for (int i = 0; i < 6; i++) {
    pinMode(buttonPin[i], INPUT);
  }
}

void loop() {
  //Serial.println(currentChannel);
  updateChannel();
  checkButtons();
  setLeds();
  sequence();

}
void updateChannel(){
 
 for (int i = 4; i < 6; i++)  {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPin[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      
      //Up Button Code
        if (i == 4) {
         currentChannel++;
         if(currentChannel > 2) {
          currentChannel = 0;
         }
        }

      //Down Button Code
        if (i == 5) {
          currentChannel--;
          if(currentChannel < 0) {
            currentChannel = 2;
          }
        }
    }
 }
}


void checkButtons() {
  for (int i = 0; i < 4; i++)  {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPin[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (stepState[currentChannel][i] == false) {
        stepState[currentChannel][i] = true;
      } else if (stepState[currentChannel][i] == true) {
        stepState[currentChannel][i] = false;
      }
    }
  }
}

void setLeds() {
  for (int i = 0; i < 4; i++) {

    //If the LED corresponds to the currentStep, the led is set HIGH.
    if (currentStep == i) {
      analogWrite(ledPin[i], 100);
    }
    //Otherwise, if that step’s stepStates[i] is HIGH, that led is also set HIGH.
    else if (stepState[currentChannel][i] == true) {
      analogWrite(ledPin[i], 5);
    }
    //Otherwise, the led is set LOW.
    else {
      digitalWrite(ledPin[i], LOW);
    }
  }
}
/*
  void setLeds() {
  for (int i = 0; i < 4; i++) {

    if (stepState[i] == true) {
      // digitalWrite(ledPin[i], HIGH);
    } else if (stepState[i] == false) {
      // digitalWrite(ledPin[i], LOW);
    }
    else {
    }
  }
  }
*/
void sequence() {

  tempo = map(analogRead(A13), 0, 1023, 10, 250);

  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
    // digitalWrite(ledPin[currentStep], LOW);  //turn off the current led

    for (int i = 0; i < 3; i++) {

      if (stepState[i][currentStep] == HIGH) {
        usbMIDI.sendNoteOn(midiNote[i], 127, 1);
      }
      else {
        usbMIDI.sendNoteOff(midiNote[i], 0, 1);
      }
    }
    currentStep = currentStep + 1;         //increment to the next step
    if (currentStep > 3) {
      currentStep = 0;
    }
    // digitalWrite(ledPin[currentStep], HIGH); //turn on the new led

    lastStepTime = millis();               //set lastStepTime to the current time
  }
}

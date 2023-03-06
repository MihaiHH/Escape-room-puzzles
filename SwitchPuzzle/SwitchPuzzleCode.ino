// Número total de switches
const byte numSwitches = 8;
// Pines a los que se ha conectado los switches
const byte switchPins[numSwitches] = {2, 3, 4, 5, 6, 7, 8, 9};
// Pin al que se encuentra conectado el relé
const byte relayPin = A1;

// Último estado de llos switches
bool lastSwitchState[numSwitches];
// Los posibles estados del puzle
enum State {Initialaising, Active, Solved};
// Estado actual del puzle
State state = Initialaising;
// Solución del puzle
// True interruptor arriba
// False interruptor abajo
bool solution[numSwitches] = {true, true, true, true, false, false, false, false};

// Se encarga de leer y persistir el estado actual de cada switch
void readState() {
  for(int i=0; i<numSwitches; i++) {
    int switchValue = digitalRead(switchPins[i]);
    if (switchValue != lastSwitchState[i]) {
      lastSwitchState[i] = (bool)switchValue;
    }
  }
}

// Comprueba si se ha resuelto el puzle
bool isSolved() {
  bool solved = true;
  for(int i=0; i<numSwitches; i++) {
    if(lastSwitchState[i] != solution[i]) {
      solved = false;
    }
  }
  return solved;
}

void setup() {
  // Se establecen como pines de entrada los que hay conectados a los switches
  for(int i=0; i<numSwitches; i++) {
    pinMode(switchPins[i], INPUT);
  }
  // Pin de salida para el relé
  pinMode(relayPin, OUTPUT);
  // Se cambia el estado del puzzle a activo
  state = Active;
}

void loop() {
  readState();
  bool isPuzzleSolved = isSolved();
  // Si el puzzle está activado y se ha resuelto
  if (state == Active && isPuzzleSolved) {
    // Se corta la corriente al relé
    digitalWrite(relayPin, LOW);
    state = Solved;
  }
  // Si el puzzle está en estado resuelto pero el puzle se ha modificado
  else if (state == Solved && !isPuzzleSolved) {
    // Se vuelve a pasar corriente por el relé
    digitalWrite(relayPin, HIGH);
    state = Active;
  }
}

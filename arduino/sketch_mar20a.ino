// Predefined Chars
// bool CHAR_ZERO[7] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW };
// bool CHAR_ONE[7] = { LOW, HIGH, HIGH, LOW, LOW, LOW, LOW };
// bool CHAR_TWO[7] = { HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH };
// bool CHAR_THREE[7] = { HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH };
// bool CHAR_FOUR[7] = { LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH };
// bool CHAR_FIVE[7] = { HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH };
// bool CHAR_SIX[7] = { HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH };
// bool CHAR_SEVEN[7] = { HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW };
// bool CHAR_EIGHT[7] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
// bool CHAR_NINE[7] = { HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH };
// bool CHAR_C[7] = { HIGH, LOW, LOW, HIGH, HIGH, HIGH, LOW };

uint8_t pointPin = 9;
uint8_t segmentPins[7] = { 4, 13, 8, 10, 11, 3, 6 };
uint8_t positionPins[4] = { 5, 2, 12, 7 };

String receivedText = "";

bool displayCharAtPosition(int position, char c, bool point = false);
bool displayChar(char c, bool point = false);

void setup() {
  // Point Pin
  pinMode(pointPin, OUTPUT);
  // Position Pins
  for (int i = 0; i < 4; i++) {
    pinMode(positionPins[i], OUTPUT);
  }
  // Segment Pins
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    receivedText = Serial.readStringUntil('\n');
  }
  if (receivedText != "") {
    displayText(receivedText);
  }
}

// Helper functions

void displayText(String text) {
  uint8_t position = 0;
  unsigned int textLength = text.length();
  for (int i = 0; i < textLength || position < 4; i++) {
    if (displayCharAtPosition(position, text[i], i+1 < textLength ? text[i+1] == '.' : false)) {
      position++;
    }
  }
}

bool displayCharAtPosition(int position, char c, bool point) {
  clearSegments();
  closePositions();
  int posPin = positionPins[position];
  switchOnPosition(posPin);
  return displayChar(c, point);
}

bool displayChar(char c, bool point) {
  bool * segmentsMap = getSegmentsMap(c);
  bool segmentsMapDefined = segmentsMap != NULL;
  if (segmentsMapDefined && point) switchOnSegment(pointPin);
  if (!segmentsMapDefined) clearSegments();
  else switchOnSegments(segmentsMap);
  delete segmentsMap;
  return segmentsMapDefined;
}

bool * getSegmentsMap(char c) {
  switch (c) {
    case '0': return new bool[7] { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW };
    case '1': return new bool[7] { LOW, HIGH, HIGH, LOW, LOW, LOW, LOW };
    case '2': return new bool[7] { HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH };
    case '3': return new bool[7] { HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH };
    case '4': return new bool[7] { LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH };
    case '5': return new bool[7] { HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH };
    case '6': return new bool[7] { HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH };
    case '7': return new bool[7] { HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW };
    case '8': return new bool[7] { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
    case '9': return new bool[7] { HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH };
    case 'C': return new bool[7] { HIGH, LOW, LOW, HIGH, HIGH, HIGH, LOW };
  }
  return NULL;
}

void switchOnSegments(bool segments[]) {
  for (int i = 0; i < 7; i++) {
    if (segments[i]) switchOnSegment(segmentPins[i]);
  }
}

void switchOnSegment(uint8_t p) {
  digitalWrite(p, LOW);
}

void switchOffSegment(uint8_t p) {
  digitalWrite(p, HIGH);
}

void switchOnPosition(uint8_t p) {
  digitalWrite(p, HIGH);  
}

void switchOffPosition(uint8_t p) {
  digitalWrite(p, LOW);
}

void closePositions() {
  for (int i = 0; i < 4; i++) {
    switchOffPosition(positionPins[i]);
  }
}

void clearSegments() {
  for (int i = 0; i < 7; i++) {
    switchOffSegment(segmentPins[i]);
  }
  switchOffSegment(pointPin);
}

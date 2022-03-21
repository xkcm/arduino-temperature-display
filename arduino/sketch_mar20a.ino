uint8_t pointPin = 9;
uint8_t segmentPins[7] = { 4, 13, 8, 10, 11, 3, 6 }; // { A, B, C, D, E, F, G }
uint8_t positionPins[4] = { 5, 2, 12, 7 }; // { D1, D2, D3, D4 }

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
  unsigned int textLength = text.length();
  uint8_t position = 0;
  for (int i = 0; i < textLength; i++) {
    if (position == 4) break;
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

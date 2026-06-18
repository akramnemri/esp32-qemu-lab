/*
 * ESP32 Embedded Systems Lab - Exercise 1+
 * Intern Work - June 2026
 * 
 * Hardware: DOIT ESP32 DevKit V1
 * LED: On-board blue LED (GPIO 2) - external RGB module defective
 * 
 * Pattern: 2 quick blinks → 2 long blinks → on 2s → off 2s → repeat
 */

const int ledPin = 2;           // On-board LED pin (GPIO 2)
const int quickOn = 200;        // Quick blink ON duration (ms)
const int quickOff = 200;       // Quick blink OFF duration (ms)
const int longOn = 800;         // Long blink ON duration (ms)
const int longOff = 800;        // Long blink OFF duration (ms)
const int holdOn = 2000;        // Steady ON duration (ms)
const int holdOff = 2000;       // Steady OFF duration (ms)

unsigned long cycleCount = 0;   // Track completed cycles

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  
  // Startup banner
  Serial.println("========================================");
  Serial.println("ESP32 LED Pattern - Intern Lab Work");
  Serial.println("Board: DOIT ESP32 DevKit V1");
  Serial.println("LED: On-board GPIO 2 (external RGB dead)");
  Serial.println("Pattern: 2 quick → 2 long → 2s on → 2s off");
  Serial.println("========================================");
  Serial.println();
}

void blinkQuick() {
  digitalWrite(ledPin, HIGH);
  Serial.println("[QUICK] LED ON  (200ms)");
  delay(quickOn);
  digitalWrite(ledPin, LOW);
  Serial.println("[QUICK] LED OFF (200ms)");
  delay(quickOff);
}

void blinkLong() {
  digitalWrite(ledPin, HIGH);
  Serial.println("[LONG]  LED ON  (800ms)");
  delay(longOn);
  digitalWrite(ledPin, LOW);
  Serial.println("[LONG]  LED OFF (800ms)");
  delay(longOff);
}

void holdSteady() {
  digitalWrite(ledPin, HIGH);
  Serial.println("[HOLD]  LED ON  (2000ms)");
  delay(holdOn);
  digitalWrite(ledPin, LOW);
  Serial.println("[HOLD]  LED OFF (2000ms)");
  delay(holdOff);
}

void loop() {
  cycleCount++;
  Serial.print(">>> CYCLE #");
  Serial.println(cycleCount);
  Serial.println("------------------------------");
  
  // 2 quick blinks
  blinkQuick();
  blinkQuick();
  
  // 2 long blinks
  blinkLong();
  blinkLong();
  
  // Steady on 2s, off 2s
  holdSteady();
  
  Serial.println("------------------------------");
  Serial.println();
}
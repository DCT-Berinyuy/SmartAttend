/*
 * SmartAttend - Main Attendance System
 * 
 * Purpose: Record student attendance by scanning RFID cards
 * Reads student data from programmed RFID cards and logs attendance via serial
 * 
 * Author: SmartAttend Team
 * Date: 2026-02-08
 * 
 * HARDWARE CONNECTIONS:
 * RFID Module (MFRC522) → Arduino UNO
 * - SDA (SS) → Pin 10
 * - SCK → Pin 13
 * - MOSI → Pin 11
 * - MISO → Pin 12
 * - RST → Pin 9
 * - 3.3V → 3.3V (IMPORTANT: NOT 5V!)
 * - GND → GND
 * 
 * RGB LED Indicators → Arduino UNO
 * - Red LED → Pin 6 (with 220Ω resistor)
 * - Green LED → Pin 5 (with 220Ω resistor)
 * - Blue LED → Pin 4 (with 220Ω resistor)
 * 
 * Buzzer → Arduino UNO
 * - Buzzer (+) → Pin 8
 * - Buzzer (-) → GND
 * 
 * LED STATUS CODES:
 * - Blue: System ready, waiting for card
 * - Yellow (Red + Green): Reading card data
 * - Green: Success - Attendance recorded
 * - Red: Error - Authentication failed, duplicate scan, or unknown card
 * 
 * BUZZER PATTERNS:
 * - 1 short beep: Card detected
 * - 2 short beeps: Success - Attendance recorded
 * - 3 short beeps: Error - Duplicate or invalid card
 * 
 * SERIAL OUTPUT FORMAT:
 * ATTENDANCE,Date,Time,Name,Email,WhatsApp,MatricNo
 * Example: ATTENDANCE,2026-02-08,09:15:23,John Doe,john@school.edu,+237123456789,MAT001
 */

#include <SPI.h>
#include <MFRC522.h>

// Pin Definitions
#define RST_PIN 9
#define SS_PIN 10
#define RED_LED 6
#define GREEN_LED 5
#define BLUE_LED 4
#define BUZZER 8

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

// Memory blocks for student data
#define BLOCK_NAME 4
#define BLOCK_EMAIL 5
#define BLOCK_WHATSAPP 6
#define BLOCK_MATRIC 8

// Buffer for reading
byte bufferLen = 18;
byte readBlockData[18];

// Duplicate prevention
String lastCardUID = "";
unsigned long lastScanTime = 0;
const unsigned long SCAN_DELAY = 5000; // 5 seconds between scans of same card

// Session tracking
int attendanceCount = 0;
unsigned long sessionStartTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to connect
  
  // Initialize SPI and RFID
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Initialize pins
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  // Set default authentication key
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  // Welcome message
  Serial.println(F("========================================"));
  Serial.println(F("      SmartAttend - Attendance System"));
  Serial.println(F("========================================"));
  Serial.println(F(""));
  Serial.println(F("System initialized successfully!"));
  Serial.println(F(""));
  Serial.println(F("LED Status Indicators:"));
  Serial.println(F("  Blue   = Ready, waiting for card"));
  Serial.println(F("  Yellow = Reading card data"));
  Serial.println(F("  Green  = Success, attendance recorded"));
  Serial.println(F("  Red    = Error or duplicate scan"));
  Serial.println(F(""));
  Serial.println(F("Serial Output Format:"));
  Serial.println(F("ATTENDANCE,Date,Time,Name,Email,WhatsApp,MatricNo"));
  Serial.println(F(""));
  Serial.println(F("========================================"));
  Serial.println(F("Ready to scan cards!"));
  Serial.println(F("========================================"));
  Serial.println(F(""));
  
  // Ready signal
  beepReady();
  setLED("blue");
  
  sessionStartTime = millis();
}

void loop() {
  // Set blue LED (ready state)
  setLED("blue");
  
  // Look for new card
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  // Card detected - beep once
  beepOnce();
  
  // Get card UID
  String currentCardUID = getCardUID();
  
  // Check for duplicate scan
  if (currentCardUID == lastCardUID && (millis() - lastScanTime) < SCAN_DELAY) {
    Serial.println(F(""));
    Serial.println(F("⚠ DUPLICATE SCAN DETECTED"));
    Serial.print(F("Card UID: "));
    Serial.println(currentCardUID);
    Serial.println(F("Please wait 5 seconds before scanning again."));
    Serial.println(F(""));
    
    setLED("red");
    beepError();
    delay(2000);
    
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return;
  }
  
  // Update last scan info
  lastCardUID = currentCardUID;
  lastScanTime = millis();
  
  // Set yellow LED (reading state)
  setLED("yellow");
  
  Serial.println(F(""));
  Serial.println(F("--- CARD DETECTED ---"));
  Serial.print(F("Card UID: "));
  Serial.println(currentCardUID);
  Serial.println(F("Reading student data..."));
  
  // Read student data from card
  String studentName = readDataFromBlock(BLOCK_NAME);
  String studentEmail = readDataFromBlock(BLOCK_EMAIL);
  String studentWhatsApp = readDataFromBlock(BLOCK_WHATSAPP);
  String studentMatric = readDataFromBlock(BLOCK_MATRIC);
  
  // Check if card is programmed
  if (studentName == "" || studentName == "ERROR") {
    Serial.println(F(""));
    Serial.println(F("✗ ERROR: Card not programmed!"));
    Serial.println(F("Please program this card using SmartAttend_CardWriter"));
    Serial.println(F(""));
    
    setLED("red");
    beepError();
    delay(2000);
    
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return;
  }
  
  // Display student info
  Serial.println(F(""));
  Serial.println(F("--- STUDENT INFORMATION ---"));
  Serial.println(F("Name: ") + studentName);
  Serial.println(F("Email: ") + studentEmail);
  Serial.println(F("WhatsApp: ") + studentWhatsApp);
  Serial.println(F("Matric No: ") + studentMatric);
  Serial.println(F(""));
  
  // Get current date and time (placeholder - will be added by computer script)
  String currentDate = "DATE";
  String currentTime = "TIME";
  
  // Output attendance record in CSV format
  Serial.print(F("ATTENDANCE,"));
  Serial.print(currentDate);
  Serial.print(F(","));
  Serial.print(currentTime);
  Serial.print(F(","));
  Serial.print(studentName);
  Serial.print(F(","));
  Serial.print(studentEmail);
  Serial.print(F(","));
  Serial.print(studentWhatsApp);
  Serial.print(F(","));
  Serial.println(studentMatric);
  
  // Increment attendance count
  attendanceCount++;
  
  Serial.println(F(""));
  Serial.println(F("✓ ATTENDANCE RECORDED"));
  Serial.print(F("Total attendance today: "));
  Serial.println(attendanceCount);
  Serial.println(F(""));
  Serial.println(F("========================================"));
  
  // Success feedback
  setLED("green");
  beepSuccess();
  delay(2000);
  
  // Halt card
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  // Small delay before next scan
  delay(500);
}

// Function to get card UID as string
String getCardUID() {
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

// Function to read data from a block
String readDataFromBlock(int blockNum) {
  // Authenticate
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed for block "));
    Serial.print(blockNum);
    Serial.print(F(": "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return "ERROR";
  }
  
  // Read data
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed for block "));
    Serial.print(blockNum);
    Serial.print(F(": "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return "ERROR";
  }
  
  // Convert to string and clean
  String data = String((char*)readBlockData);
  data.trim();
  
  // Remove null characters
  for (int i = 0; i < data.length(); i++) {
    if (data[i] == '\0') {
      data = data.substring(0, i);
      break;
    }
  }
  
  return data;
}

// LED control function
void setLED(String color) {
  // Turn off all LEDs first
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  
  // Set requested color
  if (color == "red") {
    digitalWrite(RED_LED, HIGH);
  } else if (color == "green") {
    digitalWrite(GREEN_LED, HIGH);
  } else if (color == "blue") {
    digitalWrite(BLUE_LED, HIGH);
  } else if (color == "yellow") {
    // Yellow = Red + Green
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
  } else if (color == "off") {
    // All off (already done above)
  }
}

// Buzzer functions
void beepOnce() {
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}

void beepSuccess() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }
}

void beepError() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(150);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }
}

void beepReady() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(50);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }
}

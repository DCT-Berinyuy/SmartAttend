/*
 * SmartAttend - RFID Card Writer
 * 
 * Purpose: Program student information onto RFID cards
 * This sketch writes student data to RFID card memory blocks
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
 * LED Indicators → Arduino UNO
 * - Green LED → Pin 5 (with 220Ω resistor)
 * - Red LED → Pin 6 (with 220Ω resistor)
 * 
 * Buzzer → Arduino UNO
 * - Buzzer (+) → Pin 8
 * - Buzzer (-) → GND
 */

#include <SPI.h>
#include <MFRC522.h>

// Pin Definitions
#define RST_PIN 9
#define SS_PIN 10
#define GREEN_LED 5
#define RED_LED 6
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

// Buffer for reading/writing
byte bufferLen = 18;
byte readBlockData[18];

// Student data variables
String studentName = "";
String studentEmail = "";
String studentWhatsApp = "";
String studentMatric = "";

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to connect
  
  // Initialize SPI and RFID
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Initialize pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  // Set default authentication key (0xFF for all bytes)
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  // Welcome message
  Serial.println(F("========================================"));
  Serial.println(F("   SmartAttend - RFID Card Writer"));
  Serial.println(F("========================================"));
  Serial.println(F(""));
  Serial.println(F("This tool programs student data onto RFID cards."));
  Serial.println(F(""));
  Serial.println(F("IMPORTANT NOTES:"));
  Serial.println(F("- Maximum 16 characters per field"));
  Serial.println(F("- Use abbreviations if names are too long"));
  Serial.println(F("- Each card can only be programmed once per session"));
  Serial.println(F(""));
  Serial.println(F("Ready to program cards!"));
  Serial.println(F("========================================"));
  Serial.println(F(""));
  
  // Ready signal
  beepSuccess();
  blinkLED(GREEN_LED, 2);
}

void loop() {
  // Prompt for student data
  Serial.println(F("\n--- NEW CARD PROGRAMMING ---"));
  Serial.println(F("Please enter student details:"));
  Serial.println(F(""));
  
  // Get student name
  Serial.print(F("Student Name (max 16 chars): "));
  studentName = getSerialInput();
  if (studentName.length() > 16) {
    Serial.println(F("WARNING: Name truncated to 16 characters!"));
    studentName = studentName.substring(0, 16);
  }
  Serial.println(studentName);
  
  // Get student email
  Serial.print(F("Email Address (max 16 chars): "));
  studentEmail = getSerialInput();
  if (studentEmail.length() > 16) {
    Serial.println(F("WARNING: Email truncated to 16 characters!"));
    studentEmail = studentEmail.substring(0, 16);
  }
  Serial.println(studentEmail);
  
  // Get WhatsApp number
  Serial.print(F("WhatsApp Number (max 16 chars): "));
  studentWhatsApp = getSerialInput();
  if (studentWhatsApp.length() > 16) {
    Serial.println(F("WARNING: WhatsApp truncated to 16 characters!"));
    studentWhatsApp = studentWhatsApp.substring(0, 16);
  }
  Serial.println(studentWhatsApp);
  
  // Get matriculation number
  Serial.print(F("Matriculation Number (max 16 chars): "));
  studentMatric = getSerialInput();
  if (studentMatric.length() > 16) {
    Serial.println(F("WARNING: Matric No truncated to 16 characters!"));
    studentMatric = studentMatric.substring(0, 16);
  }
  Serial.println(studentMatric);
  
  // Display summary
  Serial.println(F(""));
  Serial.println(F("--- DATA SUMMARY ---"));
  Serial.print(F("Name: ")); Serial.println(studentName);
  Serial.print(F("Email: ")); Serial.println(studentEmail);
  Serial.print(F("WhatsApp: ")); Serial.println(studentWhatsApp);
  Serial.print(F("Matric No: ")); Serial.println(studentMatric);
  Serial.println(F(""));
  
  // Wait for card
  Serial.println(F(">>> Place RFID card on reader now <<<"));
  digitalWrite(RED_LED, HIGH); // Red LED while waiting
  
  // Wait for card to be present
  while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(100);
  }
  
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  beepOnce();
  
  Serial.println(F(""));
  Serial.println(F("Card detected! Programming..."));
  
  // Display card UID
  Serial.print(F("Card UID: "));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  // Write data to card
  bool success = true;
  
  if (!writeDataToBlock(BLOCK_NAME, studentName)) success = false;
  if (!writeDataToBlock(BLOCK_EMAIL, studentEmail)) success = false;
  if (!writeDataToBlock(BLOCK_WHATSAPP, studentWhatsApp)) success = false;
  if (!writeDataToBlock(BLOCK_MATRIC, studentMatric)) success = false;
  
  if (success) {
    Serial.println(F(""));
    Serial.println(F("✓ Card programmed successfully!"));
    Serial.println(F(""));
    
    // Verify by reading back
    Serial.println(F("Verifying data..."));
    verifyCard();
    
    digitalWrite(GREEN_LED, LOW);
    beepSuccess();
    blinkLED(GREEN_LED, 3);
  } else {
    Serial.println(F(""));
    Serial.println(F("✗ Card programming FAILED!"));
    Serial.println(F("Please try again with a new card."));
    Serial.println(F(""));
    
    digitalWrite(GREEN_LED, LOW);
    beepError();
    blinkLED(RED_LED, 3);
  }
  
  // Halt card
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  Serial.println(F("========================================"));
  Serial.println(F("Ready for next card..."));
  delay(2000);
}

// Function to get serial input
String getSerialInput() {
  while (Serial.available() == 0) {
    // Wait for input
  }
  String input = Serial.readStringUntil('\n');
  input.trim();
  return input;
}

// Function to write data to a block
bool writeDataToBlock(int blockNum, String data) {
  byte blockData[16];
  memset(blockData, 0, sizeof(blockData)); // Clear buffer
  data.getBytes(blockData, 16); // Convert string to byte array
  
  Serial.print(F("Writing to block "));
  Serial.print(blockNum);
  Serial.print(F(": "));
  Serial.print(data);
  
  // Authenticate
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F(" - Authentication FAILED: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  
  // Write data
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F(" - Write FAILED: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  
  Serial.println(F(" ✓"));
  return true;
}

// Function to read data from a block
String readDataFromBlock(int blockNum) {
  // Authenticate
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    return "ERROR";
  }
  
  // Read data
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK) {
    return "ERROR";
  }
  
  // Convert to string and clean
  String data = String((char*)readBlockData);
  data.trim();
  return data;
}

// Function to verify card data
void verifyCard() {
  String name = readDataFromBlock(BLOCK_NAME);
  String email = readDataFromBlock(BLOCK_EMAIL);
  String whatsapp = readDataFromBlock(BLOCK_WHATSAPP);
  String matric = readDataFromBlock(BLOCK_MATRIC);
  
  Serial.println(F("--- VERIFICATION ---"));
  Serial.print(F("Name: ")); Serial.println(name);
  Serial.print(F("Email: ")); Serial.println(email);
  Serial.print(F("WhatsApp: ")); Serial.println(whatsapp);
  Serial.print(F("Matric No: ")); Serial.println(matric);
  
  if (name == studentName && email == studentEmail && 
      whatsapp == studentWhatsApp && matric == studentMatric) {
    Serial.println(F("✓ Verification PASSED"));
  } else {
    Serial.println(F("✗ Verification FAILED - Data mismatch!"));
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
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }
}

// LED blink function
void blinkLED(int pin, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(200);
    digitalWrite(pin, LOW);
    delay(200);
  }
}

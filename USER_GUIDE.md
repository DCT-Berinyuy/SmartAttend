# SmartAttend - Step-by-Step User Guide

This guide walks you through the complete process of setting up and using SmartAttend, from hardware assembly to recording your first attendance.

---

## 📋 Prerequisites Checklist

Before starting, ensure you have:

- [ ] Arduino UNO board
- [ ] MFRC522 RFID module
- [ ] RFID cards (MIFARE Classic 1K)
- [ ] 3x LEDs (Red, Green, Blue)
- [ ] 3x 220Ω resistors
- [ ] Active buzzer
- [ ] Breadboard
- [ ] Jumper wires
- [ ] USB cable
- [ ] Computer with Arduino IDE installed
- [ ] (Optional) Battery for portable operation

---

## Phase 1: Hardware Assembly (30-45 minutes)

### Step 1: Organize Your Workspace

1. Clear a flat, well-lit workspace
2. Lay out all components
3. Have the WIRING_GUIDE.md open for reference
4. Prepare jumper wires of different colors

### Step 2: Connect RFID Module

> ⚠️ **CRITICAL:** Use 3.3V, NOT 5V for the RFID module!

1. **Power connections:**
   - RFID 3.3V → Arduino 3.3V (RED wire)
   - RFID GND → Arduino GND (BLACK wire)

2. **SPI connections:**
   - RFID SCK → Arduino Pin 13 (YELLOW wire)
   - RFID MOSI → Arduino Pin 11 (BLUE wire)
   - RFID MISO → Arduino Pin 12 (GREEN wire)
   - RFID SDA → Arduino Pin 10 (ORANGE wire)

3. **Control connection:**
   - RFID RST → Arduino Pin 9 (WHITE wire)

### Step 3: Connect LEDs with Resistors

For each LED:

1. **Red LED (Error indicator):**
   - Insert LED into breadboard
   - Connect long leg (+) to 220Ω resistor
   - Connect resistor to Arduino Pin 6
   - Connect short leg (-) to breadboard ground rail

2. **Green LED (Success indicator):**
   - Same process, connect to Arduino Pin 5

3. **Blue LED (Ready indicator):**
   - Same process, connect to Arduino Pin 4

### Step 4: Connect Buzzer

1. Identify buzzer polarity (+ and - markings)
2. Connect buzzer + to Arduino Pin 8
3. Connect buzzer - to breadboard ground rail

### Step 5: Connect Ground

1. Use a black jumper wire
2. Connect breadboard ground rail to Arduino GND pin
3. This provides common ground for all components

### Step 6: Visual Inspection

Double-check:

- [ ] RFID module on 3.3V (NOT 5V)
- [ ] All SPI pins correct (10, 11, 12, 13)
- [ ] RST on Pin 9
- [ ] All LEDs have resistors
- [ ] LED polarity correct (long leg to resistor)
- [ ] Buzzer polarity correct
- [ ] All grounds connected
- [ ] No loose wires

---

## Phase 2: Software Setup (15-20 minutes)

### Step 1: Install Arduino IDE

If not already installed:

1. Download from: https://www.arduino.cc/en/software
2. Install for your operating system
3. Launch Arduino IDE

### Step 2: Install MFRC522 Library

1. Open Arduino IDE
2. Click **Sketch → Include Library → Manage Libraries**
3. Search for "MFRC522"
4. Click **Install** on "MFRC522 by GithubCommunity"
5. Wait for installation to complete
6. Close Library Manager

### Step 3: Configure Arduino IDE

1. Connect Arduino to computer via USB
2. Select board: **Tools → Board → Arduino UNO**
3. Select port: **Tools → Port → /dev/ttyACM0** (or similar)
4. Verify connection: **Tools → Get Board Info**

### Step 4: Test Hardware

1. Open: **File → Examples → MFRC522 → DumpInfo**
2. Click **Upload** button (→)
3. Wait for "Done uploading"
4. Open **Tools → Serial Monitor**
5. Set baud rate to **9600**
6. Place RFID card on reader
7. You should see card UID and details

✅ If you see card info, hardware is working correctly!

---

## Phase 3: Programming RFID Cards (5 minutes per card)

### Step 1: Upload Card Writer Sketch

1. Open `SmartAttend_CardWriter_Tool/SmartAttend_CardWriter_Tool.ino` in Arduino IDE
2. Click **Verify** (✓) to check for errors
3. Click **Upload** (→) to upload to Arduino
4. Wait for "Done uploading"

### Step 2: Open Serial Monitor

1. Click **Tools → Serial Monitor**
2. Set baud rate to **9600**
3. You should see welcome message

### Step 3: Program First Card

1. **Enter student name** when prompted
   - Type name (max 16 characters)
   - Press Enter
   - Example: `John Doe`

2. **Enter email address**
   - Type email (max 16 characters)
   - Press Enter
   - Example: `john@school.edu`

3. **Enter WhatsApp number**
   - Type number with country code
   - Press Enter
   - Example: `+237123456789`

4. **Enter matriculation number**
   - Type matric number
   - Press Enter
   - Example: `MAT2024001`

5. **Review data summary**
   - Check all information is correct
   - If wrong, restart Arduino (press reset button)

6. **Place card on reader**
   - Position card flat on RFID module
   - Wait for beeps and LED flashes
   - Watch Serial Monitor for success message

7. **Verify card**
   - System automatically reads back data
   - Check verification matches input
   - If failed, try a new card

8. **Remove card**
   - Take card off reader
   - Label card with student name
   - Store safely

### Step 4: Repeat for All Students

1. For each student, repeat Step 3
2. Keep a record of which card belongs to which student
3. Store programmed cards securely

**Pro Tip:** Create a spreadsheet mapping Card UID → Student Name for reference.

---

## Phase 4: Recording Attendance (Daily Use)

### Step 1: Upload Attendance Sketch

1. Open `SmartAttend_Main/SmartAttend_Main.ino` in Arduino IDE
2. Click **Upload** (→)
3. Wait for "Done uploading"

### Step 2: Choose Data Logging Method

Pick one of the following options:

#### Option A: Manual Serial Monitor (Simplest)

1. Open **Tools → Serial Monitor**
2. Set baud rate to **9600**
3. Watch for ATTENDANCE lines
4. Copy/paste to spreadsheet manually

#### Option B: Python Script (Recommended)

1. Open terminal
2. Navigate to SmartAttend folder:
   ```bash
   cd ~/Desktop/Development/Arduino/SmartAttend
   ```
3. Run Python script:
   ```bash
   python3 attendance_logger.py
   ```
4. Script will auto-detect Arduino and start logging
5. Data saved to `attendance_log.csv`
6. Press Ctrl+C to stop

#### Option C: Bash Script (Linux)

1. Open terminal
2. Navigate to SmartAttend folder:
   ```bash
   cd ~/Desktop/Development/Arduino/SmartAttend
   ```
3. Run bash script:
   ```bash
   ./attendance_logger.sh
   ```
4. Script will auto-detect Arduino and start logging
5. Data saved to timestamped text file
6. Press Ctrl+C to stop

### Step 3: Record Attendance

1. **System Ready:**
   - Blue LED should be on
   - Serial output shows "Ready to scan cards"

2. **Student Scans Card:**
   - Student places card on RFID reader
   - 1 beep = card detected
   - Yellow LED = reading data

3. **Success:**
   - Green LED lights up
   - 2 beeps
   - Student info displayed
   - ATTENDANCE line output
   - Total count incremented

4. **Error/Duplicate:**
   - Red LED lights up
   - 3 beeps
   - Error message displayed
   - Student should wait 5 seconds and try again

### Step 4: End of Session

1. **If using Python/Bash script:**
   - Press Ctrl+C to stop
   - Check output file for records

2. **If using Serial Monitor:**
   - Copy all ATTENDANCE lines
   - Paste into spreadsheet
   - Save file

3. **Disconnect Arduino:**
   - Close Serial Monitor
   - Safely disconnect USB (or leave connected for next session)

---

## Phase 5: Data Management

### Viewing Attendance Records

#### CSV File (Python Script)

1. Open `attendance_log.csv` in spreadsheet software
2. Columns: Date, Time, Name, Email, WhatsApp, Matric No
3. Sort, filter, and analyze as needed

#### Text File (Bash Script)

1. Open `attendance_log_YYYYMMDD_HHMMSS.txt` in text editor
2. Parse ATTENDANCE lines
3. Import to spreadsheet if needed

### Importing to Google Sheets

#### Manual Import:

1. Open Google Sheets
2. File → Import → Upload
3. Select `attendance_log.csv`
4. Choose import settings
5. Click Import

#### Automatic Sync (Python Script):

1. Set up Google Cloud project
2. Enable Google Sheets API
3. Download `credentials.json`
4. Place in SmartAttend folder
5. Edit `attendance_logger.py`:
   ```python
   ENABLE_GOOGLE_SHEETS = True
   ```
6. Run script - data uploads automatically

---

## Daily Workflow Summary

### Morning Setup (5 minutes)

1. Connect Arduino to computer via USB
2. Upload `SmartAttend.ino` (if not already uploaded)
3. Start data logger script (Python or Bash)
4. Verify blue LED is on (system ready)

### During Class (Continuous)

1. Students scan cards as they arrive
2. Monitor for errors (red LED)
3. Assist students if needed

### End of Class (2 minutes)

1. Stop data logger script (Ctrl+C)
2. Verify attendance count matches expected
3. Check output file for completeness
4. Upload to Google Sheets (if not automatic)

---

## Troubleshooting Quick Reference

| Problem                       | Quick Fix                                   |
| ----------------------------- | ------------------------------------------- |
| Blue LED not on               | Check power connections, upload sketch      |
| Card not detected             | Check RFID module on 3.3V, verify SPI pins  |
| Red LED always on             | Check LED polarity, verify pin 6 connection |
| No buzzer sound               | Check buzzer polarity, verify active buzzer |
| Duplicate scan error          | Wait 5 seconds between scans                |
| Card not programmed           | Use CardWriter sketch to program card       |
| Python script error           | Install pyserial: `pip install pyserial`    |
| Bash script permission denied | Run: `chmod +x attendance_logger.sh`        |

For detailed troubleshooting, see README.md.

---

## Tips for Success

### Hardware Tips:

- ✅ Use color-coded wires for easy troubleshooting
- ✅ Secure all connections firmly
- ✅ Keep RFID module away from metal surfaces
- ✅ Use fresh batteries for portable operation

### Card Programming Tips:

- ✅ Use abbreviations for long names
- ✅ Test each card immediately after programming
- ✅ Keep backup list of card UIDs
- ✅ Label cards clearly

### Attendance Recording Tips:

- ✅ Position RFID reader at comfortable height
- ✅ Instruct students to hold card flat on reader
- ✅ Monitor for duplicate scans
- ✅ Back up attendance data regularly

### Data Management Tips:

- ✅ Export data daily
- ✅ Keep multiple backups
- ✅ Review for anomalies
- ✅ Archive old records

---

## Next Steps

After completing this guide:

1. ✅ Test system with a few cards
2. ✅ Program all student cards
3. ✅ Run a practice attendance session
4. ✅ Train students on card scanning
5. ✅ Set up regular data backup routine

---

**Need Help?** Refer to:

- [README.md](README.md) - Complete documentation
- [WIRING_GUIDE.md](WIRING_GUIDE.md) - Detailed wiring instructions
- Troubleshooting section in README.md

---

_Last Updated: 2026-02-08_

# SmartAttend - RFID-Based Automated Attendance System

![Arduino](https://img.shields.io/badge/Arduino-UNO-00979D?style=flat&logo=arduino)
![License](https://img.shields.io/badge/License-MIT-green)
![Status](https://img.shields.io/badge/Status-Ready-brightgreen)

**SmartAttend** is an RFID-based automated attendance system that eliminates manual sign-in sheets, reduces errors, and prevents impersonation. Students simply scan their RFID cards to register attendance instantly.

---

## 📋 Table of Contents

- [Features](#-features)
- [Hardware Requirements](#-hardware-requirements)
- [Software Requirements](#-software-requirements)
- [Quick Start Guide](#-quick-start-guide)
- [Installation](#-installation)
- [Usage](#-usage)
- [Data Logging Options](#-data-logging-options)
- [Troubleshooting](#-troubleshooting)
- [Project Structure](#-project-structure)
- [Contributing](#-contributing)
- [License](#-license)

---

## ✨ Features

- ✅ **Instant Attendance Recording** - Scan RFID card to register attendance
- ✅ **Student Data Storage** - Stores Name, Email, WhatsApp, Matriculation Number on cards
- ✅ **Visual Feedback** - RGB LED status indicators (Blue=Ready, Green=Success, Red=Error)
- ✅ **Audio Feedback** - Buzzer patterns for different events
- ✅ **Duplicate Prevention** - 5-second delay between duplicate scans
- ✅ **Serial Data Output** - CSV-formatted attendance records
- ✅ **Multiple Logging Options** - Manual, Python script, or Bash script
- ✅ **Google Sheets Integration** - Optional cloud sync (Python script)
- ✅ **Portable Operation** - Battery-powered option
- ✅ **Easy Card Programming** - Interactive card writer tool

---

## 🛠️ Hardware Requirements

### Essential Components

- **Arduino UNO** (or compatible board)
- **MFRC522 RFID Module** (13.56 MHz)
- **RFID Cards/Tags** (MIFARE Classic 1K)
- **3x LEDs** (Red, Green, Blue)
- **3x 220Ω Resistors** (for LEDs)
- **Active Buzzer**
- **Breadboard**
- **Jumper Wires** (Male-to-Male, Male-to-Female)
- **USB Cable** (for programming and serial communication)

### Optional Components

- **9V Battery + Connector** (for portable operation)
- **Enclosure/Case** (for finished product)

---

## 💻 Software Requirements

### Arduino Development

- **Arduino IDE** (1.8.x or 2.x)
- **MFRC522 Library** (install via Library Manager)
- **SPI Library** (built-in)

### Data Logging (Choose One)

#### Option 1: Manual (No Additional Software)

- Arduino IDE Serial Monitor

#### Option 2: Python Script (Recommended)

- **Python 3.x**
- **pyserial** library: `pip install pyserial`
- **(Optional)** **gspread** library for Google Sheets: `pip install gspread oauth2client`

#### Option 3: Bash Script (Linux)

- Bash shell (included in Omarchy Linux)
- No additional dependencies

---

## 🚀 Quick Start Guide

### Step 1: Hardware Setup

1. Follow the [WIRING_GUIDE.md](WIRING_GUIDE.md) to connect all components
2. **IMPORTANT:** Connect RFID module to **3.3V** (NOT 5V!)
3. Double-check all connections before powering on

### Step 2: Install Arduino Libraries

1. Open Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries**
3. Search for "MFRC522" and install the latest version

### Step 3: Program RFID Cards

1. Open `SmartAttend_CardWriter_Tool/SmartAttend_CardWriter_Tool.ino` in Arduino IDE
2. Upload to Arduino UNO
3. Open Serial Monitor (9600 baud)
4. Follow prompts to enter student data
5. Place RFID card on reader when prompted
6. Repeat for all students

### Step 4: Run Attendance System

1. Open `SmartAttend_Main/SmartAttend_Main.ino` in Arduino IDE
2. Upload to Arduino UNO
3. Choose a data logging method (see below)
4. Students scan cards to register attendance

---

## 📥 Installation

### 1. Clone or Download Project

```bash
cd ~/Desktop/Development/Arduino
# Project already created in SmartAttend folder
```

### 2. Install Arduino Libraries

```bash
# Open Arduino IDE
# Sketch → Include Library → Manage Libraries
# Search: MFRC522
# Click Install
```

### 3. Install Python Dependencies (If Using Python Logger)

```bash
# Install pyserial
pip install pyserial

# Optional: For Google Sheets integration
pip install gspread oauth2client
```

### 4. Make Bash Script Executable (If Using Bash Logger)

```bash
cd ~/Desktop/Development/Arduino/SmartAttend
chmod +x attendance_logger.sh
```

---

## 📖 Usage

### Phase 1: Programming RFID Cards

1. **Upload Card Writer Sketch:**

   ```bash
   # Open SmartAttend_CardWriter_Tool/SmartAttend_CardWriter_Tool.ino in Arduino IDE
   # Select: Tools → Board → Arduino UNO
   # Select: Tools → Port → (your Arduino port)
   # Click Upload button
   ```

2. **Open Serial Monitor:**
   - Tools → Serial Monitor
   - Set baud rate to **9600**

3. **Program Each Card:**
   - Follow prompts to enter:
     - Student Name (max 16 characters)
     - Email Address (max 16 characters)
     - WhatsApp Number (max 16 characters)
     - Matriculation Number (max 16 characters)
   - Place card on reader when prompted
   - Wait for verification
   - Remove card and repeat for next student

4. **Keep Card Inventory:**
   - Create a list mapping card UIDs to students
   - Store in safe place for reference

### Phase 2: Recording Attendance

1. **Upload Main Attendance Sketch:**

   ```bash
   # Open SmartAttend_Main/SmartAttend_Main.ino in Arduino IDE
   # Upload to Arduino UNO
   ```

2. **Choose Data Logging Method:**
   - See [Data Logging Options](#-data-logging-options) below

3. **Start Recording:**
   - Blue LED indicates system is ready
   - Students place cards on reader
   - Green LED + 2 beeps = Success
   - Red LED + 3 beeps = Error/Duplicate

4. **Monitor Attendance:**
   - View real-time output in Serial Monitor or logger script
   - Check CSV file or Google Sheets for records

---

## 📊 Data Logging Options

### Option 1: Manual Serial Monitor (Simplest)

**Best for:** Small classes, testing, quick checks

**How to use:**

1. Open Arduino IDE Serial Monitor (9600 baud)
2. Watch for ATTENDANCE lines
3. Copy/paste to spreadsheet manually

**Pros:**

- No additional software needed
- Works immediately

**Cons:**

- Manual work required
- No automatic timestamps

---

### Option 2: Python Script (Recommended)

**Best for:** Automated logging, Google Sheets integration

**How to use:**

```bash
cd ~/Desktop/Development/Arduino/SmartAttend
python3 attendance_logger.py
```

**Features:**

- ✅ Auto-detects Arduino port
- ✅ Adds timestamps automatically
- ✅ Saves to CSV file (`attendance_log.csv`)
- ✅ Real-time display
- ✅ Optional Google Sheets upload

**Google Sheets Setup (Optional):**

1. Create Google Cloud project
2. Enable Google Sheets API
3. Download credentials.json
4. Place in SmartAttend folder
5. Edit `attendance_logger.py`:
   ```python
   ENABLE_GOOGLE_SHEETS = True
   GOOGLE_SHEET_NAME = "SmartAttend Attendance"
   ```

---

### Option 3: Bash Script (Linux-Native)

**Best for:** Linux users, simple logging without Python

**How to use:**

```bash
cd ~/Desktop/Development/Arduino/SmartAttend
./attendance_logger.sh
```

**Features:**

- ✅ No dependencies (works on any Linux)
- ✅ Auto-detects Arduino port
- ✅ Adds timestamps
- ✅ Saves to timestamped text file
- ✅ Colored terminal output

**Output:** `attendance_log_YYYYMMDD_HHMMSS.txt`

---

## 🔧 Troubleshooting

### RFID Module Not Detecting Cards

**Symptoms:** No response when card is placed on reader

**Solutions:**

- ✓ Check RFID module is connected to **3.3V** (NOT 5V!)
- ✓ Verify SPI pins: 10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK)
- ✓ Check RST pin connected to Pin 9
- ✓ Try different RFID card
- ✓ Ensure card is MIFARE Classic 1K compatible
- ✓ Check for loose connections

### LEDs Not Working

**Symptoms:** LEDs don't light up or stay on

**Solutions:**

- ✓ Check LED polarity (long leg = +, short leg = -)
- ✓ Verify 220Ω resistors are connected
- ✓ Test LEDs with multimeter
- ✓ Check pin numbers: Red=6, Green=5, Blue=4
- ✓ Upload simple LED blink test sketch

### Buzzer Not Beeping

**Symptoms:** No sound from buzzer

**Solutions:**

- ✓ Check buzzer polarity (+ to Pin 8, - to GND)
- ✓ Verify it's an **active** buzzer (not passive)
- ✓ Test with simple digitalWrite HIGH/LOW
- ✓ Try different buzzer
- ✓ Check buzzer voltage rating (3-5V)

### Serial Communication Issues

**Symptoms:** No data in Serial Monitor or logger script

**Solutions:**

- ✓ Check USB cable connection
- ✓ Select correct COM port in Arduino IDE
- ✓ Set baud rate to **9600**
- ✓ Try different USB port
- ✓ Restart Arduino IDE
- ✓ Check serial port permissions (Linux): `sudo usermod -a -G dialout $USER`

### Card Programming Fails

**Symptoms:** "Authentication failed" or "Write failed" errors

**Solutions:**

- ✓ Use fresh, unprogrammed cards
- ✓ Some cards may be read-only
- ✓ Try different memory blocks
- ✓ Check RFID module connections
- ✓ Ensure card is MIFARE Classic 1K

### Python Script Can't Find Arduino

**Symptoms:** "Arduino not found" error

**Solutions:**

- ✓ Check Arduino is connected via USB
- ✓ Check USB cable is data-capable (not charge-only)
- ✓ Install pyserial: `pip install pyserial`
- ✓ Check port permissions (Linux): `ls -l /dev/ttyACM*`
- ✓ Try different USB port
- ✓ Manually specify port in script

### Bash Script Permission Denied

**Symptoms:** "Permission denied" when running script

**Solutions:**

```bash
chmod +x attendance_logger.sh
# If still fails, check serial port permissions:
sudo usermod -a -G dialout $USER
# Log out and log back in
```

---

## 📁 Project Structure

```
SmartAttend/
├── SmartAttend_CardWriter_Tool/
│   └── SmartAttend_CardWriter_Tool.ino    # RFID card programming sketch
├── SmartAttend_Main/
│   └── SmartAttend_Main.ino                # Main attendance system sketch
├── attendance_logger.py           # Python data logger script
├── attendance_logger.sh           # Bash data logger script
├── README.md                      # This file
├── WIRING_GUIDE.md               # Detailed wiring instructions
└── attendance_log.csv            # Generated CSV file (after running)
```

---

## 🎯 LED Status Codes

| LED Color | Meaning | Description                                                 |
| --------- | ------- | ----------------------------------------------------------- |
| 🔵 Blue   | Ready   | System waiting for card                                     |
| 🟡 Yellow | Reading | Card detected, reading data                                 |
| 🟢 Green  | Success | Attendance recorded                                         |
| 🔴 Red    | Error   | Authentication failed, duplicate scan, or unprogrammed card |

---

## 🔊 Buzzer Patterns

| Pattern       | Meaning       | Description                      |
| ------------- | ------------- | -------------------------------- |
| 1 short beep  | Card Detected | RFID card placed on reader       |
| 2 short beeps | Success       | Attendance recorded successfully |
| 3 short beeps | Error         | Duplicate scan or invalid card   |
| 2 quick beeps | System Ready  | System initialized successfully  |

---

## 📋 Serial Output Format

Attendance records are output in CSV format:

```
ATTENDANCE,Date,Time,Name,Email,WhatsApp,MatricNo
```

**Example:**

```
ATTENDANCE,2026-02-08,09:15:23,John Doe,john@school.edu,+237123456789,MAT001
```

**Note:** `DATE` and `TIME` placeholders are replaced by logger scripts with actual timestamps.

---

## 🔐 Security Considerations

- **Card Cloning:** MIFARE Classic 1K cards can be cloned. For high-security applications, consider upgrading to MIFARE DESFire or other encrypted cards.
- **Physical Security:** Store programmed cards securely to prevent unauthorized access.
- **Data Privacy:** Attendance records contain personal information. Handle according to your institution's data protection policies.

---

## 🚀 Future Enhancements

Potential improvements for SmartAttend:

- [ ] Add RTC module for offline timestamp support
- [ ] Implement SD card logging for offline operation
- [ ] Add LCD display for visual feedback
- [ ] Integrate with school management systems
- [ ] Add WiFi module (ESP8266) for direct Google Sheets upload
- [ ] Implement web dashboard for real-time monitoring
- [ ] Add fingerprint sensor for multi-factor authentication
- [ ] Create mobile app for attendance viewing

---

## 🤝 Contributing

Contributions are welcome! If you'd like to improve SmartAttend:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

---

## 📄 License

This project is open-source and available under the MIT License.

---

## 👥 Authors

**SmartAttend Team**

- Developed for classroom attendance automation
- Built with Arduino and RFID technology

---

## 📞 Support

If you encounter issues or have questions:

1. Check the [Troubleshooting](#-troubleshooting) section
2. Review the [WIRING_GUIDE.md](WIRING_GUIDE.md)
3. Verify all connections and component specifications
4. Test components individually

---

## 🙏 Acknowledgments

- **MFRC522 Library** by miguelbalboa and contributors
- **Arduino Community** for extensive documentation and support
- Inspired by existing RFID attendance systems

---

## 📊 System Requirements Summary

| Component         | Specification                   |
| ----------------- | ------------------------------- |
| Microcontroller   | Arduino UNO (ATmega328P)        |
| RFID Module       | MFRC522 (13.56 MHz)             |
| RFID Cards        | MIFARE Classic 1K               |
| Power Supply      | 5V via USB or 7-12V DC          |
| Operating System  | Windows, macOS, Linux (Omarchy) |
| Arduino IDE       | 1.8.x or 2.x                    |
| Python (Optional) | 3.x                             |

---

**Ready to get started?** Follow the [Quick Start Guide](#-quick-start-guide) above!

---

_Last Updated: 2026-02-08_

# SmartAttend - Wiring Guide

## Complete Hardware Connections

This guide shows you exactly how to connect all components to your Arduino UNO.

---

## ⚠️ IMPORTANT SAFETY NOTES

> [!CAUTION]
> **RFID Module Voltage:** The MFRC522 RFID module operates at **3.3V**. Connecting it to 5V will **damage the module**! Always use the 3.3V pin on Arduino UNO.

> [!WARNING]
> **LED Resistors:** Always use resistors (220Ω recommended) with LEDs to prevent burning them out.

> [!TIP]
> **Power Supply:** When using battery power, ensure it provides stable 7-12V DC to the Arduino's VIN pin or use a 5V regulated supply to the 5V pin.

---

## Component List

### Required Components

- ✅ Arduino UNO
- ✅ MFRC522 RFID Module
- ✅ RFID Cards/Tags (MIFARE Classic 1K)
- ✅ Active Buzzer
- ✅ 3x LEDs (Red, Green, Blue)
- ✅ 3x 220Ω Resistors (for LEDs)
- ✅ Breadboard
- ✅ Jumper Wires (Male-to-Male, Male-to-Female)
- ✅ USB Cable (for programming and power)
- ✅ Battery (optional, for portable operation)

---

## Pin Configuration Table

### RFID Module (MFRC522) → Arduino UNO

| RFID Pin | Arduino Pin   | Wire Color (Suggested) | Notes                    |
| -------- | ------------- | ---------------------- | ------------------------ |
| SDA (SS) | Pin 10        | Orange                 | Slave Select             |
| SCK      | Pin 13        | Yellow                 | SPI Clock                |
| MOSI     | Pin 11        | Blue                   | Master Out Slave In      |
| MISO     | Pin 12        | Green                  | Master In Slave Out      |
| IRQ      | Not connected | -                      | Optional, not used       |
| GND      | GND           | Black                  | Ground                   |
| RST      | Pin 9         | White                  | Reset                    |
| 3.3V     | **3.3V**      | **Red**                | **⚠️ USE 3.3V, NOT 5V!** |

### LED Indicators → Arduino UNO

| LED Color            | Arduino Pin | Resistor | Notes                     |
| -------------------- | ----------- | -------- | ------------------------- |
| Red LED (Anode +)    | Pin 6       | 220Ω     | Error/Duplicate indicator |
| Green LED (Anode +)  | Pin 5       | 220Ω     | Success indicator         |
| Blue LED (Anode +)   | Pin 4       | 220Ω     | Ready/Waiting indicator   |
| All LED Cathodes (-) | GND         | -        | Common ground             |

### Buzzer → Arduino UNO

| Buzzer Terminal | Arduino Pin | Notes         |
| --------------- | ----------- | ------------- |
| Positive (+)    | Pin 8       | Active buzzer |
| Negative (-)    | GND         | Ground        |

---

## Step-by-Step Wiring Instructions

### Step 1: Prepare Your Workspace

1. Disconnect Arduino from power
2. Place breadboard on flat surface
3. Organize components and wires
4. Have this guide ready for reference

### Step 2: Connect RFID Module

> [!IMPORTANT]
> The RFID module is the most critical component. Double-check all connections!

1. **Power Connections:**
   - Connect RFID **3.3V** pin to Arduino **3.3V** pin (use RED wire)
   - Connect RFID **GND** pin to Arduino **GND** pin (use BLACK wire)

2. **SPI Communication Pins:**
   - Connect RFID **SCK** to Arduino **Pin 13** (yellow wire)
   - Connect RFID **MOSI** to Arduino **Pin 11** (blue wire)
   - Connect RFID **MISO** to Arduino **Pin 12** (green wire)
   - Connect RFID **SDA (SS)** to Arduino **Pin 10** (orange wire)

3. **Control Pins:**
   - Connect RFID **RST** to Arduino **Pin 9** (white wire)
   - Leave RFID **IRQ** pin unconnected

### Step 3: Connect LEDs

Each LED needs a resistor to limit current and prevent damage.

1. **Red LED (Error Indicator):**
   - Insert LED into breadboard
   - Connect LED **long leg (anode +)** to one end of 220Ω resistor
   - Connect other end of resistor to Arduino **Pin 6**
   - Connect LED **short leg (cathode -)** to breadboard ground rail

2. **Green LED (Success Indicator):**
   - Insert LED into breadboard
   - Connect LED **long leg (anode +)** to one end of 220Ω resistor
   - Connect other end of resistor to Arduino **Pin 5**
   - Connect LED **short leg (cathode -)** to breadboard ground rail

3. **Blue LED (Ready Indicator):**
   - Insert LED into breadboard
   - Connect LED **long leg (anode +)** to one end of 220Ω resistor
   - Connect other end of resistor to Arduino **Pin 4**
   - Connect LED **short leg (cathode -)** to breadboard ground rail

### Step 4: Connect Buzzer

1. Identify buzzer polarity (usually marked with + and -)
2. Connect buzzer **positive (+)** terminal to Arduino **Pin 8**
3. Connect buzzer **negative (-)** terminal to breadboard ground rail

### Step 5: Connect Ground Rail

1. Connect breadboard **ground rail** to Arduino **GND** pin using black wire
2. This provides common ground for all LEDs and buzzer

### Step 6: Verify Connections

Before powering on, verify:

- [ ] RFID module connected to **3.3V** (NOT 5V)
- [ ] All SPI pins correctly connected (10, 11, 12, 13)
- [ ] RST pin connected to Pin 9
- [ ] All LEDs have resistors
- [ ] All grounds connected to GND
- [ ] Buzzer polarity correct
- [ ] No loose wires

---

## Circuit Diagram (Text-Based)

```
                    Arduino UNO
                  ┌─────────────┐
                  │             │
    RFID Module   │  Digital    │    LEDs + Resistors
    ┌──────────┐  │  Pins       │    ┌────────────┐
    │ 3.3V ────┼──┤ 3.3V        │    │            │
    │ RST  ────┼──┤ 9           │    │  Pin 4 ────┼─[220Ω]─┤>├─┐ Blue LED
    │ GND  ────┼──┤ GND         │    │  Pin 5 ────┼─[220Ω]─┤>├─┤ Green LED
    │ IRQ       │  │             │    │  Pin 6 ────┼─[220Ω]─┤>├─┘ Red LED
    │ MISO ────┼──┤ 12          │    │            │         │
    │ MOSI ────┼──┤ 11          │    │            │        GND
    │ SCK  ────┼──┤ 13          │    │            │
    │ SDA  ────┼──┤ 10          │    │            │
    └──────────┘  │             │    └────────────┘
                  │             │
                  │  Pin 8  ────┼───┐
                  │             │   │  Buzzer
                  │  GND    ────┼───┤  ┌──┐
                  │             │   └──┤+ │
                  └─────────────┘      │  │
                                       └──┘
                                        │
                                       GND
```

---

## LED Wiring Detail

```
Arduino Pin → [220Ω Resistor] → LED Anode (+) ──┤>├── LED Cathode (-) → GND

Example for Red LED:
Pin 6 → [220Ω] → Long Leg ──┤>├── Short Leg → GND
```

---

## Power Options

### Option 1: USB Power (Recommended for Development)

- Connect Arduino to computer via USB cable
- Provides 5V power and serial communication
- Best for programming and testing

### Option 2: Battery Power (For Portable Use)

- Use 9V battery with barrel jack connector
- Or use 7-12V DC power supply to VIN pin
- USB cable still needed for initial programming

### Option 3: External 5V Power

- Use regulated 5V power supply
- Connect to 5V pin (bypass voltage regulator)
- Ensure current capacity is sufficient (>500mA)

---

## Testing Your Wiring

### Quick Test Procedure

1. **Visual Inspection:**
   - Check all connections match this guide
   - Ensure no wires are loose
   - Verify RFID module is on 3.3V

2. **Power On Test:**
   - Connect Arduino to USB
   - Arduino power LED should light up
   - RFID module may have indicator LED

3. **LED Test:**
   - Upload a simple LED blink sketch
   - Test each LED individually (pins 4, 5, 6)
   - All LEDs should light up

4. **Buzzer Test:**
   - Upload a simple buzzer sketch
   - Buzzer should beep
   - If no sound, check polarity

5. **RFID Test:**
   - Upload RFID test sketch (from MFRC522 library examples)
   - Open Serial Monitor (9600 baud)
   - Place RFID card on reader
   - Card UID should appear

---

## Troubleshooting

### RFID Module Not Working

- ✓ Check 3.3V connection (NOT 5V!)
- ✓ Verify all SPI pins (10, 11, 12, 13)
- ✓ Check RST pin connection (Pin 9)
- ✓ Ensure good contact with breadboard
- ✓ Try different RFID card

### LEDs Not Lighting

- ✓ Check LED polarity (long leg = +)
- ✓ Verify resistor is connected
- ✓ Test LED with multimeter
- ✓ Check pin numbers in code match wiring

### Buzzer Not Beeping

- ✓ Check buzzer polarity (+ to Pin 8)
- ✓ Verify it's an active buzzer (not passive)
- ✓ Test with simple digitalWrite HIGH/LOW
- ✓ Try different buzzer

### Serial Communication Issues

- ✓ Check USB cable connection
- ✓ Select correct COM port in Arduino IDE
- ✓ Set baud rate to 9600
- ✓ Try different USB port

---

## Component Specifications

### MFRC522 RFID Module

- **Operating Voltage:** 3.3V DC
- **Current:** 13-26mA
- **Frequency:** 13.56 MHz
- **Communication:** SPI
- **Read Distance:** 0-60mm (depends on card/tag)

### LEDs

- **Forward Voltage:** ~2V (Red), ~3V (Blue/Green)
- **Forward Current:** 20mA (max)
- **Resistor Value:** 220Ω (for 5V Arduino)

### Active Buzzer

- **Operating Voltage:** 3-5V DC
- **Current:** 20-30mA
- **Sound Output:** 85dB @ 10cm

---

## Next Steps

After completing the wiring:

1. ✅ Double-check all connections
2. ✅ Upload test sketches to verify each component
3. ✅ Upload `SmartAttend_CardWriter.ino` to program RFID cards
4. ✅ Upload `SmartAttend.ino` for attendance recording
5. ✅ Run data logger script on your computer

---

## Safety Reminders

> [!CAUTION]
>
> - Never connect RFID module to 5V
> - Always use resistors with LEDs
> - Disconnect power before changing wiring
> - Check polarity of buzzer and LEDs
> - Avoid short circuits

---

**Need Help?** Refer to the main README.md for detailed usage instructions and troubleshooting.

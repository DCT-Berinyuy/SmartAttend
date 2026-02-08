#!/bin/bash
#
# SmartAttend - Attendance Data Logger (Bash Script)
# 
# Simple bash script to capture Arduino serial output and save to file
# This is a lightweight alternative to the Python script for basic logging
#
# Usage:
#   1. Make executable: chmod +x attendance_logger.sh
#   2. Connect Arduino via USB
#   3. Run: ./attendance_logger.sh
#   4. Press Ctrl+C to stop
#
# Output: attendance_log_YYYYMMDD_HHMMSS.txt
#
# Author: SmartAttend Team
# Date: 2026-02-08

# Colors for terminal output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Configuration
BAUD_RATE=9600
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
LOG_FILE="attendance_log_${TIMESTAMP}.txt"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  SmartAttend - Attendance Logger${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Find Arduino device
echo -e "${YELLOW}Searching for Arduino...${NC}"

# Common Arduino device paths on Linux
ARDUINO_DEVICES=(
    /dev/ttyACM0
    /dev/ttyACM1
    /dev/ttyUSB0
    /dev/ttyUSB1
)

ARDUINO_PORT=""

for device in "${ARDUINO_DEVICES[@]}"; do
    if [ -e "$device" ]; then
        ARDUINO_PORT="$device"
        echo -e "${GREEN}✓ Found Arduino on: $device${NC}"
        break
    fi
done

if [ -z "$ARDUINO_PORT" ]; then
    echo -e "${RED}✗ Arduino not found!${NC}"
    echo ""
    echo "Available devices:"
    ls -l /dev/tty* | grep -E "ACM|USB" || echo "  No Arduino-like devices found"
    echo ""
    echo "Please connect Arduino and try again."
    exit 1
fi

# Configure serial port
echo -e "${YELLOW}Configuring serial port...${NC}"
stty -F "$ARDUINO_PORT" $BAUD_RATE cs8 -cstopb -parenb

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Failed to configure serial port${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Serial port configured${NC}"
echo ""

# Create log file with header
echo "SmartAttend Attendance Log" > "$LOG_FILE"
echo "Session started: $(date '+%Y-%m-%d %H:%M:%S')" >> "$LOG_FILE"
echo "========================================" >> "$LOG_FILE"
echo "" >> "$LOG_FILE"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Logging started${NC}"
echo -e "${BLUE}  Output file: $LOG_FILE${NC}"
echo -e "${BLUE}  Press Ctrl+C to stop${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Trap Ctrl+C to clean up
trap ctrl_c INT

function ctrl_c() {
    echo ""
    echo ""
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  Logging stopped${NC}"
    echo -e "${BLUE}  Data saved to: $LOG_FILE${NC}"
    echo -e "${BLUE}========================================${NC}"
    
    # Add session end marker
    echo "" >> "$LOG_FILE"
    echo "========================================" >> "$LOG_FILE"
    echo "Session ended: $(date '+%Y-%m-%d %H:%M:%S')" >> "$LOG_FILE"
    
    exit 0
}

# Read from serial port and log
ATTENDANCE_COUNT=0

while IFS= read -r line; do
    # Get current timestamp
    CURRENT_TIME=$(date '+%Y-%m-%d %H:%M:%S')
    
    # Replace DATE and TIME placeholders in ATTENDANCE lines
    if [[ $line == ATTENDANCE,* ]]; then
        # Extract date and time
        CURRENT_DATE=$(date '+%Y-%m-%d')
        CURRENT_TIME_ONLY=$(date '+%H:%M:%S')
        
        # Replace placeholders
        line="${line/DATE/$CURRENT_DATE}"
        line="${line/TIME/$CURRENT_TIME_ONLY}"
        
        # Increment counter
        ((ATTENDANCE_COUNT++))
        
        # Display with color
        echo -e "${GREEN}$line${NC}"
        echo -e "${GREEN}  ✓ Attendance recorded (Total: $ATTENDANCE_COUNT)${NC}"
        echo ""
    else
        # Display regular output
        echo "$line"
    fi
    
    # Write to log file with timestamp
    echo "[$CURRENT_TIME] $line" >> "$LOG_FILE"
    
done < "$ARDUINO_PORT"

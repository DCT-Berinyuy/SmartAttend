#!/usr/bin/env python3
"""
SmartAttend - Attendance Data Logger (Python)

This script captures attendance data from the Arduino via serial port
and saves it to a CSV file with automatic timestamps.

Features:
- Automatic timestamp insertion (replaces DATE and TIME placeholders)
- CSV file creation and appending
- Real-time display of attendance records
- Optional Google Sheets integration (requires setup)

Requirements:
- Python 3.x
- pyserial library: pip install pyserial
- (Optional) gspread library for Google Sheets: pip install gspread oauth2client

Usage:
1. Connect Arduino to computer via USB
2. Upload SmartAttend.ino to Arduino
3. Run this script: python3 attendance_logger.py
4. Script will auto-detect Arduino serial port
5. Attendance records will be saved to attendance_log.csv

Author: SmartAttend Team
Date: 2026-02-08
"""

import serial
import serial.tools.list_ports
import csv
import os
from datetime import datetime
import time
import sys

# Configuration
BAUD_RATE = 9600
CSV_FILENAME = "attendance_log.csv"
ENABLE_GOOGLE_SHEETS = False  # Set to True to enable Google Sheets upload

# Google Sheets configuration (if enabled)
GOOGLE_SHEET_NAME = "SmartAttend Attendance"
GOOGLE_CREDENTIALS_FILE = "credentials.json"  # Download from Google Cloud Console


def find_arduino_port():
    """
    Automatically detect Arduino serial port
    """
    print("Searching for Arduino...")
    ports = serial.tools.list_ports.comports()
    
    for port in ports:
        # Look for Arduino-like devices
        if 'Arduino' in port.description or 'USB' in port.description or 'ACM' in port.device:
            print(f"✓ Found Arduino on port: {port.device}")
            return port.device
    
    print("✗ Arduino not found!")
    print("\nAvailable ports:")
    for port in ports:
        print(f"  - {port.device}: {port.description}")
    
    return None


def initialize_csv():
    """
    Create CSV file with headers if it doesn't exist
    """
    if not os.path.exists(CSV_FILENAME):
        with open(CSV_FILENAME, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(['Date', 'Time', 'Name', 'Email', 'WhatsApp', 'Matric No'])
        print(f"✓ Created new CSV file: {CSV_FILENAME}")
    else:
        print(f"✓ Using existing CSV file: {CSV_FILENAME}")


def upload_to_google_sheets(data):
    """
    Upload attendance record to Google Sheets (optional)
    Requires gspread and oauth2client libraries
    """
    if not ENABLE_GOOGLE_SHEETS:
        return
    
    try:
        import gspread
        from oauth2client.service_account import ServiceAccountCredentials
        
        # Setup Google Sheets API
        scope = ['https://spreadsheets.google.com/feeds',
                 'https://www.googleapis.com/auth/drive']
        
        creds = ServiceAccountCredentials.from_json_keyfile_name(GOOGLE_CREDENTIALS_FILE, scope)
        client = gspread.authorize(creds)
        
        # Open spreadsheet
        sheet = client.open(GOOGLE_SHEET_NAME).sheet1
        
        # Append data
        sheet.append_row(data)
        print("  ↳ Uploaded to Google Sheets")
        
    except ImportError:
        print("  ✗ Google Sheets libraries not installed")
        print("    Install with: pip install gspread oauth2client")
    except Exception as e:
        print(f"  ✗ Google Sheets upload failed: {e}")


def process_attendance_line(line):
    """
    Process an ATTENDANCE line from Arduino
    Format: ATTENDANCE,DATE,TIME,Name,Email,WhatsApp,MatricNo
    """
    try:
        parts = line.strip().split(',')
        
        if len(parts) != 7 or parts[0] != 'ATTENDANCE':
            return None
        
        # Get current date and time
        now = datetime.now()
        current_date = now.strftime('%Y-%m-%d')
        current_time = now.strftime('%H:%M:%S')
        
        # Replace DATE and TIME placeholders
        parts[1] = current_date
        parts[2] = current_time
        
        # Extract data
        data = {
            'date': parts[1],
            'time': parts[2],
            'name': parts[3],
            'email': parts[4],
            'whatsapp': parts[5],
            'matric': parts[6]
        }
        
        return data
        
    except Exception as e:
        print(f"✗ Error processing line: {e}")
        return None


def save_to_csv(data):
    """
    Append attendance record to CSV file
    """
    try:
        with open(CSV_FILENAME, 'a', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow([
                data['date'],
                data['time'],
                data['name'],
                data['email'],
                data['whatsapp'],
                data['matric']
            ])
        return True
    except Exception as e:
        print(f"✗ Error saving to CSV: {e}")
        return False


def main():
    """
    Main function - connect to Arduino and log attendance
    """
    print("=" * 60)
    print("  SmartAttend - Attendance Data Logger")
    print("=" * 60)
    print()
    
    # Find Arduino port
    port = find_arduino_port()
    if not port:
        print("\nPlease connect Arduino and try again.")
        sys.exit(1)
    
    # Initialize CSV file
    initialize_csv()
    
    print()
    print("=" * 60)
    print("  Starting attendance logging...")
    print("  Press Ctrl+C to stop")
    print("=" * 60)
    print()
    
    try:
        # Connect to Arduino
        ser = serial.Serial(port, BAUD_RATE, timeout=1)
        time.sleep(2)  # Wait for Arduino to reset
        
        print("✓ Connected to Arduino")
        print(f"✓ Logging to: {CSV_FILENAME}")
        if ENABLE_GOOGLE_SHEETS:
            print(f"✓ Google Sheets: Enabled")
        print()
        print("-" * 60)
        print()
        
        attendance_count = 0
        
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                
                # Display all Arduino output
                if line:
                    print(line)
                
                # Process ATTENDANCE lines
                if line.startswith('ATTENDANCE,'):
                    data = process_attendance_line(line)
                    
                    if data:
                        # Save to CSV
                        if save_to_csv(data):
                            attendance_count += 1
                            print(f"  ✓ Saved to CSV (Total: {attendance_count})")
                        
                        # Upload to Google Sheets (if enabled)
                        if ENABLE_GOOGLE_SHEETS:
                            upload_to_google_sheets([
                                data['date'],
                                data['time'],
                                data['name'],
                                data['email'],
                                data['whatsapp'],
                                data['matric']
                            ])
                        
                        print()
    
    except KeyboardInterrupt:
        print()
        print()
        print("=" * 60)
        print(f"  Logging stopped. Total attendance recorded: {attendance_count}")
        print("=" * 60)
        ser.close()
        sys.exit(0)
    
    except Exception as e:
        print(f"\n✗ Error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()

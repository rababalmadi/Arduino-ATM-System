# Arduino ATM System

## Core Function
An embedded ATM simulation system that authenticates users using a keypad PIN. The system allows users to check their account balance, withdraw cash, deposit money, and securely stores the account balance using EEPROM. A servo motor simulates the ATM door mechanism, while an LCD provides a simple user interface.

---

## Main Components
- **Arduino Uno:** Controls the entire ATM system.
- **4×4 Keypad:** Used to enter the PIN and navigate the ATM menu.
- **16×2 I2C LCD:** Displays instructions, menus, and transaction information.
- **Servo Motor:** Simulates the ATM security door.

---

## Working Principle

1. The user enters a 4-digit PIN and presses **#** to confirm.
2. If the PIN is correct:
   - Access is granted.
   - The servo motor unlocks the ATM.
   - The main menu is displayed.
3. The user can:
   - Press **A** to check the current balance.
   - Press **B** to withdraw money.
   - Press **D** to open the deposit menu.
   - Press **C** to deposit money.
4. Every successful withdrawal or deposit automatically updates the balance stored in EEPROM.
5. If an incorrect PIN is entered three consecutive times:
   - The ATM locks.
   - A 10-second countdown is displayed on the LCD.
   - The user can try again after the countdown finishes.

---

## Features
- PIN Authentication
- Balance Inquiry
- Cash Withdrawal
- Cash Deposit
- EEPROM Balance Storage
- LCD User Interface
- Servo Door Simulation
- Wrong PIN Protection
- 10-Second Lockout Countdown
- Delete Input Using `*`
- Confirm Input Using `#`

---

## Future Improvements
- Change PIN option
- Transaction history
- RFID authentication
- Fingerprint authentication
- SD Card transaction logging
- Mobile application (ESP32)
- Online banking simulation


## Author
**Rabab Almadi**

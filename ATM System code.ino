// =====================
// Libraries
// =====================
#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>
#include <Keypad.h>
#include <EEPROM.h>

// =====================
// LCD Configuration
// =====================
Adafruit_LiquidCrystal lcd(0);

// =====================
// Keypad Configuration
// =====================
const byte Rows = 4;
const byte Colms = 4;

// Keypad layout
char hexakeys[Rows][Colms] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Arduino pins connected to keypad rows
byte RowPins[Rows] = {13,12,11,10};

// Arduino pins connected to keypad columns
byte ColmPins[Colms] = {9,8,7,6};

// Create keypad object
Keypad Mykeypad = Keypad(makeKeymap(hexakeys), RowPins, ColmPins, Rows, Colms);

// =====================
// Global Variables
// =====================

// Servo motor object
Servo myServo;

// User entered PIN
String inputPin = "";

// Default ATM PIN
String CorrectPin = "1211";

// User account balance
int balance = 1000;

// Login status
bool loggedin = false;

// Stores amount entered from keypad
String amountInput = "";

// Withdrawal amount
int withdrawAmount = 0;

// Deposit amount
int depositAmount = 0;

// Counts incorrect PIN attempts
int wrongAttempts = 0;

// =====================
// Function Prototypes
// =====================
void checkPIN(char key);
void handleMenu(char key);
void checkBalance();
void withdrawMoney();
void depositMenu();
void depositMoney();
void showMenu();
void showMenu2();

void setup()
{
  // Initialize LCD (16 columns × 2 rows)
  lcd.begin(16,2);
  lcd.setBacklight(1);

  // Initialize servo and keep ATM door locked
  myServo.attach(2);
  myServo.write(0);

  // Load saved balance from EEPROM
  EEPROM.get(0, balance);

  // If EEPROM contains invalid data, initialize default balance
  if (balance <= 0 || balance > 100000)
  {
    balance = 1000;
    EEPROM.put(0, balance);
  }

  // Welcome screen
  lcd.setCursor(0,0);
  lcd.print("Arduino ATM");

  lcd.setCursor(0,1);
  lcd.print("Welcome");

  delay(2000);

  // Ask user to enter the PIN
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter PIN:");
}

// Main program loop
void loop()
{
    // Read pressed key from keypad
    char key = Mykeypad.getKey();

    // Check PIN before allowing access
    if(!loggedin)
    {
        checkPIN(key);
    }
    else
    {
        // Display ATM functions after successful login
        handleMenu(key);
    }
}

// ======================================================
// PIN Authentication
// ======================================================
void checkPIN(char key)
{
    // Check if any key is pressed
    if(key)
    {
        // '#' submits the entered PIN
        if(key == '#')
        {
            // Correct PIN
            if(inputPin == CorrectPin)
            {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Access Granted");

                inputPin = "";
                delay(2000);

                // Reset failed attempts counter
                wrongAttempts = 0;

                // User successfully logged in
                loggedin = true;

                // Unlock ATM door
                myServo.write(90);

                // Display main menu
                showMenu();
            }

            // Incorrect PIN
            else
            {
                // Increase failed attempts
                wrongAttempts++;

                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Wrong PIN");

                delay(2000);

                // Lock system after three failed attempts
                if(wrongAttempts >= 3)
                {
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("ATM Locked");

                    lcd.setCursor(0,1);
                    lcd.print("Wait 10 Sec");

                    delay(2000);
                    lcd.clear();

                    // Display countdown timer
                    for(int i = 10; i >= 1; i--)
                    {
                        lcd.setCursor(0,0);
                        lcd.print("Wait:");

                        lcd.setCursor(0,1);
                        lcd.print(i);
                        lcd.print(" sec ");

                        delay(1000);
                    }

                    // Reset failed attempts
                    wrongAttempts = 0;
                }

                // Ask user to enter PIN again
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Enter PIN:");

                inputPin = "";
            }
        }

        // Handle PIN entry
        else
        {
            // Delete last entered digit
            if(key == '*')
            {
                if(inputPin.length() > 0)
                {
                    inputPin.remove(inputPin.length() - 1, 1);

                    lcd.setCursor(inputPin.length(), 1);
                    lcd.print(" ");
                }
            }

            // Accept numeric digits only
            else if(key >= '0' && key <= '9')
            {
                if(inputPin.length() < 4)
                {
                    inputPin += key;

                    // Display '*' instead of actual PIN digits
                    lcd.setCursor(inputPin.length() - 1, 1);
                    lcd.print('*');
                }
            }
        }
    }
}

// ======================================================
// Main Menu
// ======================================================
void handleMenu(char key)
{
    // Balance Inquiry
    if(key == 'A')
    {
        checkBalance();
    }

    // Cash Withdrawal
    else if(key == 'B')
    {
        withdrawMoney();
    }

    // Deposit Menu
    else if(key == 'D')
    {
        depositMenu();
    }
   else if(key == '*')
    {
        loggedin = false;

        myServo.write(0);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Logged Out");

        delay(1500);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Enter PIN:");
    }
}

// ======================================================
// Balance Inquiry
// ======================================================
void checkBalance()
{
    char key = 0;

    // Display current balance
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Balance:");

    lcd.setCursor(0,1);
    lcd.print(balance);

    // Wait until '#' is pressed
    while(key != '#')
    {
        key = Mykeypad.getKey();
    }

    // Return to main menu and lock the door
    showMenu();
  
}

// ======================================================
// Cash Withdrawal
// ======================================================
void withdrawMoney()
{
    char key = 0;
    amountInput = "";

    // Ask user to enter withdrawal amount
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter Amount:");

    while(true)
    {
        key = Mykeypad.getKey();

        if(key)
        {
            // Accept numeric input
            if(key >= '0' && key <= '9')
            {
                amountInput += key;

                lcd.setCursor(0,1);
                lcd.print(amountInput);
            }

            // Delete last digit
            else if(key == '*')
            {if (amountInput.length() > 0)
{
    amountInput.remove(amountInput.length() - 1, 1);

    lcd.setCursor(amountInput.length(), 1);
    lcd.print(" ");
    lcd.setCursor(amountInput.length(), 1);
}
            }

            // Confirm entered amount
            else if(key == '#')
            {
                withdrawAmount = amountInput.toInt();
                break;
            }
        }
    }

    // Check if balance is sufficient
    if(withdrawAmount <= balance)
    {
        balance -= withdrawAmount;

        // Save updated balance to EEPROM
        EEPROM.put(0, balance);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("New Balance:");

        lcd.setCursor(0,1);
        lcd.print(balance);

        delay(2000);
    }
    else
    {
        // Display insufficient balance message
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Insufficient");

        lcd.setCursor(0,1);
        lcd.print("Balance:");
        lcd.print(balance);

        delay(2000);
    }

    // Return to menu and lock the door
    showMenu();
    
}

// ======================================================
// Deposit Menu
// ======================================================
void depositMenu()
{
    char key = 0;

    // Display deposit options
    showMenu2();

    while(true)
    {
        key = Mykeypad.getKey();

        // Return to main menu
        if(key == '#')
        {
            showMenu();
            break;
        }

        // Start deposit process
        else if(key == 'C')
        {
            depositMoney();
            break;
        }
    }
}

// ======================================================
// Cash Deposit
// ======================================================
void depositMoney()
{
    char key = 0;
    amountInput = "";
    depositAmount = 0;

    // Ask user to enter deposit amount
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter Amount:");

    while(true)
    {
        key = Mykeypad.getKey();

        if(key)
        {
            // Accept numeric input
            if(key >= '0' && key <= '9')
            {
                amountInput += key;

                lcd.setCursor(0,1);
                lcd.print(amountInput);
            }

            // Delete last digit
            else if(key == '*')
{
    if(amountInput.length() > 0)
    {
        amountInput.remove(amountInput.length() - 1, 1);

        lcd.setCursor(amountInput.length(), 1);
        lcd.print(" ");

        lcd.setCursor(amountInput.length(), 1);
    }
}

            // Confirm entered amount
            else if(key == '#')
            {
                depositAmount = amountInput.toInt();
                break;
            }
        }
    }

    // Update balance
    balance += depositAmount;

    // Save new balance to EEPROM
    EEPROM.put(0, balance);

    // Display updated balance
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("New Balance:");

    lcd.setCursor(0,1);
    lcd.print(balance);

    delay(2000);

    // Return to menu and lock the door
    showMenu();
   
}
// ======================================================
// Display Main Menu
// ======================================================
void showMenu()
{
    // Clear LCD screen
    lcd.clear();

    // Display available ATM options
    lcd.setCursor(0,0);
    lcd.print("A:Balance");

    lcd.setCursor(0,1);
    lcd.print("B:Withdraw");
}

// ======================================================
// Display Deposit Menu
// ======================================================
void showMenu2()
{
    // Clear LCD screen
    lcd.clear();

    // Display deposit options
    lcd.setCursor(0,0);
    lcd.print("C:Deposit");

    lcd.setCursor(0,1);
    lcd.print("#:Back");
}

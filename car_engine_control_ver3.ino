// sketch pornire motor
// apasare scurta buton -> contact pe prima treapta 
// inca o apasare scurta buton -> contact pe a doua treapta
// apasare lunga de 3-4 secunde -> actionare demaror
// apasare scurta -> deconectare consumatori
// apasare iar 2-3 secunde -> oprire motor
// scusrtare timp demaror la 1 secunda si schimbare conectare buton
// conectat afisaj LCD 1602 pe i2c
// versiune 3.0 - 22.08.2016, Craiova, Romania
// autor: Nicu FLORICA (niq_ro) - http://www.tehnic.go.ro
// http://nicuflorica.blogspot.ro/
// http://arduinotehniq.blogspot.com/
// http://www.arduinotehniq.com/

#include <Wire.h>                // folosire librarie pentru protocol i2c
#include <LiquidCrystal_I2C.h>   // folosire librarie pentru afisaj lcd1602 pe i2c

// afisaj 1602 pe i2c cu adresa 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define buton 12    // buton pus la D12
#define contact1 10
#define contact2 9
#define demaror 8
#define motor 11
#define scurt 500   // timp in ms pentru eliminare 


byte cheie;       // daca este 0 este repaus
                  // daca este 1 este contact pe prima treapta (accesorii)
                  // daca este 2 este contact pe treapta 2
                  // daca este 3 motorul este pornit
                 
// These variables are for the push button routine
// variabile folosite pentru partea de apasare buton
byte starebuton = 0; // variabila pentru determinare stare buton (apasat sau nu)
int apasarelunga1 = 1500; // valoare timp apasare lunga in ms pentru pornire motor
int apasarelunga2 = 1000; // valoare timp apasare lunga in ms pentru oprire motor
int apasare = apasarelunga1; // valoare timp apasare buton
int coborare = 0;// valoare pentru timp apasare buton
int ridicare = 0;// valoare pentr timp eliberare buton
boolean starelogica = false; // variabila logica stare buton



void setup()
{
  
lcd.begin();  // initializare afisaj
lcd.backlight(); // aprindere fundal

lcd.setCursor(0, 0);   // pozitionare cursor pe colana a 4-a din stanga - sus
lcd.print("Sistem pornire /"); // afisare text
lcd.setCursor(0, 1);   // mutare cursor pe coloana a 2-a din stanga - jos
lcd.print("oprire motor v.3"); // afisare text

delay(3000);  
lcd.clear();
  
 pinMode(buton, INPUT);
 pinMode(contact1, OUTPUT);
 pinMode(contact2, OUTPUT);
 pinMode(demaror, OUTPUT);
 pinMode(motor, OUTPUT);

 digitalWrite(buton, HIGH); // intrarea butonului in 1 logic
 digitalWrite(contact1, LOW); 
 digitalWrite(contact2, LOW);
 digitalWrite(demaror, LOW);
 digitalWrite(motor, LOW);

cheie = 0;   // la alimentarea modului masina nu este alimentata
lcd.noBacklight(); // aprindere fundal
}

void loop() {
  
if (cheie == 0)
//lcd.noBacklight(); // aprindere fundal
{      // daca echipamentul e in repaus
if (digitalRead(buton) == LOW)
{    // se apasa butonul
delay(scurt);
cheie = 1;    // trecem in starea de accesorii 
digitalWrite(contact1, HIGH);
lcd.backlight(); // aprindere fundal
lcd.clear();
lcd.print("Cheie pe pozitia");
lcd.setCursor(2, 1);
lcd.print("ACCESORII...");
delay(scurt);
}
}

if (cheie == 1)
{      // daca echipamentul e pe accesorii
if (digitalRead(buton) == LOW)
{    // se apasa butonul
delay(scurt);
cheie = 2;    // trecem in starea de contact treapta 2-a
digitalWrite(contact2, HIGH);
lcd.clear();
lcd.print("Cheie pe pozitia");
lcd.setCursor(3, 1);
lcd.print("CONTACT...");
delay(scurt);
}
}

if (cheie == 2)
{      // daca echipamentul e pe contact
    apasare = apasarelunga1;
    apasare = cattimp ();
    delay (10);
  
if (apasare > apasarelunga1) {    
digitalWrite(demaror, HIGH);
lcd.clear();
lcd.print("Cheie pe pozitia");
lcd.setCursor(2, 1);
lcd.print("DEMAROR...");
delay(1000);    // actiomare demaror
digitalWrite(demaror, LOW);
digitalWrite(motor, HIGH);
lcd.clear();
cheie = 3;    // trecem la starea de motor pornit
lcd.print("Motor pornit... ");
    }
if (apasare < apasarelunga1) {    
digitalWrite(contact1, LOW);
digitalWrite(contact2, LOW);
cheie = 0;    // trecem la starea de motor pornit
lcd.noBacklight(); // aprindere fundal
lcd.clear();  
    }
}

if (cheie == 3)
{
  // daca motorul e pornit
    apasare = apasarelunga2;
    apasare = cattimp ();
    delay (10);
  
    if (apasare > apasarelunga2) {    
digitalWrite(motor, LOW);
lcd.clear();
lcd.print("Oprire motor... ");
delay(2000);
lcd.clear();
digitalWrite(contact1, LOW); 
digitalWrite(contact2, LOW);
cheie = 0;     // trecem la starea de repaus
lcd.noBacklight(); // aprindere fundal
    }
}


} // se termina programul


// subrutina care trimite timpul de apasare al butonului
int cattimp () {
  starebuton = digitalRead(buton);  
       if(starebuton == LOW && starelogica==false) {     
              coborare = millis();
              starelogica = true;
          };
          
       if (starebuton == HIGH && starelogica==true) {
         ridicare = millis ();
         apasare = ridicare - coborare;
         starelogica = false;
       };
       return apasare;
}

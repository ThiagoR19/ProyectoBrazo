#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);

// --- Boca tipo robótica ---
byte bocaCerrada[8] = {
  B00000,
  B00000,
  B11111,
  B10001,
  B10001,
  B11111,
  B00000,
  B00000
};

byte bocaAbierta[8] = {
  B00000,
  B00000,
  B11111,
  B10101,
  B10101,
  B11111,
  B00000,
  B00000
};

void setup() {
  lcd.init();
  lcd.backlight();

  // Crear caracteres personalizados
  lcd.createChar(0, bocaCerrada);
  lcd.createChar(1, bocaAbierta);

  // --- Hola + boca simultáneo ---
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Hola");

  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) lcd.write(byte(0)); // Boca cerrada llena
  delay(2000);

  // --- ": ]" ---
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print(": ]");
  delay(1000);

  // --- JoJo en la línea 0 ---
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("JoJo:");

  // --- Texto desplazado en línea 1 ---
  String mensaje = "Asistente Robotico";
  int total = mensaje.length();

  for (int pos = 16; pos > -total; pos--) {
    // Limpiar sólo la línea inferior
    lcd.setCursor(0, 1);
    lcd.print("                "); // 16 espacios

    // Mantener JoJo fijo
    lcd.setCursor(5, 0);
    lcd.print("JoJo:");

    // Mostrar el mensaje desplazado
    lcd.setCursor(pos, 1);
    lcd.print(mensaje.substring(max(0, -pos), min(total, 16 - pos + total)));

    delay(150);
  }

  // --- Pausa con pantalla vacía ---
  lcd.clear();
  delay(2000);

  // --- Boca grande animada (dos líneas) ---
  
}

void loop() {}

/****************************************************
 * Robot Asistencia – JoJo
 ****************************************************/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

// ---- LCD I2C (opcional) ----
/* Si NO usas LCD, deja USE_LCD_I2C en false */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---- (Opcional TFT) hooks/plantilla ----
// #include <Adafruit_GFX.h>
// #include <Adafruit_ILI9341.h>  // o tu driver
// Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

/* ===========================
   ========== CONFIG ==========
   =========================== */

// --- WiFi / Servidor ---
namespace NetCfg {
  const char* SSID      = "Zhone_0A92";
  const char* PASSWORD  = "48354583";
  const char* CONTROL_URL = "http://servidor.com/robot_estado.php";
  const uint32_t POLL_MS  = 3000;  // cada 3 s
}

// --- Componentes ---
namespace Features {
  const bool USE_LCD_I2C = true;
  const bool USE_TFT     = false;  // activa y añade tu driver si quieres
  const bool USE_AUDIO   = true;   // placeholder de parlante (PWM/DC)
  const bool USE_MIC     = true;   // solo modelo
}

// Motores DC
namespace PinsMotor {
  // Motor Izquierdo
  const int IN1 = 25;     // In1
  const int IN2 = 26;     // In2
  const int PWM = 27;     // PWM izq (ledc)
  // Motor Derecho
  const int IN3 = 32;     // In3
  const int IN4 = 33;     // In4
  const int PWM_R = 14;   // PWM der (ledc)
  // Canales LEDC (0..15 en ESP32)
  const int CH_L = 0;
  const int CH_R = 1;
  // Frecuencia PWM y resolución
  const int FREQ = 20000;       // 20 kHz para silencio
  const int RES  = 8;           // 8 bits (0..255)
}

// Ultrasónico (HC-SR04 o similar)
namespace PinsUltra {
  const int TRIG = 12;
  const int ECHO = 13;
  const long MAX_CM = 300;    // distancia máxima útil
  const long SAFE_CM = 25;    // umbral obstáculo
}

// Sensor IR de cornisa (digital).
namespace PinsIR {
  const int IR_PIN = 4;
  const bool ACTIVE_LOW = true;
}

// Servos 
namespace PinsServo {
  const int S1 = 15;
  const int S2 = 2;
  const int S3 = 0;   // ojo: algunos boots usan GPIO0
  const int S4 = 16;
  const int MIN_US = 500;
  const int MAX_US = 2400;
}

// LCD I2C
namespace LcdCfg {
  const uint8_t I2C_ADDR = 0x27; // cambia si es 0x3F
  const int COLS = 16;
  const int ROWS = 2;
}

// Parlante (placeholder PWM o pin digital a amplificador)
namespace PinsAudio {
  const int SPK_PWM = 5;     // ledc canal 2
  const int LEDC_CH = 2;
  const int FREQ    = 4000;  // 4 kHz beep
  const int RES     = 8;
}

// Micrófono (no implementado audio real)
namespace PinsMic {
  const int ADC_PIN = 34; // mic analógico
}

/*Utilidades*/

static inline long cmFromEchoUs(long us) {
  // Sonido ~343 m/s: ida-vuelta -> 58 us por cm aprox.
  return us / 58;
}

/* ===========================
   ======= CLASES/HAL ========
   =========================== */

// ----- LCD -----
class LCD16x2 {
public:
  LCD16x2(): lcd(LcdCfg::I2C_ADDR, LcdCfg::COLS, LcdCfg::ROWS) {}
  void begin() {
    if (!Features::USE_LCD_I2C) return;
    Wire.begin(); // SDA/SCL por defecto de la placa
    lcd.init();
    lcd.backlight();
    lcd.clear();
    print(0,0,"Robot listo");
  }
  void print(uint8_t col, uint8_t row, const String& s) {
    if (!Features::USE_LCD_I2C) return;
    lcd.setCursor(col, row);
    lcd.print("                ");
    lcd.setCursor(col, row);
    lcd.print(s.substring(0, LcdCfg::COLS));
  }
private:
  LiquidCrystal_I2C lcd;
};

// ----- TFT (hooks mínimos) -----
class TFTDisplay {
public:
  void begin() {
    if (!Features::USE_TFT) return;
    // tft.begin(); tft.setRotation(1); etc.
  }
  void status(const String& line1, const String& line2="") {
    if (!Features::USE_TFT) return;
    // Dibuja textos si activas la TFT
  }
};

// ----- Ultrasonico -----
class Ultrasonic {
public:
  Ultrasonic(int trig, int echo): _trig(trig), _echo(echo) {}
  void begin() {
    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
    digitalWrite(_trig, LOW);
  }
  long readCM() {
    // Trigger
    digitalWrite(_trig, LOW); delayMicroseconds(2);
    digitalWrite(_trig, HIGH); delayMicroseconds(10);
    digitalWrite(_trig, LOW);
    // Echo
    long dur = pulseIn(_echo, HIGH, PinsUltra::MAX_CM * 58UL * 2);
    if (dur == 0) return PinsUltra::MAX_CM;  // timeout => lejos
    return cmFromEchoUs(dur);
  }
private:
  int _trig, _echo;
};

// ----- Sensor IR cornisa -----
class IRSensor {
public:
  IRSensor(int pin, bool activeLow): _pin(pin), _activeLow(activeLow) {}
  void begin() { pinMode(_pin, INPUT_PULLUP); } // ajusta si tu módulo lo requiere
  bool cliffDetected() {
    int v = digitalRead(_pin);
    return _activeLow ? (v == LOW) : (v == HIGH);
  }
private:
  int _pin; bool _activeLow;
};

// ----- Motores DC con ledc -----
class MotorDriver {
public:
  MotorDriver(int in1, int in2, int pwmPin, int ch):
    _in1(in1), _in2(in2), _pwm(pwmPin), _ch(ch) {}

  void begin() {
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    ledcSetup(_ch, PinsMotor::FREQ, PinsMotor::RES);
    ledcAttachPin(_pwm, _ch);
    stop();
  }

  void drive(int8_t dir, uint8_t speed) {
    // dir: -1 atrás, 0 freno, +1 adelante
    dir = constrain(dir, -1, 1);
    if (dir > 0) { digitalWrite(_in1, HIGH); digitalWrite(_in2, LOW); }
    else if (dir < 0) { digitalWrite(_in1, LOW); digitalWrite(_in2, HIGH); }
    else { digitalWrite(_in1, LOW); digitalWrite(_in2, LOW); }
    ledcWrite(_ch, speed);
  }

  void stop() { drive(0, 0); }

private:
  int _in1, _in2, _pwm, _ch;
};

class DifferentialDrive {
public:
  DifferentialDrive(MotorDriver& left, MotorDriver& right)
    : L(left), R(right) {}

  void begin() {}
  void move(int8_t dir, uint8_t speed, int8_t turnBias = 0) {
    // turnBias -100..100 (neg: gira a izquierda, pos: a derecha)
    turnBias = constrain(turnBias, -100, 100);
    int leftSpeed = speed;
    int rightSpeed = speed;

    if (turnBias < 0) rightSpeed = speed * (100 + turnBias) / 100;
    else if (turnBias > 0) leftSpeed = speed * (100 - turnBias) / 100;

    L.drive(dir, constrain(leftSpeed, 0, 255));
    R.drive(dir, constrain(rightSpeed, 0, 255));
  }
  void pivot(int8_t dirLeft, int8_t dirRight, uint8_t speed) {
    L.drive(dirLeft, speed);
    R.drive(dirRight, speed);
  }
  void stop() { L.stop(); R.stop(); }

private:
  MotorDriver& L; MotorDriver& R;
};

// ----- Servos -----
class ServoManager {
public:
  void begin() {
    S1.setPeriodHertz(50); S2.setPeriodHertz(50);
    S3.setPeriodHertz(50); S4.setPeriodHertz(50);
    S1.attach(PinsServo::S1, PinsServo::MIN_US, PinsServo::MAX_US);
    S2.attach(PinsServo::S2, PinsServo::MIN_US, PinsServo::MAX_US);
    S3.attach(PinsServo::S3, PinsServo::MIN_US, PinsServo::MAX_US);
    S4.attach(PinsServo::S4, PinsServo::MIN_US, PinsServo::MAX_US);
  }
  void setAngles(int a1, int a2, int a3, int a4) {
    S1.write(a1); S2.write(a2); S3.write(a3); S4.write(a4);
  }
private:
  Servo S1, S2, S3, S4;
};

// ----- Audio (placeholder) -----
class AudioOut {
public:
  void begin() {
    if (!Features::USE_AUDIO) return;
    ledcSetup(PinsAudio::LEDC_CH, PinsAudio::FREQ, PinsAudio::RES);
    ledcAttachPin(PinsAudio::SPK_PWM, PinsAudio::LEDC_CH);
    mute();
  }
  void beep(uint8_t vol = 100, uint16_t ms = 120) {
    if (!Features::USE_AUDIO) return;
    ledcWrite(PinsAudio::LEDC_CH, vol);
    delay(ms);
    mute();
  }
  void say(const String& phrase) {
    if (!Features::USE_AUDIO) return;
    // Coloca aquí tu TTS o reproducción de archivo WAV/MP3:
    // Ej: I2S + DAC + archivo en SPIFFS/SD
    // Por ahora: un “beep” corto como confirmación.
    beep(180, 100);
  }
  void mute() { if (Features::USE_AUDIO) ledcWrite(PinsAudio::LEDC_CH, 0); }
};

// ----- Mic (modelo) -----
class MicrophoneIn {
public:
  void begin() {
    if (!Features::USE_MIC) return;
    // Configura ADC o I2S aquí si lo usarás luego.
    // pinMode(PinsMic::ADC_PIN, INPUT);
  }
  int sample() {
    if (!Features::USE_MIC) return 0;
    // return analogRead(PinsMic::ADC_PIN);
    return 0; // placeholder
  }
};

// ----- Red / Control remoto -----
struct RemoteState {
  bool move = false;
  String phrase = "";
  int servos[4] = {90,90,90,90};
  int speed = 160;
  int turnBias = 0; // -100..100
};

class ControlClient {
public:
  void begin() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(NetCfg::SSID, NetCfg::PASSWORD);
  }
  bool ready() { return (WiFi.status() == WL_CONNECTED); }

  bool fetch(RemoteState& out) {
    if (!ready()) return false;
    HTTPClient http;
    http.setTimeout(2500);
    http.begin(NetCfg::CONTROL_URL);
    int code = http.GET();
    if (code != 200) {
      http.end();
      return false;
    }
    String payload = http.getString();
    http.end();
    // ---- Parseo MUY simple (si quieres, usa ArduinoJson) ----
    // Busca claves de forma robusta en producción; aquí, mínimo viable.
    out.move     = payload.indexOf("\"move\":true") >= 0;
    out.speed    = extractInt(payload, "\"speed\":", out.speed);
    out.turnBias = extractInt(payload, "\"turnBias\":", out.turnBias);
    out.servos[0]= extractIntFromArray(payload, "\"servos\"", 0, out.servos[0]);
    out.servos[1]= extractIntFromArray(payload, "\"servos\"", 1, out.servos[1]);
    out.servos[2]= extractIntFromArray(payload, "\"servos\"", 2, out.servos[2]);
    out.servos[3]= extractIntFromArray(payload, "\"servos\"", 3, out.servos[3]);
    out.phrase   = extractString(payload, "\"phrase\":\"", "\"", out.phrase);
    return true;
  }

private:
  // Helpers de parseo naive
  static int extractInt(const String& s, const String& key, int deflt) {
    int p = s.indexOf(key);
    if (p < 0) return deflt;
    p += key.length();
    int e = p;
    while (e < (int)s.length() && (isDigit(s[e]) || s[e]=='-' )) e++;
    return s.substring(p, e).toInt();
  }
  static int extractIntFromArray(const String& s, const String& arrKey, int idx, int deflt) {
    int p = s.indexOf(arrKey);
    if (p < 0) return deflt;
    p = s.indexOf('[', p); if (p < 0) return deflt; p++;
    // Busca idx-ésimo número
    for (int i=0;i<idx;i++) {
      p = s.indexOf(',', p);
      if (p < 0) return deflt;
      p++;
    }
    int e = p;
    while (e < (int)s.length() && (isDigit(s[e]) || s[e]=='-')) e++;
    return s.substring(p, e).toInt();
  }
  static String extractString(const String& s, const String& startKey, const String& endKey, const String& deflt) {
    int p = s.indexOf(startKey);
    if (p < 0) return deflt;
    p += startKey.length();
    int e = s.indexOf(endKey, p);
    if (e < 0) return deflt;
    return s.substring(p, e);
  }
};

/* ===========================
   ========= LÓGICA ==========
   =========================== */

enum class RobotState { IDLE, FORWARD, AVOIDING, BACKING };

LCD16x2 lcd;
TFTDisplay tft;
Ultrasonic ultra(PinsUltra::TRIG, PinsUltra::ECHO);
IRSensor ir(PinsIR::IR_PIN, PinsIR::ACTIVE_LOW);

MotorDriver mL(PinsMotor::IN1, PinsMotor::IN2, PinsMotor::PWM, PinsMotor::CH_L);
MotorDriver mR(PinsMotor::IN3, PinsMotor::IN4, PinsMotor::PWM_R, PinsMotor::CH_R);
DifferentialDrive drive(mL, mR);

ServoManager servos;
AudioOut audio;
MicrophoneIn mic;
ControlClient net;

RobotState state = RobotState::IDLE;
RemoteState remote;

uint32_t lastPoll = 0;
uint32_t avoidStart = 0;
uint32_t backingStart = 0;

void setup() {
  Serial.begin(115200);
  lcd.begin();
  tft.begin();
  ultra.begin();
  ir.begin();

  mL.begin(); mR.begin(); drive.begin();
  servos.begin();
  audio.begin();
  mic.begin();

  net.begin();

  lcd.print(0,0,"Conectando WiFi");
  waitForWiFi(8000);
  lcd.print(0,0, (net.ready() ? "WiFi OK" : "Sin WiFi") );
  lcd.print(0,1,"Listo.");
  // tft.status("Robot listo","WiFi: " + String(net.ready()?"OK":"NO"));
}

void loop() {
  // 1) Poll servidor cada 3s
  if (millis() - lastPoll >= NetCfg::POLL_MS) {
    lastPoll = millis();
    if (net.fetch(remote)) {
      // Aplica ángulos de servos recibidos (tú luego programa cinemática):
      servos.setAngles(remote.servos[0], remote.servos[1], remote.servos[2], remote.servos[3]);

      if (remote.move) {
        if (remote.phrase.length()) {
          audio.say(remote.phrase); // “en camino”, “enseguida”, etc.
          lcd.print(0,0, remote.phrase);
        }
        state = RobotState::FORWARD; // habilita movimiento
      } else {
        state = RobotState::IDLE;
      }
    } else {
      // Fallo de red: mantén último estado o entra en IDLE
      // state = RobotState::IDLE;
      lcd.print(0,1,"Red err/poll");
    }
  }

  // 2) Sensado rápido
  long dist = ultra.readCM();
  bool cliff = ir.cliffDetected();

  // 3) FSM de movimiento
  switch (state) {
    case RobotState::IDLE:
      drive.stop();
      break;

    case RobotState::FORWARD:
      if (cliff) {
        // Detener y retroceder un poco
        drive.stop();
        backingStart = millis();
        state = RobotState::BACKING;
        break;
      }
      if (dist <= PinsUltra::SAFE_CM) {
        // Obstáculo: pivotar a la izquierda
        avoidStart = millis();
        state = RobotState::AVOIDING;
        break;
      }
      // Avanza con sesgo (turnBias) si te lo envían
      drive.move(+1, (uint8_t)constrain(remote.speed,0,255), (int8_t)remote.turnBias);
      break;

    case RobotState::AVOIDING:
      // Gira izquierda en el lugar durante ~400 ms
      drive.pivot(-1, +1, (uint8_t)constrain(remote.speed,0,255));
      if (millis() - avoidStart > 400) {
        state = RobotState::FORWARD;
      }
      break;

    case RobotState::BACKING:
      // Retrocede 300 ms y luego intenta esquivar
      drive.move(-1, (uint8_t)constrain(remote.speed,0,255));
      if (millis() - backingStart > 300) {
        avoidStart = millis();
        state = RobotState::AVOIDING;
      }
      break;
  }

  // 4) UI básica
  // tft.status(String("d:")+dist+"cm", stateToStr(state));
  delay(5); // lazo suave
}

/* ===========================
   ======= HELPERS UI ========
   =========================== */

String stateToStr(RobotState s) {
  switch(s) {
    case RobotState::IDLE: return "IDLE";
    case RobotState::FORWARD: return "FORWARD";
    case RobotState::AVOIDING: return "AVOID";
    case RobotState::BACKING: return "BACK";
  }
  return "?";
}

void waitForWiFi(uint32_t timeoutMs) {
  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && (millis()-t0) < timeoutMs) {
    delay(200);
  }
}

// Pines del Motor 1 (izquierdo)
const int motor1_in1 = 50;
const int motor1_in2 = 51;
const int motor1_pwm = 3; // PWM A

// Pines del Motor 2 (derecho)
const int motor2_in3 = 52;
const int motor2_in4 = 53;
const int motor2_pwm = 2; // PWM B

void setup() {
  // Configurar pines como salida
  pinMode(motor1_in1, OUTPUT);
  pinMode(motor1_in2, OUTPUT);
  pinMode(motor1_pwm, OUTPUT);
  
  pinMode(motor2_in3, OUTPUT);
  pinMode(motor2_in4, OUTPUT);
  pinMode(motor2_pwm, OUTPUT);

  Serial.begin(9600);
  Serial.println("Iniciando control de motores...");
}

void loop() {
  // 🚗 Ambos motores hacia adelante
  Serial.println("Ambos motores hacia adelante...");
  digitalWrite(motor1_in1, HIGH);
  digitalWrite(motor1_in2, LOW);
  digitalWrite(motor2_in3, HIGH);
  digitalWrite(motor2_in4, LOW);
  analogWrite(motor1_pwm, 200); // Velocidad (0-255)
  analogWrite(motor2_pwm, 200);
  delay(2000);

  // ⏪ Ambos motores hacia atrás
  Serial.println("Ambos motores hacia atrás...");
  digitalWrite(motor1_in1, LOW);
  digitalWrite(motor1_in2, HIGH);
  digitalWrite(motor2_in3, LOW);
  digitalWrite(motor2_in4, HIGH);
  delay(5000);

  // 🔄 Giro: motores en direcciones opuestas
  Serial.println("Girando (motores en sentidos opuestos)...");
  digitalWrite(motor1_in1, HIGH);
  digitalWrite(motor1_in2, LOW);
  digitalWrite(motor2_in3, LOW);
  digitalWrite(motor2_in4, HIGH);
  delay(4000);

  // 🛑 Parar motores
  Serial.println("Parando motores...");
  digitalWrite(motor1_in1, LOW);
  digitalWrite(motor1_in2, LOW);
  digitalWrite(motor2_in3, LOW);
  digitalWrite(motor2_in4, LOW);
  delay(3000);
}

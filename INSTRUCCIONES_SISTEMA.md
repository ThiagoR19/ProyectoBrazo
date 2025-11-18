# 🤖 INSTRUCCIONES PARA LEVANTAR EL SISTEMA JOJO

## 📋 REQUISITOS PREVIOS

### Hardware necesario:
- ✅ Router JOJO_NET encendido
- ✅ PC con WiFi conectada a JOJO_NET
- ✅ ESP32 Principal con código cargado y alimentado
- ✅ ESP32-CAM con código cargado y alimentado
- ✅ DFPlayer con tarjeta SD (tracks: 1=HOLA, 2=AYUDA, 3=ACTIVA CAMARA)
- ✅ L298N conectado a motores
- ✅ Servo en GPIO 13 (opcional)

### Configuración de red (DHCP Reservations):
```
Router:          192.168.0.1 (gateway JOJO_NET)
PC:              192.168.0.102
Raspberry Pi 2:  192.168.0.100
ESP32-CAM:       192.168.0.101
ESP32 Principal: 192.168.0.103
```

---

## 🚀 PASO 1: CONECTAR PC A JOJO_NET

### 1.1 Desconectar de red actual
- Click en el icono WiFi (barra de tareas)
- Desconectar de red con internet

### 1.2 Conectar a JOJO_NET
- Seleccionar red: `JOJO_NET`
- Contraseña: `jojo2025`
- Esperar conexión

### 1.3 Verificar IP asignada
Abrir **PowerShell** y ejecutar:
```powershell
ipconfig
```

**DEBE MOSTRAR:**
```
Adaptador de LAN inalámbrica Wi-Fi:
   Puerta de enlace predeterminada: 192.168.0.1
   Dirección IPv4: 192.168.0.102
```

⚠️ **Si la IP es diferente:** El router no tiene la reserva DHCP configurada.

---

## 🚀 PASO 2: INICIAR MOSQUITTO MQTT BROKER

### 2.1 Abrir PowerShell
- Presionar `Win + X`
- Seleccionar "Windows PowerShell"

### 2.2 Navegar a la carpeta del proyecto
```powershell
cd "d:\FERIA DE CIENCIAS 2025\JOJO_2025_OK\proyjojo"
```

### 2.3 Iniciar Mosquitto
```powershell
& "C:\Program Files\mosquitto\mosquitto.exe" -c mosquitto_dev.conf -v
```

**SALIDA ESPERADA:**
```
1699999999: mosquitto version 2.0.18 starting
1699999999: Config loaded from mosquitto_dev.conf
1699999999: Opening ipv4 listen socket on port 1884.
1699999999: mosquitto 2.0.18 running
```

✅ **Broker corriendo en puerto 1884**

⚠️ **NO CERRAR esta ventana PowerShell** (Mosquitto debe quedar corriendo)

---

## 🚀 PASO 3: INICIAR FLASK (WEB APP)

### 3.1 Abrir NUEVA PowerShell
- Presionar `Win + X`
- Seleccionar "Windows PowerShell" (segunda ventana)

### 3.2 Navegar al proyecto
```powershell
cd "d:\FERIA DE CIENCIAS 2025\JOJO_2025_OK\proyjojo"
```

### 3.3 Iniciar Flask
```powershell
& "D:\FERIA DE CIENCIAS 2025\JOJO_2025_OK\.venv\Scripts\python.exe" run.py
```

**SALIDA ESPERADA:**
```
 * Serving Flask app 'app'
 * Debug mode: off
WARNING: This is a development server.
 * Running on http://192.168.0.102:5000
Press CTRL+C to quit
```

✅ **Flask corriendo en http://192.168.0.102:5000**

⚠️ **NO CERRAR esta ventana PowerShell** (Flask debe quedar corriendo)

---

## 🚀 PASO 4: VERIFICAR ESP32 PRINCIPAL

### 4.1 Conectar ESP32 por USB (opcional)
Si querés ver los logs del ESP32:
- Conectar ESP32 a PC por USB
- Abrir Arduino IDE → Tools → Serial Monitor (115200 baudios)

### 4.2 Verificar conexión WiFi
**DEBE MOSTRAR en Serial Monitor:**
```
[JOJO COMPLETO v3 - Ruedas + Audio + Servo] Iniciando...
Servo OK (posición 0°)
DFPlayer OK.
Conectando a WiFi JOJO_NET
.....
¡WiFi conectado!
192.168.0.103
```

### 4.3 Verificar conexión MQTT
**DEBE MOSTRAR:**
```
Reconectando MQTT... OK
Suscrito a todos los temas.
```

✅ **ESP32 conectado a broker MQTT**

❌ **Si dice "rc=-2":** Mosquitto NO está corriendo o está en puerto incorrecto

---

## 🚀 PASO 5: VERIFICAR ESP32-CAM

### 5.1 Abrir navegador
En la PC conectada a JOJO_NET, abrir navegador web.

### 5.2 Probar stream de cámara
Ir a: `http://192.168.0.101:81/stream`

✅ **Deberías ver el video de la cámara**

❌ **Si no carga:** Reiniciar ESP32-CAM o verificar que esté alimentada.

---

## 🚀 PASO 6: ABRIR WEB APP

### 6.1 Abrir navegador
En la PC conectada a JOJO_NET.

### 6.2 Ir a la aplicación
URL: `http://192.168.0.102:5000`

### 6.3 Login
- Usuario: (el que creaste)
- Contraseña: (la que pusiste)

### 6.4 Seleccionar robot JOJO
- Click en "JOJO"
- Entrar a sección "Control de Robot"

---

## 🎮 PASO 7: PROBAR CONTROLES

### 7.1 Probar movimiento
- Click en flechas: ↑ ↓ ← →
- Verificar que los motores se muevan
- Click en STOP (o barra espaciadora)

### 7.2 Probar audio
- Click en botón "HOLA" → Track 1
- Click en botón "AYUDA" → Track 2
- Click en botón "ACTIVA CÁMARA" → Track 3

### 7.3 Probar voz
- Mantener presionado botón de micrófono
- Decir: "adelante", "atrás", "izquierda", "derecha", "parar"
- Decir: "hola", "ayuda", "activa cámara"
- Soltar botón

### 7.4 Probar cámara
- Click en "Iniciar Cámara"
- Verificar que se vea el video
- Click en "Detener Cámara"

---

## 🔧 TROUBLESHOOTING

### ❌ Problema: ESP32 no conecta WiFi
**Solución:**
1. Verificar que JOJO_NET esté encendido
2. Verificar contraseña en código ESP32: `jojo2025`
3. Reiniciar ESP32

### ❌ Problema: ESP32 dice "rc=-2"
**Solución:**
1. Verificar que Mosquitto esté corriendo (Paso 2)
2. Verificar puerto 1884 en mosquitto_dev.conf
3. Verificar que PC tenga IP 192.168.0.102

### ❌ Problema: Flask no inicia
**Solución:**
1. Verificar que el entorno virtual esté activado
2. Verificar que Mosquitto esté corriendo PRIMERO
3. Reintentar ejecutar run.py

### ❌ Problema: ESP32-CAM no muestra video
**Solución:**
1. Verificar que ESP32-CAM esté alimentada (5V)
2. Ir directamente a http://192.168.0.101:81/stream
3. Reiniciar ESP32-CAM
4. Verificar que tenga IP 192.168.0.101

### ❌ Problema: PC tiene IP diferente a 192.168.0.102
**Solución:**
1. Configurar reserva DHCP en router
2. Reiniciar PC
3. Reconectar a JOJO_NET

### ❌ Problema: Comandos no llegan al robot
**Solución:**
1. Verificar Serial Monitor del ESP32 (debe decir "OK" en MQTT)
2. Verificar que Flask esté corriendo
3. Verificar que Mosquitto esté corriendo
4. Revisar consola del navegador (F12) para errores

---

## 📊 CHECKLIST DE VERIFICACIÓN

Antes de usar el sistema, verificar:

- [ ] Router JOJO_NET encendido
- [ ] PC conectada a JOJO_NET (192.168.0.102)
- [ ] Mosquitto corriendo (puerto 1884)
- [ ] Flask corriendo (puerto 5000)
- [ ] ESP32 conectado a WiFi (192.168.0.103)
- [ ] ESP32 conectado a MQTT (rc=0)
- [ ] ESP32-CAM streaming (http://192.168.0.101:81/stream)
- [ ] DFPlayer con tarjeta SD insertada
- [ ] Motores alimentados
- [ ] L298N conectado correctamente

---

## 🔄 APAGAR EL SISTEMA

### Orden correcto:
1. Cerrar navegador web
2. En PowerShell de Flask: `Ctrl + C`
3. En PowerShell de Mosquitto: `Ctrl + C`
4. Desconectar alimentación del ESP32
5. Desconectar alimentación del ESP32-CAM
6. Apagar router (opcional)

---

## 📝 NOTAS IMPORTANTES

- **SIN INTERNET:** JOJO_NET NO tiene internet, es una red local.
- **IPs FIJAS:** Configuradas por DHCP Reservation en el router.
- **PUERTO MQTT:** 1884 (NO 1883, para evitar conflicto con servicio Windows).
- **DEBUG MODE:** Flask corre con `debug=False` para estabilidad MQTT.
- **ARCHIVOS DE CONFIGURACIÓN:**
  - `.env` → Configuración actual (desarrollo en PC)
  - `.env.produccion` → Para cuando migres a Raspberry Pi
  - `mosquitto_dev.conf` → Configuración del broker MQTT

---

## 🎯 PRÓXIMAS IMPLEMENTACIONES

- [ ] Sensor ultrasónico HC-SR04 (mapeo de distancias)
- [ ] Display LCD 16x2 I2C (visualización de estado)
- [ ] Migración a Raspberry Pi 2 (192.168.0.100)
- [ ] Modo autónomo (navegación sin control manual)

---

**Última actualización:** 16 de noviembre de 2025  
**Proyecto:** JOJO - Feria de Ciencias 2025  
**Red:** JOJO_NET (192.168.0.X)

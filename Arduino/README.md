# Carpeta Arduino

Contiene el firmware desarrollado en C++ para el microcontrolador ESP32, encargado de gestionar el hardware del brazo robótico y servir la aplicación web embebida.

## Archivo principal

- `ProyectoBrazo.ino`: Implementa el servidor web asíncrono, la lógica de control del brazo y la comunicación con los módulos periféricos.

## Bibliotecas requeridas

- `WiFi.h`
- `ESPAsyncWebServer.h`
- `ESP32Servo.h`
- `SPIFFS.h`

## Funciones principales

- Configuración y manejo del servidor web embebido.
- Control individual de los servomotores (4DOF).
- Gestión de las rutas HTTP para comandos desde la interfaz.
- Comunicación con módulos adicionales: DFPlayer, TFT y ESP32-CAM.

## Recomendaciones

- Verificar la correcta asignación de pines antes de cargar el firmware.
- Utilizar una fuente de alimentación externa para los servos.
- Montar los archivos web en SPIFFS antes de ejecutar el sistema.

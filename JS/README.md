# Carpeta JavaScript

Incluye la lógica de control y comunicación del sistema de interfaz web con el microcontrolador ESP32.

## Archivos principales

- `Index.js`: Controla los eventos de la interfaz y la comunicación con el servidor web embebido en el ESP32.

## Funcionalidad

- Captura las acciones del usuario desde la interfaz (botones, sliders, selectores).
- Envía comandos al ESP32 mediante peticiones `fetch()` HTTP.
- Gestiona actualizaciones dinámicas en la interfaz, reflejando el estado actual del brazo robótico.

## Consideraciones

- Utilizar programación asíncrona para evitar bloqueos en la interfaz.
- Validar las respuestas del servidor antes de actualizar el estado visual.
- Mantener la modularidad del código para futuras expansiones del sistema.

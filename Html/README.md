# Carpeta HTML

Esta carpeta contiene la estructura de la interfaz web utilizada para el control remoto del brazo robótico.

## Archivos principales

- `Index.html`: Página base del sistema de control.  
  Incluye los componentes visuales y los enlaces a los estilos (CSS) y scripts (JS).

## Descripción técnica

El archivo HTML define la interfaz del usuario que interactúa con el microcontrolador ESP32 mediante peticiones HTTP locales.  
Los elementos del DOM están organizados para permitir el control directo del brazo robótico y otras funciones asociadas al proyecto Austral AsisTech.

## Buenas prácticas

- Mantener las rutas relativas correctas hacia los archivos CSS y JS.
- Asegurar la compatibilidad con navegadores modernos.
- Evitar incluir lógica dentro del HTML; concentrarla en los scripts de la carpeta `/JS`.
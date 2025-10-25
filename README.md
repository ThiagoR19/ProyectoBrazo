# Proyecto Brazo Robótico – AsisTech

Este repositorio forma parte del proyecto **Austral AsisTech**, una iniciativa de robótica asistencial orientada a mejorar la calidad de vida de los adultos mayores a través de “JOJO”, un asistente robótico de bajo costo.  

El presente módulo corresponde al **sistema de control y operación del brazo robótico**, desarrollado mediante tecnologías web (HTML, CSS, JavaScript) e integrado con un microcontrolador **ESP32** programado desde **Arduino IDE**.

## Estructura del Repositorio

ProyectoBrazo/
├── Html/ Archivos de la interfaz web.
├── CSS/ Estilos visuales y adaptabilidad del sistema.
├── JS/ Lógica y comunicación entre la interfaz y el hardware.
├── Arduino/ Código fuente del firmware para el microcontrolador.

## Objetivo

Desarrollar una interfaz web funcional y de bajo costo que permita controlar un brazo robótico de 4 grados de libertad (4DOF) alojada directamente en un microcontrolador ESP32, integrando comunicación en red local sin necesidad de conexión externa.

## Tecnologías Utilizadas

- **Frontend:** HTML5, CSS3, JavaScript.
- **Backend embebido:** Servidor Web Asíncrono (ESPAsyncWebServer) en ESP32.
- **Hardware:** ESP32, servomotores MG90S, DFPlayer Mini, ESP32-CAM.
- **Entorno de desarrollo:** Arduino IDE con core para ESP32.
- **Control de versiones:** Git y GitHub.


# Funcionalidades Principales

- Control de movimiento del brazo robótico (4DOF).
- Comunicación bidireccional entre el servidor web y el microcontrolador.
- Interfaz de usuario simple, accesible y responsiva.
- Envío de comandos mediante peticiones HTTP al ESP32.
- Visualización e interacción en tiempo real.

# Créditos
Autores: Nahuel Gallo, Thiago Riffo
Docente asesor: Prof. Nicolás Cussi
Institución: Colegio Técnico Provincial Olga Bronzovich de Arko – Ushuaia, Tierra del Fuego
Especialidad: Programación
Año: 2025

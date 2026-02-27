# Drawing Machine

Plotter de dibujo construido durante el secundario. La máquina mueve un marcador en los ejes X e Y usando motores paso a paso, y sube/baja el marcador con un servo, todo controlado por un Arduino con una CNC Shield.

## Hardware

- Arduino Uno
- CNC Shield con drivers A4988
- 2 motores paso a paso (ejes X e Y)
- 1 servo (eje Z, para subir/bajar el marcador)
- 2 finales de carrera (home X e Y)
- Estructura mecánica propia (ver carpeta `parts3d/`)

### Área de dibujo

- X: 0 – 228 mm
- Y: 0 – 244 mm

## Software

### Firmware (Arduino)

El archivo principal es [`src/drawing_machine.ino`](src/drawing_machine.ino).  
Recibe comandos G-code por puerto serie (9600 baud) y los ejecuta.  
Al iniciar, la máquina hace homing automático yendo al origen (0, 0) con los finales de carrera.

Parámetros configurables dentro del `.ino`:

| Parámetro | Valor por defecto | Descripción |
|---|---|---|
| `modoDePaso` | 16 (1/16 step) | Microstepping |
| `stepDelay` | `1000 / modoDePaso` | Velocidad de los motores |
| `StepsPerMillimeterX/Y` | `~50.4 steps/mm` | Calibración |
| `penZUp / penZDown` | 90° / 0° | Posición del servo |

### Envío del G-code (Python)

El script [`src/sender.py`](src/sender.py) lee un archivo G-code línea por línea y lo envía al Arduino por serial, esperando confirmación (`ok`) antes de enviar la siguiente línea.

Requiere la librería `pyserial`:

```bash
pip install pyserial
```

Antes de correrlo, ajustar el puerto COM en el script y pasarle la ruta al archivo G-code.

## Flujo de uso

1. **Generar el G-code** en Inkscape con el plugin 4xiDraw. Tutorial: https://www.youtube.com/watch?v=bhzCfIEzs0w

2. **Filtrar arcos** (convertir arcos a segmentos rectos): https://constant-flow.github.io/arc-breaker/

3. **Conectar el Arduino** y correr el script Python:
   ```bash
   python src/Prueba_esperar_al_ok.py
   ```

## Estructura del repositorio

```
src/        → Firmware Arduino y script Python
parts3d/    → Modelos 3D de las piezas
shield/     → Diseño del PCB (Altium)
drawings/   → G-codes e imágenes de dibujos realizados
```

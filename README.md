# Proyecto 1
Creamos un proyecto que crea un screensaver insipirado en el juego de Snake. Hicimos un programa secuencial
y uno paralelo con OpenMP.

## Instrucciones para compilar

snakes.cpp
```bash
  gcc -o snakes snakes.cpp -lSDL2
```
El ejecutable recibe como parámetros la cantidad de serpientes por generar y su largo.

snakes_parallel.cpp
```bash
  gcc -o snakes_parallel snakes_parallel.cpp -fopenmp -lSDL2
```
El ejecutable recibe como parámetros la cantidad de serpientes por generar y su largo.

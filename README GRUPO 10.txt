README – GymTracker (uso básico)

1) ARCHIVOS NECESARIOS

En una misma carpeta tienen que estar:

- codigo princial.c
- socio.h
- rutina.h
- socios.dat
- rutinas.dat

Los archivos socios.dat y rutinas.dat ya tienen datos de ejemplo para probar el programa.


2) COMO USAR EL PROGRAMA

Al ejecutar el programa aparece un menu por consola con varias opciones numeradas.

Uso basico de las opciones:

1 - Alta de socio
    Carga un nuevo socio pidiendo nombre, edad, peso, altura y objetivo.
    El programa asigna el ID automaticamente.

2 - Listar socios
    Muestra todos los socios cargados (incluye los leidos desde socios.dat).

3 - Registrar asistencia de socio
    Pide el ID de un socio y suma 1 a su cantidad de asistencias.

4 - Listar rutinas
    Muestra todas las rutinas disponibles (leidas desde rutinas.dat).

5 - Asignar rutina a socio
    Pide ID de socio e ID de rutina y asigna esa rutina al socio.

6 - Ver rutina asignada de un socio
    Pide ID de socio y muestra la informacion de la rutina que tiene asignada.

7 - Ver historial de eventos
    Muestra un listado de las ultimas acciones (alta de socio, asistencias, asignaciones).

0 - Salir
    Guarda los datos actualizados en socios.dat y rutinas.dat
    Libera la memoria y termina el programa.


3) NOTAS IMPORTANTES

- Si los .dat no existen, el programa puede iniciar sin datos y luego crearlos al salir.
- Al salir, los archivos se sobreescribiran con la informacion nueva en caso de ser añadida 

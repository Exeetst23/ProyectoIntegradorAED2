#ifndef SOCIO_H
#define SOCIO_H

typedef struct {
    int idSocio;
    char nombre[40];
    int edad;
    float peso;
    float altura;
    char objetivo[40];
    int idRutina;   /* -1 si no tiene rutina */
    int asistencias;
} tSocio;

#endif


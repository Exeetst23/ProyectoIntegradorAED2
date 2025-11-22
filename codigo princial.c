#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "socio.h"
#include "rutina.h"

/* ---------- Tipos internos para las estructuras dinamicas ---------- */

typedef struct sNodoSocio {
    tSocio dato;
    struct sNodoSocio *sig;
} tNodoSocio;

typedef tNodoSocio* tListaSocios;

typedef struct sNodoRutina {
    tRutina dato;
    struct sNodoRutina *izq;
    struct sNodoRutina *der;
} tNodoRutina;

typedef tNodoRutina* tArbolRutinas;

typedef struct {
    int tipo;       /* 1 = alta socio, 2 = asistencia, 3 = asignar rutina */
    int idSocio;
    int idRutina;
    char texto[80];
} tEvento;

typedef struct sNodoHist {
    tEvento dato;
    struct sNodoHist *sig;
} tNodoHist;

typedef tNodoHist* tPilaHist;

/* ---------- Constantes ---------- */

#define ARCH_SOCIOS  "socios.dat"
#define ARCH_RUTINAS "rutinas.dat"

/* ---------- Prototipos ---------- */

/* lista de socios */
void lista_inicializar(tListaSocios *lista);
int  lista_agregarFin(tListaSocios *lista, tSocio socio);
tSocio* lista_buscarPorId(tListaSocios lista, int id);
void lista_mostrar(tListaSocios lista);
int  lista_proximoId(tListaSocios lista);
void lista_liberar(tListaSocios *lista);

/* arbol de rutinas */
void arbol_inicializar(tArbolRutinas *arbol);
void arbol_insertar(tArbolRutinas *arbol, tRutina r);
tRutina* arbol_buscarPorId(tArbolRutinas arbol, int id);
void arbol_mostrarInorden(tArbolRutinas arbol);
void arbol_liberar(tArbolRutinas *arbol);

/* pila de historial */
void pila_inicializar(tPilaHist *pila);
void pila_apilar(tPilaHist *pila, tEvento e);
void pila_mostrar(tPilaHist pila);
void pila_liberar(tPilaHist *pila);

/* archivos */
void cargarSocios(tListaSocios *lista);
void guardarSocios(tListaSocios lista);
void cargarRutinas(tArbolRutinas *arbol);
void guardarRutinas(tArbolRutinas arbol);

/* interfaz */
void limpiarBuffer(void);
void pausar(void);
void mostrarMenu(void);
void altaSocio(tListaSocios *lista, tPilaHist *hist);
void registrarAsistencia(tListaSocios lista, tPilaHist *hist);
void asignarRutinaASocio(tListaSocios lista, tArbolRutinas arbol, tPilaHist *hist);
void verRutinaDeSocio(tListaSocios lista, tArbolRutinas arbol);
void verHistorial(tPilaHist hist);

/* ---------- main ---------- */

int main(void) {
    tListaSocios listaSocios;
    tArbolRutinas arbolRutinas;
    tPilaHist historial;
    int opcion;

    lista_inicializar(&listaSocios);
    arbol_inicializar(&arbolRutinas);
    pila_inicializar(&historial);

    cargarSocios(&listaSocios);
    cargarRutinas(&arbolRutinas);

    do {
        mostrarMenu();
        printf("\nOpcion: ");
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no valida.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        switch (opcion) {
        case 1:
            altaSocio(&listaSocios, &historial);
            break;
        case 2:
            lista_mostrar(listaSocios);
            pausar();
            break;
        case 3:
            registrarAsistencia(listaSocios, &historial);
            pausar();
            break;
        case 4:
            if (arbolRutinas == NULL) {
                printf("\nNo hay rutinas cargadas.\n");
            } else {
                printf("\nListado de rutinas:\n\n");
                arbol_mostrarInorden(arbolRutinas);
            }
            pausar();
            break;
        case 5:
            asignarRutinaASocio(listaSocios, arbolRutinas, &historial);
            pausar();
            break;
        case 6:
            verRutinaDeSocio(listaSocios, arbolRutinas);
            pausar();
            break;
        case 7:
            verHistorial(historial);
            pausar();
            break;
        case 0:
            printf("\nGuardando datos y saliendo...\n");
            break;
        default:
            printf("\nOpcion incorrecta.\n");
            pausar();
        }

    } while (opcion != 0);

    guardarSocios(listaSocios);
    guardarRutinas(arbolRutinas);

    lista_liberar(&listaSocios);
    arbol_liberar(&arbolRutinas);
    pila_liberar(&historial);

    return 0;
}

/* ---------- lista de socios ---------- */

void lista_inicializar(tListaSocios *lista) {
    *lista = NULL;
}

int lista_agregarFin(tListaSocios *lista, tSocio socio) {
    tNodoSocio *nuevo = (tNodoSocio*) malloc(sizeof(tNodoSocio));
    tNodoSocio *aux;
    if (nuevo == NULL) {
        return 0;
    }
    nuevo->dato = socio;
    nuevo->sig = NULL;

    if (*lista == NULL) {
        *lista = nuevo;
    } else {
        aux = *lista;
        while (aux->sig != NULL) {
            aux = aux->sig;
        }
        aux->sig = nuevo;
    }
    return 1;
}

tSocio* lista_buscarPorId(tListaSocios lista, int id) {
    tNodoSocio *aux = lista;
    while (aux != NULL) {
        if (aux->dato.idSocio == id) {
            return &(aux->dato);
        }
        aux = aux->sig;
    }
    return NULL;
}

int lista_proximoId(tListaSocios lista) {
    int maxId = 0;
    tNodoSocio *aux = lista;
    while (aux != NULL) {
        if (aux->dato.idSocio > maxId) {
            maxId = aux->dato.idSocio;
        }
        aux = aux->sig;
    }
    return maxId + 1;
}

void lista_mostrar(tListaSocios lista) {
    tNodoSocio *aux = lista;

    if (aux == NULL) {
        printf("\nNo hay socios cargados.\n");
        return;
    }

    printf("\nListado de socios:\n");
    printf("---------------------------------------------\n");
    while (aux != NULL) {
        tSocio s = aux->dato;
        printf("ID: %d | Nombre: %s | Edad: %d | Peso: %.1f | Altura: %.2f | Obj: %s | Asistencias: %d | Rutina: %d\n",
               s.idSocio, s.nombre, s.edad, s.peso, s.altura, s.objetivo, s.asistencias, s.idRutina);
        aux = aux->sig;
    }
    printf("---------------------------------------------\n");
}

void lista_liberar(tListaSocios *lista) {
    tNodoSocio *aux = *lista;
    tNodoSocio *sig;
    while (aux != NULL) {
        sig = aux->sig;
        free(aux);
        aux = sig;
    }
    *lista = NULL;
}

/* ---------- arbol de rutinas ---------- */
/* Estas funciones de arbol (insertar y buscar) las armamos con ayuda de
   una herramienta de IA (ChatGPT) y luego las simplificamos. */

void arbol_inicializar(tArbolRutinas *arbol) {
    *arbol = NULL;
}

void arbol_insertar(tArbolRutinas *arbol, tRutina r) {
    if (*arbol == NULL) {
        tNodoRutina *nuevo = (tNodoRutina*) malloc(sizeof(tNodoRutina));
        if (nuevo == NULL) {
            return;
        }
        nuevo->dato = r;
        nuevo->izq = NULL;
        nuevo->der = NULL;
        *arbol = nuevo;
    } else {
        if (r.idRutina < (*arbol)->dato.idRutina) {
            arbol_insertar(&((*arbol)->izq), r);
        } else if (r.idRutina > (*arbol)->dato.idRutina) {
            arbol_insertar(&((*arbol)->der), r);
        } else {
            /* id repetido, no hacemos nada */
        }
    }
}

tRutina* arbol_buscarPorId(tArbolRutinas arbol, int id) {
    if (arbol == NULL) {
        return NULL;
    }
    if (id == arbol->dato.idRutina) {
        return &(arbol->dato);
    }
    if (id < arbol->dato.idRutina) {
        return arbol_buscarPorId(arbol->izq, id);
    } else {
        return arbol_buscarPorId(arbol->der, id);
    }
}

void arbol_mostrarInorden(tArbolRutinas arbol) {
    if (arbol == NULL) {
        return;
    }
    arbol_mostrarInorden(arbol->izq);
    printf("ID: %d | Nombre: %s | Grupo: %s | Nivel: %s | Duracion: %d min\n",
           arbol->dato.idRutina,
           arbol->dato.nombre,
           arbol->dato.grupo,
           arbol->dato.nivel,
           arbol->dato.duracionMin);
    arbol_mostrarInorden(arbol->der);
}

void arbol_liberar(tArbolRutinas *arbol) {
    if (*arbol == NULL) {
        return;
    }
    arbol_liberar(&((*arbol)->izq));
    arbol_liberar(&((*arbol)->der));
    free(*arbol);
    *arbol = NULL;
}

/* ---------- pila historial ---------- */

void pila_inicializar(tPilaHist *pila) {
    *pila = NULL;
}

void pila_apilar(tPilaHist *pila, tEvento e) {
    tNodoHist *nuevo = (tNodoHist*) malloc(sizeof(tNodoHist));
    if (nuevo == NULL) {
        return;
    }
    nuevo->dato = e;
    nuevo->sig = *pila;
    *pila = nuevo;
}

void pila_mostrar(tPilaHist pila) {
    tNodoHist *aux = pila;
    if (aux == NULL) {
        printf("\nNo hay eventos en el historial.\n");
        return;
    }
    printf("\nHistorial (ultimo primero):\n");
    printf("---------------------------------------------\n");
    while (aux != NULL) {
        printf("Tipo: %d | Socio: %d | Rutina: %d | Detalle: %s\n",
               aux->dato.tipo,
               aux->dato.idSocio,
               aux->dato.idRutina,
               aux->dato.texto);
        aux = aux->sig;
    }
    printf("---------------------------------------------\n");
}

void pila_liberar(tPilaHist *pila) {
    tNodoHist *aux = *pila;
    tNodoHist *sig;
    while (aux != NULL) {
        sig = aux->sig;
        free(aux);
        aux = sig;
    }
    *pila = NULL;
}

/* ---------- archivos ---------- */
/* El manejo de archivos (fgets + sscanf) tambien lo armamos con ayuda de IA
   y despues lo adaptamos al formato de nuestros .dat. */

void cargarSocios(tListaSocios *lista) {
    FILE *f = fopen(ARCH_SOCIOS, "r");
    char linea[256];
    tSocio s;

    if (f == NULL) {
        printf("\nNo se pudo abrir %s. Si es la primera vez, no pasa nada.\n", ARCH_SOCIOS);
        return;
    }

    while (fgets(linea, sizeof(linea), f) != NULL) {
        if (sscanf(linea, "%d;%39[^;];%d;%f;%f;%39[^;];%d;%d",
                   &s.idSocio,
                   s.nombre,
                   &s.edad,
                   &s.peso,
                   &s.altura,
                   s.objetivo,
                   &s.idRutina,
                   &s.asistencias) == 8) {
            lista_agregarFin(lista, s);
        }
    }

    fclose(f);
}

void guardarSocios(tListaSocios lista) {
    FILE *f = fopen(ARCH_SOCIOS, "w");
    tNodoSocio *aux = lista;

    if (f == NULL) {
        printf("\nError al abrir %s para guardar.\n", ARCH_SOCIOS);
        return;
    }

    while (aux != NULL) {
        tSocio s = aux->dato;
        fprintf(f, "%d;%s;%d;%.2f;%.2f;%s;%d;%d\n",
                s.idSocio,
                s.nombre,
                s.edad,
                s.peso,
                s.altura,
                s.objetivo,
                s.idRutina,
                s.asistencias);
        aux = aux->sig;
    }

    fclose(f);
}

static void guardarRutinasRec(FILE *f, tArbolRutinas arbol) {
    if (arbol == NULL) {
        return;
    }
    guardarRutinasRec(f, arbol->izq);
    fprintf(f, "%d;%s;%s;%s;%d\n",
            arbol->dato.idRutina,
            arbol->dato.nombre,
            arbol->dato.grupo,
            arbol->dato.nivel,
            arbol->dato.duracionMin);
    guardarRutinasRec(f, arbol->der);
}

void cargarRutinas(tArbolRutinas *arbol) {
    FILE *f = fopen(ARCH_RUTINAS, "r");
    char linea[256];
    tRutina r;

    if (f == NULL) {
        printf("\nNo se pudo abrir %s. Si es la primera vez, no pasa nada.\n", ARCH_RUTINAS);
        return;
    }

    while (fgets(linea, sizeof(linea), f) != NULL) {
        if (sscanf(linea, "%d;%39[^;];%19[^;];%19[^;];%d",
                   &r.idRutina,
                   r.nombre,
                   r.grupo,
                   r.nivel,
                   &r.duracionMin) == 5) {
            arbol_insertar(arbol, r);
        }
    }

    fclose(f);
}

void guardarRutinas(tArbolRutinas arbol) {
    FILE *f = fopen(ARCH_RUTINAS, "w");

    if (f == NULL) {
        printf("\nError al abrir %s para guardar.\n", ARCH_RUTINAS);
        return;
    }

    guardarRutinasRec(f, arbol);

    fclose(f);
}

/* ---------- interfaz / operaciones ---------- */

void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descartar */
    }
}

void pausar(void) {
    printf("\nPresione ENTER para continuar...");
    fflush(stdout);
    getchar();
}

void mostrarMenu(void) {
    printf("\n===== Menu GymTracker (version simple) =====\n");
    printf("1) Alta de socio\n");
    printf("2) Listar socios\n");
    printf("3) Registrar asistencia de socio\n");
    printf("4) Listar rutinas\n");
    printf("5) Asignar rutina a socio\n");
    printf("6) Ver rutina asignada de un socio\n");
    printf("7) Ver historial de eventos\n");
    printf("0) Salir\n");
}

void altaSocio(tListaSocios *lista, tPilaHist *hist) {
    tSocio s;
    tEvento e;
    size_t len;

    s.idSocio = lista_proximoId(*lista);
    printf("\nAlta de socio (id %d)\n", s.idSocio);

    printf("Nombre: ");
    fgets(s.nombre, sizeof(s.nombre), stdin);
    len = strlen(s.nombre);
    if (len > 0 && s.nombre[len - 1] == '\n') {
        s.nombre[len - 1] = '\0';
    }

    printf("Edad: ");
    scanf("%d", &s.edad);
    limpiarBuffer();

    printf("Peso (kg): ");
    scanf("%f", &s.peso);
    limpiarBuffer();

    printf("Altura (m): ");
    scanf("%f", &s.altura);
    limpiarBuffer();

    printf("Objetivo: ");
    fgets(s.objetivo, sizeof(s.objetivo), stdin);
    len = strlen(s.objetivo);
    if (len > 0 && s.objetivo[len - 1] == '\n') {
        s.objetivo[len - 1] = '\0';
    }

    s.idRutina = -1;
    s.asistencias = 0;

    if (lista_agregarFin(lista, s)) {
        printf("Socio cargado con exito.\n");
        e.tipo = 1;
        e.idSocio = s.idSocio;
        e.idRutina = s.idRutina;
        strcpy(e.texto, "Alta de socio");
        pila_apilar(hist, e);
    } else {
        printf("No se pudo cargar el socio (sin memoria).\n");
    }
}

void registrarAsistencia(tListaSocios lista, tPilaHist *hist) {
    int id;
    tSocio *s;
    tEvento e;

    printf("\nRegistrar asistencia\n");
    printf("ID de socio: ");
    if (scanf("%d", &id) != 1) {
        printf("Entrada no valida.\n");
        limpiarBuffer();
        return;
    }
    limpiarBuffer();

    s = lista_buscarPorId(lista, id);
    if (s == NULL) {
        printf("No se encontro socio con ese ID.\n");
        return;
    }

    s->asistencias++;
    printf("Asistencia registrada. Total ahora: %d\n", s->asistencias);

    e.tipo = 2;
    e.idSocio = s->idSocio;
    e.idRutina = s->idRutina;
    strcpy(e.texto, "Registro de asistencia");
    pila_apilar(hist, e);
}

void asignarRutinaASocio(tListaSocios lista, tArbolRutinas arbol, tPilaHist *hist) {
    int idSocio, idRutina;
    tSocio *s;
    tRutina *r;
    tEvento e;

    printf("\nAsignar rutina a socio\n");
    printf("ID de socio: ");
    if (scanf("%d", &idSocio) != 1) {
        printf("Entrada no valida.\n");
        limpiarBuffer();
        return;
    }
    limpiarBuffer();

    s = lista_buscarPorId(lista, idSocio);
    if (s == NULL) {
        printf("No se encontro socio con ese ID.\n");
        return;
    }

    printf("ID de rutina: ");
    if (scanf("%d", &idRutina) != 1) {
        printf("Entrada no valida.\n");
        limpiarBuffer();
        return;
    }
    limpiarBuffer();

    r = arbol_buscarPorId(arbol, idRutina);
    if (r == NULL) {
        printf("No se encontro rutina con ese ID.\n");
        return;
    }

    s->idRutina = idRutina;
    printf("Rutina %d asignada al socio %s.\n", idRutina, s->nombre);

    e.tipo = 3;
    e.idSocio = s->idSocio;
    e.idRutina = idRutina;
    strcpy(e.texto, "Asignacion de rutina");
    pila_apilar(hist, e);
}

void verRutinaDeSocio(tListaSocios lista, tArbolRutinas arbol) {
    int idSocio;
    tSocio *s;
    tRutina *r;

    printf("\nVer rutina de un socio\n");
    printf("ID de socio: ");
    if (scanf("%d", &idSocio) != 1) {
        printf("Entrada no valida.\n");
        limpiarBuffer();
        return;
    }
    limpiarBuffer();

    s = lista_buscarPorId(lista, idSocio);
    if (s == NULL) {
        printf("No se encontro socio con ese ID.\n");
        return;
    }

    if (s->idRutina < 0) {
        printf("El socio no tiene rutina asignada.\n");
        return;
    }

    r = arbol_buscarPorId(arbol, s->idRutina);
    if (r == NULL) {
        printf("La rutina asignada no se encontro.\n");
        return;
    }

    printf("\nRutina del socio %s (id %d):\n", s->nombre, s->idSocio);
    printf("ID rutina: %d\n", r->idRutina);
    printf("Nombre: %s\n", r->nombre);
    printf("Grupo: %s\n", r->grupo);
    printf("Nivel: %s\n", r->nivel);
    printf("Duracion: %d min\n", r->duracionMin);
}

void verHistorial(tPilaHist hist) {
    pila_mostrar(hist);
}


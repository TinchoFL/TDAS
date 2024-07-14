#ifndef __MENU_H__
#define __MENU_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct menu menu_t;

typedef struct comando comando_t;

/*
 * Crea el menu reservando la memoria necesaria.
 * Devuelve un puntero al menu creado o NULL en caso de no poder crearlo.
 */
menu_t *menu_crear();

/*
 * Ejecuta la funcion del comando recibido o devuelve 0 en caso de error.
 */
void *menu_ejecutar_comando(menu_t *menu, char *texto);
/*
 * Agrega un comando al menu, devuelve el menu si se pudo agregar el nuevo comando o NULL en caso de error.
 */
void *menu_agregar_comando(menu_t *menu, const char *nombre, const char *letra,
			   const char *descripcion,
			   void (*ejecutar)(void *, void *), void *contexto);

/*
 * Elimina un comando dentro del Menu, devuelve el comando si se pudo eliminar o NULL en caso de error.
 */
void *menu_eliminar_comando(menu_t *menu, const char *nombre);
/*
 * Devuelve true si el menu contiene un comando o false en caso de error.
 */
bool menu_contiene(menu_t *menu, const char *nombre);

/*
 * Devuelve true si el menu esta vacio o false en caso de error.
 */
bool menu_vacio(menu_t *menu);
/*
 * Destruye el menu liberando la memoria reservada.
 */
void menu_destruir(menu_t *menu);

#endif // MENU_H_
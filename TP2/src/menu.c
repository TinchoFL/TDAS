#include "menu.h"
#include "hash.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct comando {
	const char *nombre;
	const char *letra;
	const char *descripcion;
	void (*ejecutar)(void *, void *);
	void *contexto;
};

struct menu {
	char cantidad;
	hash_t *comandos;
};

comando_t *comando_crear(const char *nombre, const char *letra,
			 const char *descripcion,
			 void (*ejecutar)(void *, void *), void *contexto)
{
	if (!nombre || !letra || !descripcion || !ejecutar) {
		return NULL;
	}

	comando_t *comando = calloc(1, sizeof(comando_t));
	if (!comando) {
		return NULL;
	}

	comando->nombre = nombre;
	comando->letra = letra;
	comando->descripcion = descripcion;
	comando->ejecutar = ejecutar;
	comando->contexto = contexto;

	return comando;
}

menu_t *menu_crear()
{
	menu_t *menu = calloc(1, sizeof(menu_t));
	if (!menu) {
		return NULL;
	}

	menu->comandos = hash_crear(5);
	if (!menu->comandos)
		return NULL;

	return menu;
}

void *menu_ejecutar_comando(menu_t *menu, char *linea)
{
	if (!menu || !linea) {
		return NULL;
	}

	comando_t *comando = hash_obtener(menu->comandos, linea);
	if (comando) {
		comando->ejecutar(menu, comando->contexto);
	}

	return comando;
}

void *menu_agregar_comando(menu_t *menu, const char *nombre, const char *letra,
			   const char *descripcion,
			   void (*ejecutar)(void *, void *), void *contexto)
{
	if (!menu || !nombre || !letra || !descripcion || !ejecutar) {
		return NULL;
	}

	comando_t *comando =
		comando_crear(nombre, letra, descripcion, ejecutar, contexto);
	if (!comando) {
		return NULL;
	}

	hash_t *aux = hash_insertar(menu->comandos, nombre, comando, NULL);

	if (!aux)
		return NULL;

	return menu;
}

void *menu_eliminar_comando(menu_t *menu, const char *nombre)
{
	if (!menu || !nombre) {
		return NULL;
	}

	return hash_quitar(menu->comandos, nombre);
}

bool menu_contiene(menu_t *menu, const char *nombre)
{
	if (!menu || !nombre) {
		return false;
	}

	return hash_contiene(menu->comandos, nombre);
}

bool menu_vacio(menu_t *menu)
{
	if (!menu) {
		return false;
	}

	if (hash_cantidad(menu->comandos) == 0)
		return true;
	return false;
}

void destruye_comandos2(void *_comando)
{
	if (!_comando) {
		return;
	}
	comando_t *comando = (comando_t *)_comando;
	free(comando);
}

void menu_destruir(menu_t *menu)
{
	if (!menu) {
		return;
	}
	hash_destruir_todo(menu->comandos, destruye_comandos2);
	free(menu);
}

#include <string.h>
#include <stdlib.h>

#include "hash.h"
#include "estructura_hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define CAPACIDAD_MINIMA 3

#define ERROR 0

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));

	if (!hash) {
		return NULL;
	}

	if (capacidad < CAPACIDAD_MINIMA) {
		capacidad = CAPACIDAD_MINIMA;
	}

	hash->capacidad = capacidad;

	hash->vector = calloc(1, sizeof(struct nodo *) * capacidad);

	if (!hash->vector) {
		free(hash);
		return NULL;
	}

	return hash;
}

unsigned long funcion_hash(const char *str)
{
	unsigned long hash = 5381;
	unsigned long c;

	while ((c = (unsigned long)*str++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

unsigned long obtener_posicion(size_t capacidad, const char *clave)
{
	return funcion_hash(clave) % capacidad;
}

char *copiar_clave(const char *str)
{
	if (!str) {
		return NULL;
	}
	char *copia = malloc((1 + strlen(str)) * sizeof(char));
	if (!copia) {
		return NULL;
	}
	strcpy(copia, str);

	return copia;
}

nodo_t *nodo_crear(const char *clave, void *valor)
{
	if (!clave) {
		return NULL;
	}

	nodo_t *nodo = malloc(sizeof(nodo_t));

	if (!nodo) {
		return NULL;
	}

	nodo->clave = copiar_clave(clave);
	if (!nodo->clave) {
		free(nodo->clave);
		return NULL;
	}
	nodo->valor = valor;
	nodo->siguiente = NULL;

	return nodo;
}

void transpasar_nodos(hash_t *hash_antiguo, hash_t *hash_ampliado,
		      size_t posicion)
{
	nodo_t *nodo_actual = hash_antiguo->vector[posicion];

	while (nodo_actual) {
		size_t nueva_posicion = obtener_posicion(
			hash_ampliado->capacidad, nodo_actual->clave);
		nodo_t *nodo_siguiente = nodo_actual->siguiente;

		nodo_t *nodo_aux = hash_ampliado->vector[nueva_posicion];
		hash_ampliado->vector[nueva_posicion] = nodo_actual;
		nodo_actual->siguiente = nodo_aux;

		hash_ampliado->cantidad++;
		nodo_actual = nodo_siguiente;
	}
}

hash_t *rehash(hash_t *hash)
{
	if (!hash) {
		return NULL;
	}

	hash_t aux = *hash;

	hash->vector = calloc(hash->capacidad * 2, sizeof(nodo_t *));
	if (!hash->vector) {
		return NULL;
	}

	hash->capacidad *= 2;
	hash->cantidad = 0;

	for (size_t posicion = 0; posicion < aux.capacidad; posicion++) {
		transpasar_nodos(&aux, hash, posicion);
	}

	free(aux.vector);

	return hash;
}

nodo_t *nodo_insertar(hash_t *hash, const char *clave, void *elemento,
		      nodo_t *nodo_actual, void **anterior)
{
	if (!hash || !clave) {
		return NULL;
	}

	if (!nodo_actual) {
		nodo_t *nodo = nodo_crear(clave, elemento);

		if (!nodo) {
			return NULL;
		}

		if (anterior != NULL) {
			*anterior = NULL;
		}

		hash->cantidad++;

		return nodo;
	}

	else if (strcmp(nodo_actual->clave, clave) == 0) {
		if (anterior != NULL) {
			*anterior = nodo_actual->valor;
		}

		nodo_actual->valor = elemento;
	} else {
		nodo_actual->siguiente = nodo_insertar(hash, clave, elemento,
						       nodo_actual->siguiente,
						       anterior);
	}

	return nodo_actual;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave) {
		return NULL;
	}

	float factor_carga = (float)(hash->cantidad) / (float)(hash->capacidad);

	if (factor_carga > FACTOR_CARGA_MAXIMO) {
		hash_t *nuevo_hash = rehash(hash);

		if (!nuevo_hash) {
			return NULL;
		}
	}

	size_t posicion = obtener_posicion(hash->capacidad, clave);

	hash->vector[posicion] = nodo_insertar(
		hash, clave, elemento, hash->vector[posicion], anterior);

	if (!hash->vector[posicion]) {
		free(hash->vector[posicion]);
		return NULL;
	}

	return hash;
}

nodo_t *nodo_quitar(hash_t *hash, const char *clave, void **elemento,
		    nodo_t *nodo_actual)
{
	if (!hash || !clave) {
		return NULL;
	}

	nodo_t *aux = NULL;

	if (strcmp(nodo_actual->clave, clave) == 0) {
		*elemento = nodo_actual->valor;

		if (nodo_actual->siguiente != NULL) {
			aux = nodo_actual->siguiente;
		}
		free(nodo_actual->valor);
		free(nodo_actual->clave);
		free(nodo_actual);
		hash->cantidad--;
		return aux;
	} else
		nodo_actual->siguiente = nodo_quitar(hash, clave, elemento,
						     nodo_actual->siguiente);

	return nodo_actual;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave) {
		return NULL;
	}

	void *elemento = NULL;
	size_t posicion = obtener_posicion(hash->capacidad, clave);

	if (hash->vector[posicion] != NULL) {
		hash->vector[posicion] = nodo_quitar(hash, clave, &elemento,
						     hash->vector[posicion]);
	}
	return elemento;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave) {
		return NULL;
	}

	size_t posicion = obtener_posicion(hash->capacidad, clave);

	if (hash->vector[posicion] == NULL) {
		return NULL;
	}

	nodo_t *nodo_actual = hash->vector[posicion];

	while (nodo_actual != NULL) {
		if (strcmp(nodo_actual->clave, clave) == 0) {
			return nodo_actual->valor;
		}
		nodo_actual = nodo_actual->siguiente;
	}

	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave) {
		return NULL;
	}

	size_t posicion = obtener_posicion(hash->capacidad, clave);

	if (hash->vector[posicion] == NULL) {
		return NULL;
	}

	nodo_t *nodo_actual = hash->vector[posicion];

	while (nodo_actual != NULL) {
		if (strcmp(nodo_actual->clave, clave) == 0) {
			return true;
		}
		nodo_actual = nodo_actual->siguiente;
	}

	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash) {
		return ERROR;
	}

	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void destruir_nodos(nodo_t *nodo, void (*destructor)(void *))
{
	if (nodo->siguiente != NULL) {
		destruir_nodos(nodo->siguiente, destructor);
	}

	if (destructor != NULL) {
		destructor(nodo->valor);
	}

	free(nodo->clave);
	free(nodo);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash) {
		return;
	}

	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->vector[i] != NULL) {
			destruir_nodos(hash->vector[i], destructor);
		}
	}

	free(hash->vector);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t contador = 0;

	if (!hash || !f) {
		return ERROR;
	}

	bool seguir = true;

	for (int posicion = 0; posicion < hash->capacidad; posicion++) {
		nodo_t *nodo_actual = hash->vector[posicion];

		while (nodo_actual != NULL && seguir == true) {
			seguir = f(nodo_actual->clave, nodo_actual->valor, aux);
			contador++;
			nodo_actual = nodo_actual->siguiente;
		}
	}

	return contador;
}

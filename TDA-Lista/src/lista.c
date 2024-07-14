#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *nodo_fin;
	size_t cantidad;
};

struct lista_iterador {
	lista_t *lista;
	nodo_t *nodo_actual;
};

lista_t *lista_crear()
{
	lista_t *lista = calloc(1, sizeof(lista_t));

	if (!lista) {
		return NULL;
	}

	return lista;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista) {
		return NULL;
	}

	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	if (!nuevo_nodo) {
		return NULL;
	}

	if (lista_vacia(lista)) {
		lista->nodo_inicio = nuevo_nodo;
	} else {
		lista->nodo_fin->siguiente = nuevo_nodo;
	}

	lista->nodo_fin = nuevo_nodo;
	nuevo_nodo->elemento = elemento;
	lista->cantidad++;

	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista) {
		return NULL;
	}

	if (posicion >= lista_tamanio(lista) || lista_vacia(lista)) {
		return lista_insertar(lista, elemento);
	}

	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	if (!nuevo_nodo) {
		return NULL;
	}

	nuevo_nodo->elemento = elemento;

	nodo_t *nodo_actual = lista->nodo_inicio;

	if (posicion == 0) {
		nuevo_nodo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nuevo_nodo;
	} else {
		for (int i = 1; i < posicion; i++) {
			nodo_actual = nodo_actual->siguiente;
		}
		nuevo_nodo->siguiente = nodo_actual->siguiente;
		nodo_actual->siguiente = nuevo_nodo;
	}

	lista->cantidad++;

	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	void *elemento_esperado = lista->nodo_fin->elemento;

	if (lista_tamanio(lista) == 1) {
		elemento_esperado = lista->nodo_inicio->elemento;
		free(lista->nodo_inicio);
		lista->nodo_fin = NULL;
		lista->nodo_inicio = NULL;

	} else {
		nodo_t *aux = lista->nodo_inicio;
		while (aux->siguiente != lista->nodo_fin) {
			aux = aux->siguiente;
		}

		elemento_esperado = lista->nodo_fin->elemento;
		free(lista->nodo_fin);
		lista->nodo_fin = aux;
		aux->siguiente = NULL;
	}

	lista->cantidad--;

	return elemento_esperado;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	if (lista_tamanio(lista) == 1 || posicion >= lista_tamanio(lista) - 1) {
		return lista_quitar(lista);
	}

	nodo_t *nodo_buscado;
	void *elemento;

	if (posicion == 0) {
		nodo_buscado = lista->nodo_inicio;
		lista->nodo_inicio = lista->nodo_inicio->siguiente;
		elemento = nodo_buscado->elemento;
	} else {
		nodo_t *nodo_anterior = lista->nodo_inicio;
		nodo_buscado = lista->nodo_inicio->siguiente;

		for (size_t i = 1; i < posicion; i++) {
			nodo_anterior = nodo_anterior->siguiente;
			nodo_buscado = nodo_buscado->siguiente;
		}

		nodo_anterior->siguiente = nodo_buscado->siguiente;
		elemento = nodo_buscado->elemento;
	}

	free(nodo_buscado);
	lista->cantidad--;

	return elemento;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista) || posicion >= lista_tamanio(lista)) {
		return NULL;
	}

	if (posicion == 0) {
		return lista_primero(lista);
	}
	if (posicion == lista_tamanio(lista) - 1) {
		return lista_ultimo(lista);
	}

	nodo_t *nodo_actual = lista->nodo_inicio;

	for (size_t i = 0; i < posicion; i++) {
		nodo_actual = nodo_actual->siguiente;
	}

	return nodo_actual->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || lista_vacia(lista) || !comparador) {
		return NULL;
	}

	for (size_t i = 0; i < lista_tamanio(lista); i++) {
		if (comparador(lista_elemento_en_posicion(lista, i),
			       contexto) == 0) {
			return lista_elemento_en_posicion(lista, i);
		}
	}

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista || lista->cantidad == 0) {
		return true;
	}
	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista) {
		return 0;
	}

	return lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	if (!lista) {
		return;
	}

	lista_destruir_todo(lista, NULL);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista) {
		return;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	nodo_t *aux;

	while (nodo_actual != NULL) {
		aux = nodo_actual;
		nodo_actual = nodo_actual->siguiente;

		if (funcion) {
			funcion(aux->elemento);
		}

		free(aux);
	}
	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}

	lista_iterador_t *iterador = calloc(1, sizeof(lista_iterador_t));

	if (!iterador) {
		return NULL;
	}

	iterador->lista = lista;
	iterador->nodo_actual = lista->nodo_inicio;

	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->nodo_actual) {
		return false;
	}

	return true;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->nodo_actual) {
		return false;
	}

	iterador->nodo_actual = iterador->nodo_actual->siguiente;

	if (!iterador->nodo_actual) {
		return false;
	}

	return true;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->nodo_actual) {
		return NULL;
	}

	return iterador->nodo_actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion) {
		return 0;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	bool se_utilizo_funcion = true;
	size_t contador = 0;

	while ((nodo_actual) && (se_utilizo_funcion != false)) {
		se_utilizo_funcion = funcion(nodo_actual->elemento, contexto);
		contador++;
		nodo_actual = nodo_actual->siguiente;
	}

	return contador;
}

#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

#define ERROR 0

typedef struct array_limitado {
	void **array;
	size_t cantidad;
	size_t tope;
} array_limitado_t;

abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador) {
		return NULL;
	}

	abb_t *arbol = calloc(1, sizeof(abb_t));

	if (!arbol) {
		return NULL;
	}

	arbol->comparador = comparador;

	return arbol;
}

nodo_abb_t *nodo_crear(void *elemento)
{
	nodo_abb_t *nodo = calloc(1, sizeof(nodo_abb_t));

	if (!nodo) {
		return NULL;
	}

	nodo->elemento = elemento;
	return nodo;
}

nodo_abb_t *insertar_elemento(nodo_abb_t *raiz, nodo_abb_t *nuevo_nodo,
			      abb_comparador comparador)
{
	if (!comparador) {
		return NULL;
	}

	if (!raiz) {
		return nuevo_nodo;
	}

	int comparacion = comparador(nuevo_nodo->elemento, raiz->elemento);

	if (comparacion <= 0) {
		raiz->izquierda = insertar_elemento(raiz->izquierda, nuevo_nodo,
						    comparador);
	} else {
		raiz->derecha = insertar_elemento(raiz->derecha, nuevo_nodo,
						  comparador);
	}

	return raiz;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
		return NULL;
	}

	nodo_abb_t *nuevo_nodo = nodo_crear(elemento);

	if (!nuevo_nodo) {
		return NULL;
	}

	nuevo_nodo->elemento = elemento;

	arbol->nodo_raiz = insertar_elemento(arbol->nodo_raiz, nuevo_nodo,
					     arbol->comparador);

	arbol->tamanio++;

	return arbol;
}

nodo_abb_t *quitar_nodo_mas_derecho(nodo_abb_t *nodo_actual,
				    nodo_abb_t **nodo_mas_derecho)
{
	if (!nodo_actual) {
		return NULL;
	}

	if (nodo_actual->derecha == NULL) {
		*nodo_mas_derecho = nodo_actual;
		return nodo_actual->izquierda;
	}

	nodo_actual->derecha =
		quitar_nodo_mas_derecho(nodo_actual->derecha, nodo_mas_derecho);

	return nodo_actual;
}

nodo_abb_t *quitar_nodo_con_dos_hijos(nodo_abb_t *nodo_actual)
{
	nodo_abb_t *nodo_derecho = NULL;

	nodo_actual->izquierda =
		quitar_nodo_mas_derecho(nodo_actual->izquierda, &nodo_derecho);

	nodo_actual->elemento = nodo_derecho->elemento;

	free(nodo_derecho);

	return nodo_actual;
}

nodo_abb_t *quitar_nodo_con_cero_o_un_hijo(nodo_abb_t *nodo_actual)
{
	nodo_abb_t *aux = NULL;

	if (nodo_actual->izquierda == NULL) {
		aux = nodo_actual->derecha;
		free(nodo_actual);
		return aux;
	}

	aux = nodo_actual->izquierda;

	free(nodo_actual);

	return aux;
}

nodo_abb_t *quitar_elemento(nodo_abb_t *nodo_actual, void *elemento,
			    abb_comparador comparador, void **elemento_quitado,
			    size_t *tamanio)
{
	if (!nodo_actual) {
		return NULL;
	}

	int comparacion = comparador(elemento, nodo_actual->elemento);

	if (comparacion == 0) {
		*elemento_quitado = nodo_actual->elemento;
		(*tamanio)--;

		if (nodo_actual->izquierda != NULL &&
		    nodo_actual->derecha != NULL) {
			return quitar_nodo_con_dos_hijos(nodo_actual);

		} else {
			return quitar_nodo_con_cero_o_un_hijo(nodo_actual);
		}

	} else if (comparacion < 0) {
		nodo_actual->izquierda =
			quitar_elemento(nodo_actual->izquierda, elemento,
					comparador, elemento_quitado, tamanio);
	} else {
		nodo_actual->derecha =
			quitar_elemento(nodo_actual->derecha, elemento,
					comparador, elemento_quitado, tamanio);
	}

	return nodo_actual;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
		return NULL;
	}

	void *elemento_quitado = NULL;

	arbol->nodo_raiz = quitar_elemento(arbol->nodo_raiz, elemento,
					   arbol->comparador, &elemento_quitado,
					   &arbol->tamanio);

	return elemento_quitado;
}

void *buscar_elemento(nodo_abb_t *nodo_actual, void *elemento,
		      abb_comparador comparador)
{
	if (!nodo_actual) {
		return NULL;
	}

	int comparacion = comparador(elemento, nodo_actual->elemento);

	if (comparacion < 0) {
		return buscar_elemento(nodo_actual->izquierda, elemento,
				       comparador);
	} else if (comparacion > 0) {
		return buscar_elemento(nodo_actual->derecha, elemento,
				       comparador);
	} else {
		return nodo_actual->elemento;
	}

	return NULL;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
		return NULL;
	}

	return buscar_elemento(arbol->nodo_raiz, elemento, arbol->comparador);
}

bool abb_vacio(abb_t *arbol)
{
	if (!arbol || arbol->tamanio == 0) {
		return true;
	}

	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol) {
		return ERROR;
	}

	return arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
	if (!arbol) {
		return;
	}

	abb_destruir_todo(arbol, NULL);
}

void destruir_todo_el_nodo(nodo_abb_t *nodo_actual, void (*destructor)(void *))
{
	if (!nodo_actual) {
		return;
	}

	destruir_todo_el_nodo(nodo_actual->izquierda, destructor);

	destruir_todo_el_nodo(nodo_actual->derecha, destructor);

	if (destructor != NULL) {
		destructor(nodo_actual->elemento);
	}

	free(nodo_actual);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol) {
		return;
	}

	destruir_todo_el_nodo(arbol->nodo_raiz, destructor);
	free(arbol);
}

bool abb_con_cada_elemento_inorden(nodo_abb_t *raiz,
				   bool (*funcion)(void *, void *), void *aux,
				   size_t *contador)
{
	if (!raiz) {
		return true;
	}

	if (!abb_con_cada_elemento_inorden(raiz->izquierda, funcion, aux,
					   contador)) {
		return false;
	}

	(*contador)++;

	if (!funcion(raiz->elemento, aux)) {
		return false;
	}

	if (!abb_con_cada_elemento_inorden(raiz->derecha, funcion, aux,
					   contador)) {
		return false;
	}

	return true;
}

bool abb_con_cada_elemento_preorden(nodo_abb_t *raiz,
				    bool (*funcion)(void *, void *), void *aux,
				    size_t *contador)
{
	if (!raiz) {
		return true;
	}

	(*contador)++;

	if (!funcion(raiz->elemento, aux)) {
		return false;
	}

	if (!abb_con_cada_elemento_preorden(raiz->izquierda, funcion, aux,
					    contador)) {
		return false;
	}

	if (!abb_con_cada_elemento_preorden(raiz->derecha, funcion, aux,
					    contador)) {
		return false;
	}

	return true;
}

bool abb_con_cada_elemento_postorden(nodo_abb_t *raiz,
				     bool (*funcion)(void *, void *), void *aux,
				     size_t *contador)
{
	if (!raiz) {
		return true;
	}

	if (!abb_con_cada_elemento_postorden(raiz->izquierda, funcion, aux,
					     contador)) {
		return false;
	}

	if (!abb_con_cada_elemento_postorden(raiz->derecha, funcion, aux,
					     contador)) {
		return false;
	}

	(*contador)++;

	if (!funcion(raiz->elemento, aux)) {
		return false;
	}

	return true;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion) {
		return ERROR;
	}

	size_t contador = 0;

	if (recorrido == INORDEN) {
		abb_con_cada_elemento_inorden(arbol->nodo_raiz, funcion, aux,
					      &contador);
	} else if (recorrido == PREORDEN) {
		abb_con_cada_elemento_preorden(arbol->nodo_raiz, funcion, aux,
					       &contador);
	} else if (recorrido == POSTORDEN) {
		abb_con_cada_elemento_postorden(arbol->nodo_raiz, funcion, aux,
						&contador);
	}

	else {
		return ERROR;
	}

	return contador;
}

bool convertir_a_array(void *elemento, void *array)
{
	if (!array) {
		return false;
	}

	array_limitado_t *aux = array;

	if (aux->cantidad >= aux->tope) {
		return false;
	}

	aux->array[aux->cantidad] = elemento;

	aux->cantidad++;

	return true;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol || !array) {
		return ERROR;
	}

	array_limitado_t aux;
	aux.array = array;
	aux.tope = tamanio_array;
	aux.cantidad = 0;

	abb_con_cada_elemento(arbol, recorrido, convertir_a_array, &aux);

	return aux.cantidad;
}

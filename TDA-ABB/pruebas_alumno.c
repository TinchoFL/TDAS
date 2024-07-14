#include "pa2m.h"
#include "src/abb.h"
#include "src/abb_estructura_privada.h"
#include <stdio.h>
#include <stdbool.h>

int comparar_elementos(void *elemento1, void *elemento2)
{
	int *i1 = elemento1;
	int *i2 = elemento2;

	return *i1 - *i2;
}

void prueba_crear_arbol()
{
	abb_t *arbol = abb_crear(comparar_elementos);

	pa2m_afirmar(arbol != NULL, "Se creo el Arbol");
	pa2m_afirmar(arbol->nodo_raiz == NULL, "La raiz es nula");
	pa2m_afirmar(arbol->tamanio == 0, "El tamanio del Arbol es 0");
	pa2m_afirmar(abb_vacio(arbol), "El Arbol está vacio");
	pa2m_afirmar(abb_crear(NULL) == NULL,
		     "Devuelve NULL si el comparador es nulo");

	abb_destruir(arbol);
}

void prueba_insertar_elementos()
{
	abb_t *arbol = abb_crear(comparar_elementos);

	int elemento1 = 23;
	int elemento2 = 12;
	int elemento3 = 59;
	int elemento4 = 47;
	int elemento5 = 9;

	abb_insertar(arbol, &elemento1);
	abb_insertar(arbol, &elemento2);
	abb_insertar(arbol, &elemento3);
	abb_insertar(arbol, &elemento4);
	abb_insertar(arbol, &elemento5);

	pa2m_afirmar(((arbol->nodo_raiz->elemento) == &elemento1),
		     "El elemento de la raiz es el primer elemento insertado");
	pa2m_afirmar(((arbol->nodo_raiz->izquierda->elemento) == &elemento2),
		     "El elemento izquierdo de la raiz es 12");
	pa2m_afirmar(((arbol->nodo_raiz->derecha->elemento) == &elemento3),
		     "El elemento derecho de la raiz es 59");
	pa2m_afirmar(((arbol->nodo_raiz->izquierda->izquierda->elemento) ==
		      &elemento5),
		     "El hijo izquierdo del 12 es 9");
	pa2m_afirmar(((arbol->nodo_raiz->derecha->izquierda->elemento) ==
		      &elemento4),
		     "El hijo izquierdo del 59 es 47");

	pa2m_afirmar(arbol->tamanio == 5, "El tamanio del arbol es 5");
	pa2m_afirmar(abb_insertar(NULL, &elemento1) == NULL,
		     "Devuelve NULL si el arbol es nulo");

	abb_destruir(arbol);
}

void prueba_buscar_elementos()
{
	abb_t *arbol = abb_crear(comparar_elementos);

	int elemento1 = 23;
	int elemento2 = 12;
	int elemento3 = 59;
	int elemento4 = 47;
	int elemento5 = 9;

	abb_insertar(arbol, &elemento1);
	abb_insertar(arbol, &elemento2);
	abb_insertar(arbol, &elemento3);
	abb_insertar(arbol, &elemento4);

	pa2m_afirmar(*(int *)abb_buscar(arbol, &elemento1) == elemento1,
		     "El elemento encontrado es 23");
	pa2m_afirmar(*(int *)abb_buscar(arbol, &elemento3) == elemento3,
		     "El elemento encontrado es 59");
	pa2m_afirmar(*(int *)abb_buscar(arbol, &elemento4) == elemento4,
		     "El elemento encontrado es 47");
	pa2m_afirmar(abb_buscar(arbol, &elemento5) == NULL,
		     "El elemento buscado no esta en el arbol");
	pa2m_afirmar(abb_buscar(NULL, &elemento5) == NULL,
		     "Si el Arbol no existe devuelve NULL");

	abb_destruir(arbol);
}

void prueba_eliminar_elementos()
{
	abb_t *arbol = abb_crear(comparar_elementos);

	int elemento1 = 23;
	int elemento2 = 12;
	int elemento3 = 5;
	int elemento4 = 16;
	int elemento5 = 31;
	int elemento6 = 44;
	int elemento7 = 37;

	abb_insertar(arbol, &elemento1);
	abb_insertar(arbol, &elemento2);
	abb_insertar(arbol, &elemento3);
	abb_insertar(arbol, &elemento4);
	abb_insertar(arbol, &elemento5);
	abb_insertar(arbol, &elemento6);
	abb_insertar(arbol, &elemento7);

	pa2m_afirmar(abb_quitar(NULL, &elemento7) == NULL,
		     "Devuelve NULL si el arbol es nulo");
	pa2m_afirmar(*(int *)abb_quitar(arbol, &elemento7) == elemento7,
		     "Se pudo quitar el 37");
	pa2m_afirmar(abb_tamanio(arbol) == 6,
		     "El arbol ahora tiene 6 elementos");
	pa2m_afirmar(abb_quitar(arbol, &elemento7) == NULL,
		     "Devuelve NULL si el elemento no existe");

	pa2m_afirmar(*(int *)abb_quitar(arbol, &elemento5) == elemento5,
		     "Se pudo quitar el 31, elemento que tiene un solo hijo");
	pa2m_afirmar(abb_tamanio(arbol) == 5,
		     "El arbol ahora tiene 5 elementos");
	pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &elemento6,
		     "El nuevo hijo derecho de la raiz ahora es 44");

	pa2m_afirmar(*(int *)abb_quitar(arbol, &elemento2) == elemento2,
		     "Se pudo quitar el 12, elemento que tiene 2 hijos");
	pa2m_afirmar(abb_tamanio(arbol) == 4,
		     "El arbol ahora tiene 4 elementos");
	pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == &elemento3,
		     "El nuevo hijo izquierdo de la raiz ahora es 5");

	pa2m_afirmar(*(int *)abb_quitar(arbol, &elemento1) == elemento1,
		     "Se pudo quitar la raiz");
	pa2m_afirmar(abb_tamanio(arbol) == 3,
		     "El arbol ahora tiene 3 elementos");
	pa2m_afirmar(arbol->nodo_raiz->elemento == &elemento4,
		     "La nueva raiz es 16");
	pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == &elemento3,
		     "El hijo izquierdo de la nueva raiz es 5");
	pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &elemento6,
		     "El hijo derecho de la nueva raiz es 44");

	pa2m_afirmar(*(int *)abb_quitar(arbol, &elemento6) == elemento6,
		     "Se pudo quitar el 44, que no tiene hijos");
	pa2m_afirmar(abb_tamanio(arbol) == 2,
		     "El arbol ahora tiene 2 elementos");

	abb_insertar(arbol, &elemento7);
	pa2m_afirmar(
		*(int *)abb_buscar(arbol, &elemento7) == elemento7,
		"Se agrega un elemento anteriormente borrado al arbol(37)");
	pa2m_afirmar(
		arbol->nodo_raiz->derecha->elemento == &elemento7,
		"El nuevo hijo derecho de la raiz es el elemento agregado(37)");
	pa2m_afirmar(abb_tamanio(arbol) == 3,
		     "El arbol ahora tiene 3 elementos");

	abb_destruir(arbol);
}

int comparar_chars(void *char1, void *char2)
{
	char *c1 = char1;
	char *c2 = char2;

	return (*c1) - (*c2);
}

bool mostrar_elemento(void *elemento, void *aux)
{
	char *c = elemento;
	printf("%c", *c);

	return true;
}

void prueba_iterador_interno()
{
	abb_t *arbol = abb_crear(comparar_chars);

	char elemento1 = 'y';
	char elemento2 = 'h';
	char elemento3 = 'c';
	char elemento4 = 'z';
	char elemento5 = 'q';
	char elemento6 = 'o';
	char elemento7 = 'i';

	abb_insertar(arbol, &elemento1);
	abb_insertar(arbol, &elemento2);
	abb_insertar(arbol, &elemento3);
	abb_insertar(arbol, &elemento4);
	abb_insertar(arbol, &elemento5);
	abb_insertar(arbol, &elemento6);
	abb_insertar(arbol, &elemento7);

	pa2m_afirmar((abb_con_cada_elemento(NULL, INORDEN, mostrar_elemento,
					    NULL) == 0),
		     "Devuelve 0 si el arbol es nulo");
	pa2m_afirmar((abb_con_cada_elemento(arbol, INORDEN, NULL, NULL) == 0),
		     "Devuelve 0 si la funcion es nula");
	pa2m_afirmar(
		(abb_con_cada_elemento(arbol, INORDEN, mostrar_elemento,
				       NULL) == 7),
		"Aplica la funcion a todos los elementos del arbol de manera INORDEN");
	pa2m_afirmar(
		(abb_con_cada_elemento(arbol, PREORDEN, mostrar_elemento,
				       NULL) == 7),
		"Aplica la funcion a todos los elementos del arbol de manera PREORDEN");
	pa2m_afirmar(
		(abb_con_cada_elemento(arbol, POSTORDEN, mostrar_elemento,
				       NULL) == 7),
		"Aplica la funcion a todos los elementos del arbol de manera POSTORDEN");

	abb_destruir(arbol);
}

bool mostrar_elemento_con_corte(void *elemento, void *aux)
{
	char *caracter = elemento;
	printf("%c", *caracter);

	if (*caracter == 'o') {
		return false;
	}

	return true;
}

void prueba_iterador_interno_con_corte()
{
	abb_t *arbol = abb_crear(comparar_chars);

	char elemento1 = 'y';
	char elemento2 = 'h';
	char elemento3 = 'c';
	char elemento4 = 'z';
	char elemento5 = 'q';
	char elemento6 = 'o';
	char elemento7 = 'i';

	abb_insertar(arbol, &elemento1);
	abb_insertar(arbol, &elemento2);
	abb_insertar(arbol, &elemento3);
	abb_insertar(arbol, &elemento4);
	abb_insertar(arbol, &elemento5);
	abb_insertar(arbol, &elemento6);
	abb_insertar(arbol, &elemento7);

	pa2m_afirmar(
		(abb_con_cada_elemento(arbol, INORDEN,
				       mostrar_elemento_con_corte, NULL) == 4),
		"Aplica la funcion hasta encontrar el elemento 'o' de manera INORDEN");
	pa2m_afirmar(
		(abb_con_cada_elemento(arbol, PREORDEN,
				       mostrar_elemento_con_corte, NULL) == 5),
		"Aplica la funcion hasta encontrar el elemento 'o' de manera PREORDEN");
	pa2m_afirmar(
		(abb_con_cada_elemento(arbol, POSTORDEN,
				       mostrar_elemento_con_corte, NULL) == 3),
		"Aplica la funcion hasta encontrar el elemento 'o' de manera POSTORDEN");

	abb_destruir(arbol);
}

void prueba_recorrer_y_guardar_en_vector()
{
	abb_t *arbol = abb_crear(comparar_chars);

	char elemento1 = 'y';
	char elemento2 = 'h';
	char elemento3 = 'c';
	char elemento4 = 'z';
	char elemento5 = 'q';
	char elemento6 = 'o';
	char elemento7 = 'i';

	abb_insertar(arbol, &elemento1);
	abb_insertar(arbol, &elemento2);
	abb_insertar(arbol, &elemento3);
	abb_insertar(arbol, &elemento4);
	abb_insertar(arbol, &elemento5);
	abb_insertar(arbol, &elemento6);
	abb_insertar(arbol, &elemento7);

	char *vectormin[2];
	char *vector[5];
	char *vectormax[10];

	pa2m_afirmar(abb_recorrer(arbol, INORDEN, NULL, 5) == 0,
		     "Devuelve 0 si el array es nulo");
	pa2m_afirmar(abb_recorrer(arbol, INORDEN, (void **)vector, 0) == 0,
		     "Devuelve 0 si el tamanio del array es 0");
	pa2m_afirmar(abb_recorrer(NULL, INORDEN, (void **)vector, 5) == 0,
		     "Devuelve 0 si el arbol es nulo");
	pa2m_afirmar(
		abb_recorrer(arbol, INORDEN, (void **)vectormin, 2) == 2,
		"Se manda un array con un tamaño menor a la cantidad de elementos");
	pa2m_afirmar(
		abb_recorrer(arbol, INORDEN, (void **)vectormax, 10) == 7,
		"Se manda un array con un tamaño mayor a la cantidad de elementos");
	pa2m_afirmar(
		abb_recorrer(arbol, INORDEN, (void **)vector, 5) == 5,
		"Se guardan 5 elementos en el vector recorriendo el arbol de manera INORDEN");
	pa2m_afirmar((vector[0] == &elemento3),
		     "El primer elemento del vector INORDEN es la letra c");
	pa2m_afirmar(
		(vector[4] == &elemento5),
		"El ultimo elemento agregado del vector POSTORDEN es la letra q");
	pa2m_afirmar(
		abb_recorrer(arbol, PREORDEN, (void **)vector, 5) == 5,
		"Se guardan 5 elementos en el vector recorriendo el arbol de manera PREORDEN");
	pa2m_afirmar((vector[0] == &elemento1),
		     "El primer elemento del vector INORDEN es la letra y");
	pa2m_afirmar(
		(vector[4] == &elemento6),
		"El ultimo elemento agregado del vector POSTORDEN es la letra o");
	pa2m_afirmar(
		abb_recorrer(arbol, POSTORDEN, (void **)vector, 5) == 5,
		"Se guardan 5 elementos en el vector recorriendo el arbol de manera PREORDEN");
	pa2m_afirmar((vector[0] == &elemento3),
		     "El primer elemento del vector INORDEN es la letra c");
	pa2m_afirmar(
		(vector[4] == &elemento2),
		"El ultimo elemento agregado del vector POSTORDEN es la letra h");

	abb_destruir(arbol);
}

int main()
{
	pa2m_nuevo_grupo("Prueba crear Arbol");
	prueba_crear_arbol();

	pa2m_nuevo_grupo("Pruebas insertar elementos");
	prueba_insertar_elementos();

	pa2m_nuevo_grupo("Pruebas buscar elementos");
	prueba_buscar_elementos();

	pa2m_nuevo_grupo("Pruebas eliminar elementos");
	prueba_eliminar_elementos();

	pa2m_nuevo_grupo("Pruebas con cada elemento");
	prueba_iterador_interno();
	prueba_iterador_interno_con_corte();

	pa2m_nuevo_grupo("Pruebas recorrer y guardar en vector");
	prueba_recorrer_y_guardar_en_vector();

	return pa2m_mostrar_reporte();
}

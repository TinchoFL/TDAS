#include "pa2m.h"
#include "./src/lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./src/cola.h"
#include "./src/pila.h"

void prueba_crear_lista()
{
	lista_t *lista = lista_crear();

	pa2m_afirmar((lista != NULL), "Se creo la lista");

	lista_destruir(lista);
}

void prueba_lista_nula()
{
	lista_t *lista = NULL;

	pa2m_afirmar((lista_primero(lista) == NULL),
		     "El primer elemento de la lista es NULL");
	pa2m_afirmar((lista_ultimo(lista) == NULL),
		     "El ultimo elemento de la lista es NULL");
	pa2m_afirmar((lista_vacia(lista) == true), "La lista esta vacia");
	pa2m_afirmar((lista_tamanio(lista) == 0),
		     "El tamanio de la lista es 0");

	lista_destruir(lista);
}

void prueba_lista_con_3_elementos()
{
	lista_t *lista = lista_crear();

	char elemento1 = 'a';
	char elemento2 = 'b';
	char elemento3 = 'c';

	lista = lista_insertar(lista, &elemento1);
	lista = lista_insertar(lista, &elemento2);
	lista = lista_insertar(lista, &elemento3);

	pa2m_afirmar((lista_primero(lista) == &elemento1),
		     "El primer elemento de la lista es 1");
	pa2m_afirmar((lista_ultimo(lista) == &elemento3),
		     "El ultimo elemento de la lista es 3");
	pa2m_afirmar((lista_tamanio(lista) == 3), "La lista tiene 2 elementos");
	pa2m_afirmar((lista_vacia(lista) == false), "La lista no esta vacia");
	pa2m_afirmar(*(char *)lista_elemento_en_posicion(lista, 1) == 'b',
		     "El elemento en posición 1 es 'b'");
	pa2m_afirmar((lista_elemento_en_posicion(lista, 3) == NULL),
		     "No puedo obtener elemento en posicion invalida");

	lista_destruir(lista);
}

void prueba_lista_con_muchisimos_elementos()
{
	lista_t *lista = lista_crear();

	int elementos[10000];

	for (int i = 0; i < 10000; i++) {
		elementos[i] = i;
		lista_insertar(lista, &elementos[i]);
	}

	pa2m_afirmar(
		(lista_primero(lista) == &elementos[0]),
		"El primer elemento de la lista es el primer elemento agregado");
	pa2m_afirmar(
		(lista_ultimo(lista) == &elementos[9999]),
		"El ultimo elemento de la lista es el ultimo elemento agregado");
	pa2m_afirmar((lista_vacia(lista) == false), "La lista no esta vacia");
	pa2m_afirmar((lista_tamanio(lista) == 10000),
		     "La lista tiene 10mil elementos");

	lista_destruir(lista);
}

void prueba_insertar_en_lista_vacia()
{
	lista_t *lista = lista_crear();

	int elemento1 = 1;

	pa2m_afirmar((lista_insertar_en_posicion(lista, &elemento1, 0) != NULL),
		     "Puedo insertar elemento en lista vacia en la posicion 0");
	pa2m_afirmar((lista_primero(lista) == &elemento1),
		     "El primer elemento de la lista es 1");
	pa2m_afirmar((lista_ultimo(lista) == &elemento1),
		     "El ultimo elemento de la lista es 1");
	pa2m_afirmar((lista_tamanio(lista) == 1), "La lista tiene 1 elementos");
	pa2m_afirmar(
		(lista_insertar_en_posicion(lista, &elemento1, 3) != NULL),
		"Puedo insertar elemento en lista vacia en posicion invalida");
	pa2m_afirmar((lista_insertar_en_posicion(NULL, &elemento1, 0) == NULL),
		     "Devuelve NULL en caso de error");

	lista_destruir(lista);
}

void prueba_insertar_en_lista_con_elementos()
{
	lista_t *lista = lista_crear();

	int elemento1 = 1;
	int elemento2 = 2;
	int elemento3 = 3;
	int elemento4 = 4;
	int elemento5 = 5;

	lista = lista_insertar(lista, &elemento1);
	lista = lista_insertar(lista, &elemento2);
	lista = lista_insertar(lista, &elemento3);
	lista = lista_insertar(lista, &elemento4);

	pa2m_afirmar(
		(lista_insertar_en_posicion(lista, &elemento5, 0) != NULL),
		"Puedo insertar elemento en lista con elementos en la posicion 0");
	pa2m_afirmar((lista_primero(lista) == &elemento5),
		     "El primer elemento de la lista es el elemento agregado");
	pa2m_afirmar((lista_tamanio(lista) == 5), "La lista tiene 5 elementos");

	lista_destruir(lista);
}

void prueba_insertar_al_final_de_lista_con_elementos()
{
	lista_t *lista = lista_crear();

	int elemento1 = 1;
	int elemento2 = 2;
	int elemento3 = 3;
	int elemento4 = 4;
	int elemento5 = 5;

	lista = lista_insertar(lista, &elemento1);
	lista = lista_insertar(lista, &elemento2);
	lista = lista_insertar(lista, &elemento3);
	lista = lista_insertar(lista, &elemento4);

	pa2m_afirmar(
		(lista_insertar_en_posicion(lista, &elemento5, 4) != NULL),
		"Puedo insertar elemento en lista con elementos en posicion final");
	pa2m_afirmar((lista_ultimo(lista) == &elemento5),
		     "El ultimo elemento de la lista es el elemento agregado");
	pa2m_afirmar((lista_tamanio(lista) == 5), "La lista tiene 5 elementos");

	lista_destruir(lista);
}

void prueba_insertar_en_cualquier_lugar_de_lista_con_elementos()
{
	lista_t *lista = lista_crear();

	int elemento1 = 1;
	int elemento2 = 2;
	int elemento3 = 3;
	int elemento4 = 4;
	int elemento5 = 5;

	lista = lista_insertar(lista, &elemento1);
	lista = lista_insertar(lista, &elemento2);
	lista = lista_insertar(lista, &elemento3);
	lista = lista_insertar(lista, &elemento4);

	pa2m_afirmar((lista_insertar_en_posicion(lista, &elemento5, 2) != NULL),
		     "Puedo insertar elemento en la posicion 3");
	pa2m_afirmar((lista_elemento_en_posicion(lista, 2) == &elemento5),
		     "El elemento agregado esta en la posicion 3");
	pa2m_afirmar((lista_tamanio(lista) == 5), "La lista tiene 5 elementos");

	lista_destruir(lista);
}

void prueba_quitar_lista_nula()
{
	lista_t *lista = NULL;

	pa2m_afirmar((lista_quitar(lista) == NULL),
		     "No se puede quitar porque la lista no existe");

	lista_destruir(lista);
}

void prueba_quitar_en_lista_con_elementos()
{
	lista_t *lista = lista_crear();

	int elemento1 = 1;
	int elemento2 = 2;

	lista = lista_insertar(lista, &elemento1);
	lista = lista_insertar(lista, &elemento2);

	pa2m_afirmar(
		(lista_quitar(lista) == &elemento2),
		"El elemento quitado es el que estaba en la ultima posicion");
	pa2m_afirmar((lista_ultimo(lista) == &elemento1),
		     "El nuevo ultimo es asignado correctamente");
	pa2m_afirmar((lista_tamanio(lista) == 1), "La lista tiene 1 elemento");

	pa2m_afirmar((lista_quitar(lista) == &elemento1),
		     "El elemento quitado es el ultimo que estaba en la lista");
	pa2m_afirmar(
		(lista_primero(lista) == NULL),
		"Al estar la lista vacia, el nuevo primero elemento es NULL");
	pa2m_afirmar(
		(lista_ultimo(lista) == NULL),
		"Al estar la lista vacia, el nuevo ultimo elemento es NULL");
	pa2m_afirmar(lista_vacia(lista) == true, "La lista esta vacia");

	lista_destruir(lista);
}

void prueba_quitar_de_posicion_lista_nula()
{
	lista_t *lista = NULL;

	pa2m_afirmar((lista_quitar_de_posicion(lista, 0) == NULL),
		     "No se puede quitar porque la lista no existe");

	lista_destruir(lista);
}

void prueba_quitar_de_posicion_en_lista_con_elementos()
{
	lista_t *lista = lista_crear();

	int elemento1 = 1;
	int elemento2 = 2;
	int elemento3 = 3;

	lista = lista_insertar(lista, &elemento1);
	lista = lista_insertar(lista, &elemento2);
	lista = lista_insertar(lista, &elemento3);

	pa2m_afirmar((lista_quitar_de_posicion(lista, 0) == &elemento1),
		     "Se quita el elemento de la primer posicion");
	pa2m_afirmar(lista_tamanio(lista) == 2,
		     "La lista queda con 2 elementos");

	pa2m_afirmar((lista_quitar_de_posicion(lista, 1) == &elemento3),
		     "Se quita el elemento de la segunda posicion");
	pa2m_afirmar(lista_primero(lista) == &elemento2,
		     "El elemento que quedaba ahora es el primero en la lista");
	pa2m_afirmar(lista_tamanio(lista) == 1,
		     "La lista se queda con 1 elemento");

	pa2m_afirmar((lista_quitar_de_posicion(lista, 0) == &elemento2),
		     "Se quita el ultimo elemento");
	pa2m_afirmar(lista_primero(lista) == NULL,
		     "El primer elemento es NULL");
	pa2m_afirmar(lista_ultimo(lista) == NULL, "El ultimo elemento es NULL");
	pa2m_afirmar(lista_vacia(lista) == true, "La lista esta vacia");
	pa2m_afirmar((lista_quitar_de_posicion(NULL, 0) == NULL),
		     "Devuelve NULL en caso de error");

	lista_destruir(lista);
}

int comparar_elementos(void *elemento1, void *elemento2)
{
	if (!elemento1 || !elemento2) {
		return -1;
	}

	return strcmp(elemento1, elemento2);
}

void prueba_buscar_elementos()
{
	lista_t *lista = lista_crear();

	int elemento1 = 1;
	int elemento2 = 2;
	int elemento3 = 3;

	lista = lista_insertar(lista, &elemento1);
	lista = lista_insertar(lista, &elemento2);

	pa2m_afirmar(*(int *)lista_buscar_elemento(lista, comparar_elementos,
						   &elemento1) == elemento1,
		     "El elemento buscado es 1");
	pa2m_afirmar(*(int *)lista_buscar_elemento(lista, comparar_elementos,
						   &elemento2) == elemento2,
		     "El elemento buscado es 2");
	pa2m_afirmar(lista_buscar_elemento(lista, comparar_elementos,
					   &elemento3) == NULL,
		     "Si el elemento no existe devuelve NULL");

	lista_destruir(lista);
}

void prueba_iterador_externo_en_lista_vacia()
{
	lista_t *lista = lista_crear();

	lista_iterador_t *iterador = lista_iterador_crear(lista);

	pa2m_afirmar(iterador != NULL,
		     "Se creo el iterador externo en lista vacia");
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador) == false,
		     "El iterador no tiene nodo siguiente");
	pa2m_afirmar(lista_iterador_avanzar(iterador) == false,
		     "El iterador no avanza ya que la lista esta vacia");
	pa2m_afirmar(lista_iterador_elemento_actual(iterador) == NULL,
		     "El elemento del iterador es NULL");

	lista_destruir(lista);
	lista_iterador_destruir(iterador);
}

void prueba_iterador_externo_en_lista_con_elementos()
{
	lista_t *lista = lista_crear();

	int elemento1 = 1;
	int elemento2 = 2;
	int elemento3 = 3;

	lista = lista_insertar(lista, &elemento1);
	lista = lista_insertar(lista, &elemento2);
	lista = lista_insertar(lista, &elemento3);

	lista_iterador_t *iterador = lista_iterador_crear(lista);

	pa2m_afirmar(iterador != NULL,
		     "Se creo el iterador externo en lista con elementos");
	pa2m_afirmar(*(int *)lista_iterador_elemento_actual(iterador) ==
			     elemento1,
		     "El primer elemento de la lista es 1");
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador),
		     "El iterador tiene un nodo siguiente");
	pa2m_afirmar(lista_iterador_avanzar(iterador),
		     "El iterador itera sin problemas");

	pa2m_afirmar(*(int *)lista_iterador_elemento_actual(iterador) ==
			     elemento2,
		     "El segundo elemento de la lista es 2");
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador),
		     "El iterador tiene un nodo siguiente");
	pa2m_afirmar(lista_iterador_avanzar(iterador),
		     "El iterador itera sin problemas");

	pa2m_afirmar(*(int *)lista_iterador_elemento_actual(iterador) ==
			     elemento3,
		     "El tercer elemento de la lista es 3");
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador),
		     "El iterador tiene un nodo siguiente");
	pa2m_afirmar(lista_iterador_avanzar(iterador) == false,
		     "El iterador no puede iterar");
	pa2m_afirmar(lista_iterador_elemento_actual(iterador) == NULL,
		     "El elemento del nodo NULL");
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador) == false,
		     "El iterador no tiene un nodo siguiente");

	lista_destruir(lista);
	lista_iterador_destruir(iterador);
}

bool iterar_en_toda_la_lista(void *elemento1, void *contexto)
{
	return true;
}

void prueba_iterador_interno_en_lista_con_elementos()
{
	lista_t *lista = lista_crear();

	size_t elemento1 = 1;
	size_t elemento2 = 2;
	size_t elemento3 = 3;

	lista = lista_insertar(lista, &elemento1);
	lista = lista_insertar(lista, &elemento2);
	lista = lista_insertar(lista, &elemento3);

	pa2m_afirmar(lista_con_cada_elemento(lista, NULL, 0) == 0,
		     "Si no hay funcion no itera");
	pa2m_afirmar(lista_con_cada_elemento(NULL, iterar_en_toda_la_lista,
					     0) == 0,
		     "Si no hay lista no itera");
	pa2m_afirmar(lista_con_cada_elemento(lista, iterar_en_toda_la_lista,
					     0) == 3,
		     "Itera en todos los elementos de la lista");

	lista_destruir(lista);
}

void pruebas_cola_vacia()
{
	cola_t *cola = cola_crear();

	pa2m_afirmar(cola != NULL, "Se crea la cola");
	pa2m_afirmar(cola_vacia(cola) == true, "La cola esta vacia");
	pa2m_afirmar(cola_desencolar(cola) == NULL,
		     "No se puede desencolar si la cola esta vacia");

	cola_destruir(cola);
}

void pruebas_cola_con_elementos()
{
	cola_t *cola = cola_crear();

	int elemento1 = 1;
	int elemento2 = 2;
	int elemento3 = 3;

	cola_encolar(cola, &elemento1);
	cola_encolar(cola, &elemento2);
	cola_encolar(cola, &elemento3);

	pa2m_afirmar(cola_tamanio(cola) == 3,
		     "Se pudieron insertar tiene 3 elementos en la cola");
	pa2m_afirmar(cola_desencolar(cola) == &elemento1,
		     "Se desencola el primer elemento de la cola");
	pa2m_afirmar(cola_tamanio(cola) == 2,
		     "El tamanio de la cola ahora es 2");
	pa2m_afirmar(*(int *)cola_frente(cola) == elemento2,
		     "El primer elemento de la cola es 2");

	cola_destruir(cola);
}

void pruebas_pila_vacia()
{
	pila_t *pila = pila_crear();

	pa2m_afirmar(pila != NULL, "Se crea la pila");
	pa2m_afirmar(pila_vacia(pila) == true, "La pila esta vacia");
	pa2m_afirmar(pila_desapilar(pila) == NULL,
		     "No se puede desapilar si la pila esta vacia");

	pila_destruir(pila);
}

void pruebas_pila_con_elementos()
{
	pila_t *pila = pila_crear();

	int elemento1 = 1;
	int elemento2 = 2;
	int elemento3 = 3;

	pila_apilar(pila, &elemento1);
	pila_apilar(pila, &elemento2);
	pila_apilar(pila, &elemento3);

	pa2m_afirmar(pila_tamanio(pila) == 3,
		     "Se pudieron insertar tiene 3 elementos en la pila");
	pa2m_afirmar(pila_desapilar(pila) == &elemento3,
		     "Se desapila el ultimo elemento en la pila");
	pa2m_afirmar(pila_tamanio(pila) == 2,
		     "El tamanio de la pila ahora es 2");
	pa2m_afirmar(*(int *)pila_tope(pila) == elemento2,
		     "El ultimo elemento de la pila es 2");

	pila_destruir(pila);
}

int main()
{
	pa2m_nuevo_grupo("Prueba crear lista");
	prueba_crear_lista();

	pa2m_nuevo_grupo("Pruebas con una lista nula");
	prueba_lista_nula();

	pa2m_nuevo_grupo("Pruebas con una lista de 3 elementos");
	prueba_lista_con_3_elementos();

	pa2m_nuevo_grupo("Pruebas lista con muchos elementos");
	prueba_lista_con_muchisimos_elementos();

	pa2m_nuevo_grupo("Pruebas insertar elemento en una lista vacia");
	prueba_insertar_en_lista_vacia();

	pa2m_nuevo_grupo(
		"Pruebas insertar elementos en una lista con elementos");
	prueba_insertar_en_lista_con_elementos();
	prueba_insertar_al_final_de_lista_con_elementos();
	prueba_insertar_en_cualquier_lugar_de_lista_con_elementos();

	pa2m_nuevo_grupo("Pruebas quitar elementos");
	prueba_quitar_lista_nula();
	prueba_quitar_en_lista_con_elementos();

	pa2m_nuevo_grupo("Pruebas de quitar en posicion");
	prueba_quitar_de_posicion_lista_nula();
	prueba_quitar_de_posicion_en_lista_con_elementos();

	pa2m_nuevo_grupo("Pruebas de busqueda");
	prueba_buscar_elementos();

	pa2m_nuevo_grupo("Pruebas del iterador externo");
	prueba_iterador_externo_en_lista_vacia();
	prueba_iterador_externo_en_lista_con_elementos();

	pa2m_nuevo_grupo("Pruebas del iterador interno");
	prueba_iterador_interno_en_lista_con_elementos();

	pa2m_nuevo_grupo("Pruebas de cola");
	pruebas_cola_vacia();
	pruebas_cola_con_elementos();

	pa2m_nuevo_grupo("Pruebas de pila");
	pruebas_pila_vacia();
	pruebas_pila_con_elementos();

	return pa2m_mostrar_reporte();
}

#include "pa2m.h"
#include "src/hash.h"
#include <string.h>
#include <stdlib.h>
#include "src/estructura_hash.h"

void prueba_crear_hash()
{
	hash_t *hash = hash_crear(6);

	pa2m_afirmar(hash != NULL, "Se puede crear el Hash");
	pa2m_afirmar(hash_cantidad(hash) == 0, "El Hash esta vacio");
	pa2m_afirmar(hash_cantidad(NULL) == 0,
		     "hash_cantidad devuelve 0 en caso de error");

	hash_destruir(hash);

	hash = hash_crear(4);

	pa2m_afirmar(
		hash != NULL,
		"Se puede crear el Hash aun teniendo menor capacidad a la minima(3)");

	hash_destruir(hash);
}

void pruebas_insertar()
{
	hash_t *hash = hash_crear(5);

	void *anterior = NULL;

	pa2m_afirmar(hash_insertar(NULL, "1", "a", NULL) == NULL,
		     "Devuelve NULL si el Hash es nulo");
	pa2m_afirmar(hash_insertar(hash, NULL, "a", NULL) == NULL,
		     "Devuelve NULL si la clave es nula");

	pa2m_afirmar(hash_insertar(hash, "1", "a", &anterior) != NULL,
		     "Se inserta <1,a> al Hash");
	pa2m_afirmar(hash_cantidad(hash) == 1, "El hash tiene un elemento");
	pa2m_afirmar(hash_insertar(hash, "2", "b", &anterior) != NULL,
		     "Se inserta <2,b> al Hash");
	pa2m_afirmar(hash_insertar(hash, "3", "c", &anterior) != NULL,
		     "Se inserta <3,c> al Hash");
	pa2m_afirmar(hash_insertar(hash, "4", "d", &anterior) != NULL,
		     "Se inserta <4,d> al Hash");

	pa2m_afirmar(hash_cantidad(hash) == 4, "El hash tiene 4 elementos");

	pa2m_afirmar(hash_insertar(hash, "3", "c", &anterior) != NULL,
		     "Se inserta un elemento de clave repetida al Hash");
	pa2m_afirmar(
		strcmp(anterior, "c") == 0,
		"Se guarda en 'anterior' lo que habia en el elemento antes de ser reemplazado");
	pa2m_afirmar(hash_cantidad(hash) == 4,
		     "El hash sigue teniendo 4 elementos");

	hash_destruir_todo(hash, NULL);
}

void pruebas_insertar_muchos_elementos()
{
	hash_t *hash = hash_crear(3);

	void *anterior = NULL;

	int array[50000];

	for (int i = 0; i < 50000; i++) {
		array[i] = i;
		char *clave = malloc(6 * sizeof(char));
		sprintf(clave, "%i", i);
		hash_insertar(hash, clave, &array[i], &anterior);

		free(clave);
	}

	pa2m_afirmar(hash_cantidad(hash) == 50000,
		     "Se pudieron insertar 50000 elementos");

	hash_destruir_todo(hash, NULL);
}

void pruebas_rehash()
{
	hash_t *hash = hash_crear(3);

	void *anterior = NULL;

	pa2m_afirmar(hash_cantidad(hash) == 0, "El Hash esta vacio");
	pa2m_afirmar(hash->capacidad == 3, "La capacidad del hash es de 3");

	pa2m_afirmar(hash_insertar(hash, "1", "a", &anterior) != NULL,
		     "Se inserta <1,a> al Hash");
	pa2m_afirmar(hash_insertar(hash, "2", "b", &anterior) != NULL,
		     "Se inserta <2,b> al Hash");
	pa2m_afirmar(hash_insertar(hash, "3", "c", &anterior) != NULL,
		     "Se inserta <3,c> al Hash");
	pa2m_afirmar(hash_insertar(hash, "4", "d", &anterior) != NULL,
		     "Se inserta <4,d> al Hash");

	pa2m_afirmar(hash_cantidad(hash) == 4, "El hash tiene 4 elementos");
	pa2m_afirmar(hash->capacidad == 6,
		     "La capacidad del hash ahora es de 6");

	pa2m_afirmar(hash_insertar(hash, "5", "e", &anterior) != NULL,
		     "Se inserta <5,e> al Hash");
	pa2m_afirmar(hash_insertar(hash, "6", "f", &anterior) != NULL,
		     "Se inserta <6,f> al Hash");
	pa2m_afirmar(hash_insertar(hash, "7", "g", &anterior) != NULL,
		     "Se inserta <7,g> al Hash");
	pa2m_afirmar(hash_insertar(hash, "8", "h", &anterior) != NULL,
		     "Se inserta <8,h> al Hash");

	pa2m_afirmar(hash_cantidad(hash) == 8, "El hash tiene 8 elementos");
	pa2m_afirmar(hash->capacidad == 12,
		     "La capacidad del hash ahora es de 12");

	hash_destruir_todo(hash, NULL);
}

void pruebas_busqueda()
{
	hash_t *hash = hash_crear(5);

	void *anterior = NULL;

	pa2m_afirmar(hash_obtener(NULL, "1") == NULL,
		     "Devuelve NULL si el Hash es nulo");
	pa2m_afirmar(hash_obtener(hash, "1") == NULL,
		     "Devuelve NULL si el Hash esta vacio");

	pa2m_afirmar(hash_insertar(hash, "1", "a", &anterior) != NULL,
		     "Se inserta <1,a> al Hash");
	pa2m_afirmar(hash_contiene(hash, "1") == true,
		     "El Hash contiene la clave pedida");
	pa2m_afirmar(hash_insertar(hash, "2", "b", &anterior) != NULL,
		     "Se inserta <2,b> al Hash");
	pa2m_afirmar(hash_insertar(hash, "3", "c", &anterior) != NULL,
		     "Se inserta <3,c> al Hash");
	pa2m_afirmar(hash_insertar(hash, "4", "d", &anterior) != NULL,
		     "Se inserta <4,d> al Hash");

	pa2m_afirmar(strcmp(hash_obtener(hash, "3"), "c") == 0,
		     "Se obtiene el elemento buscado segun su clave");
	pa2m_afirmar(hash_obtener(hash, "asdsa") == 0,
		     "Devuelve NULL si el Hash no contiene la clave buscada");

	pa2m_afirmar(hash_insertar(hash, "1", "z", &anterior) != NULL,
		     "Se inserta la clave '1' (repetida) al Hash");
	pa2m_afirmar(strcmp(hash_obtener(hash, "1"), "z") == 0,
		     "Se obtiene el nuevo elemento asignado a la clave '1' ");
	pa2m_afirmar(
		strcmp(anterior, "a") == 0,
		"Se guarda en 'anterior' lo que habia en el elemento antes de ser reemplazado");

	hash_destruir_todo(hash, NULL);
}

void pruebas_quitar()
{
	hash_t *hash = hash_crear(5);

	void *anterior = NULL;

	pa2m_afirmar(hash_quitar(NULL, "1") == NULL,
		     "Devuelve NULL si el Hash es nulo");
	pa2m_afirmar(hash_quitar(hash, NULL) == NULL,
		     "Devuelve NULL si la clave es nula");

	pa2m_afirmar(hash_insertar(hash, "1", "a", &anterior) != NULL,
		     "Se inserta <1,a> al Hash");
	pa2m_afirmar(hash_insertar(hash, "2", "b", &anterior) != NULL,
		     "Se inserta <2,b> al Hash");
	pa2m_afirmar(hash_insertar(hash, "3", "c", &anterior) != NULL,
		     "Se inserta <3,c> al Hash");
	pa2m_afirmar(hash_insertar(hash, "4", "d", &anterior) != NULL,
		     "Se inserta <4,d> al Hash");

	pa2m_afirmar(hash_cantidad(hash) == 4, "El hash tiene 4 elementos");

	pa2m_afirmar(strcmp(hash_quitar(hash, "2"), "b") == 0,
		     "Quita y devuelve el elemento pedido");

	pa2m_afirmar(hash_cantidad(hash) == 3, "El hash tiene 3 elementos");

	pa2m_afirmar(hash_quitar(hash, "asddas") == NULL,
		     "Devuelve NULL si elemento pedido no esta en el Hash");

	pa2m_afirmar(
		hash_quitar(hash, "2") == NULL,
		"Devuelve NULL si quiero quitar un elemento que ya fue eliminado");

	pa2m_afirmar(strcmp(hash_quitar(hash, "1"), "a") == 0,
		     "Se eliminan todos los elementos del Hash");
	hash_quitar(hash, "3");
	hash_quitar(hash, "4");

	pa2m_afirmar(hash_cantidad(hash) == 0, "El hash queda con 0 elementos");

	pa2m_afirmar(
		hash_insertar(hash, "3", "c", &anterior) != NULL,
		"Se puede volver a insertar un elemento previamente quitado");

	pa2m_afirmar(hash_cantidad(hash) == 1, "El hash tiene 1 elemento");

	hash_destruir_todo(hash, NULL);
}

bool mostrar_claves(const char *clave, void *valor, void *aux)
{
	if (!clave)
		return false;

	aux = aux;

	printf("Numeros en el hash: %s\n", clave);

	return true;
}

void pruebas_con_cada_clave()
{
	hash_t *hash = hash_crear(5);

	void *anterior = NULL;

	pa2m_afirmar(hash_con_cada_clave(NULL, mostrar_claves, &anterior) == 0,
		     "Devuelve 0 si el Hash es nulo");
	pa2m_afirmar(hash_con_cada_clave(hash, mostrar_claves, &anterior) == 0,
		     "Devuelve 0 si el Hash esta vacio");
	pa2m_afirmar(hash_con_cada_clave(hash, NULL, &anterior) == 0,
		     "Devuelve 0 si la funcion es nula");

	pa2m_afirmar(hash_insertar(hash, "1", "a", &anterior) != NULL,
		     "Se inserta <1,a> al Hash");
	pa2m_afirmar(hash_insertar(hash, "2", "b", &anterior) != NULL,
		     "Se inserta <2,b> al Hash");
	pa2m_afirmar(hash_insertar(hash, "3", "c", &anterior) != NULL,
		     "Se inserta <3,c> al Hash");
	pa2m_afirmar(hash_insertar(hash, "4", "d", &anterior) != NULL,
		     "Se inserta <4,d> al Hash");
	pa2m_afirmar(hash_insertar(hash, "5", "e", &anterior) != NULL,
		     "Se inserta <5,e> al Hash");
	pa2m_afirmar(hash_insertar(hash, "6", "f", &anterior) != NULL,
		     "Se inserta <6,f> al Hash");
	pa2m_afirmar(hash_insertar(hash, "7", "g", &anterior) != NULL,
		     "Se inserta <7,g> al Hash");
	pa2m_afirmar(hash_insertar(hash, "8", "h", &anterior) != NULL,
		     "Se inserta <8,h> al Hash");

	pa2m_afirmar(hash_cantidad(hash) == 8, "El hash tiene 8 elementos");

	pa2m_afirmar(hash_con_cada_clave(hash, mostrar_claves, &anterior) == 8,
		     "Devuelve la cantidad de veces que se aplico la funcion");

	hash_destruir_todo(hash, NULL);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de creacion de hash");
	prueba_crear_hash();

	pa2m_nuevo_grupo("Pruebas de insercion");
	pruebas_insertar();
	pa2m_nuevo_grupo("Pruebas de insercion con muchos elementos");
	pruebas_insertar_muchos_elementos();
	pa2m_nuevo_grupo("Pruebas de Rehash");
	pruebas_rehash();

	pa2m_nuevo_grupo("Pruebas de Busqueda");
	pruebas_busqueda();

	pa2m_nuevo_grupo("Pruebas de eliminacion");
	pruebas_quitar();

	pa2m_nuevo_grupo("Pruebas del iterador interno");
	pruebas_con_cada_clave();

	return pa2m_mostrar_reporte();
}

#include "tp1.h"

#include "pokemon.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINEA 30
#define MODO_LECTURA "r"

struct _hospital_pkm_t {
	pokemon_t **pokemones;
	size_t cantidad_pokemon;
	size_t cantidad_entrenadores;
};

hospital_t *agregar_pokemones(hospital_t *hospital, pokemon_t *pokemon)
{
	if (pokemon == NULL) {
		return NULL;
	}

	pokemon_t **vector_nuevo = realloc(
		hospital->pokemones,
		(size_t)(hospital->cantidad_pokemon + 1) * sizeof(pokemon_t *));

	if (vector_nuevo == NULL) {
		return NULL;
	}

	hospital->pokemones = vector_nuevo;

	hospital->pokemones[hospital->cantidad_pokemon] = pokemon;
	hospital->cantidad_pokemon++;

	return hospital;
}

void ordenar_pokemones(hospital_t *hospital)
{
	pokemon_t *pokemon_aux;
	for (size_t i = 0; i < hospital->cantidad_pokemon; i++) {
		for (size_t j = 0; j < hospital->cantidad_pokemon - 1; j++) {
			if (pokemon_salud(hospital->pokemones[j]) >
			    pokemon_salud(hospital->pokemones[j + 1])) {
				pokemon_aux = hospital->pokemones[j];
				hospital->pokemones[j] =
					hospital->pokemones[j + 1];
				hospital->pokemones[j + 1] = pokemon_aux;
			}
		}
	}
}

hospital_t *hospital_crear_desde_archivo(const char *nombre_archivo)
{
	if (nombre_archivo == NULL) {
		return NULL;
	}

	FILE *archivo = fopen(nombre_archivo, MODO_LECTURA);

	if (archivo == NULL) {
		return NULL;
	}

	hospital_t *hospital = calloc(1, sizeof(hospital_t));

	if (hospital == NULL) {
		fclose(archivo);
		return NULL;
	}

	char linea[MAX_LINEA];

	size_t leidos = 0;

	while (fgets(linea, MAX_LINEA, archivo) != NULL) {
		if (!agregar_pokemones(hospital,
				       pokemon_crear_desde_string(linea))) {
			fclose(archivo);
			hospital_destruir(hospital);
			return NULL;
		}

		leidos++;
	}

	if (leidos == 0) {
		hospital_destruir(hospital);
		fclose(archivo);
		return NULL;
	}

	ordenar_pokemones(hospital);

	fclose(archivo);

	return hospital;
}

size_t hospital_cantidad_pokemones(hospital_t *hospital)
{
	if (hospital == NULL) {
		return 0;
	}

	return hospital->cantidad_pokemon;
}

size_t hospital_a_cada_pokemon(hospital_t *hospital,
			       bool (*funcion)(pokemon_t *p, void *aux),
			       void *aux)
{
	if (!funcion || !hospital) {
		return 0;
	}

	bool se_utilizo_funcion = true;

	size_t pokemones_afectados = 0;

	while ((pokemones_afectados < hospital->cantidad_pokemon) &&
	       (se_utilizo_funcion != false)) {
		se_utilizo_funcion =
			funcion(hospital->pokemones[pokemones_afectados], aux);
		pokemones_afectados++;
	}

	return pokemones_afectados;
}

int hospital_aceptar_emergencias(hospital_t *hospital,
				 pokemon_t **pokemones_ambulancia,
				 size_t cant_pokes_ambulancia)
{
	if (!hospital || !pokemones_ambulancia) {
		return ERROR;
	}

	size_t pokemones_nuevos = 0;
	size_t cant_pokemon = hospital->cantidad_pokemon;

	while (cant_pokes_ambulancia != pokemones_nuevos) {
		pokemon_t **vector_aux =
			realloc(hospital->pokemones,
				(cant_pokemon + 1) * sizeof(pokemon_t *));
		if (vector_aux == NULL) {
			return 0;
		}

		hospital->pokemones = vector_aux;

		hospital->pokemones[cant_pokemon] =
			pokemones_ambulancia[pokemones_nuevos];
		cant_pokemon++;
		pokemones_nuevos++;
	}

	hospital->cantidad_pokemon =
		hospital->cantidad_pokemon + cant_pokes_ambulancia;

	ordenar_pokemones(hospital);

	return EXITO;
}

pokemon_t *hospital_obtener_pokemon(hospital_t *hospital, size_t prioridad)
{
	if (hospital == NULL || prioridad >= hospital->cantidad_pokemon) {
		return NULL;
	}

	return hospital->pokemones[prioridad];
}

void hospital_destruir(hospital_t *hospital)
{
	if (hospital == NULL) {
		return;
	}

	for (int i = 0; i < hospital->cantidad_pokemon; i++) {
		free(hospital->pokemones[i]);
	}

	free(hospital->pokemones);
	free(hospital);
}

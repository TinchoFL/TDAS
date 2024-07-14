#include "pokemon.h"
#include <string.h>
#include <stdio.h>
#include "pokemon_privado.h"

#define FORMATO_LECTURA "%zu,%[^,],%zu,%s\n"

pokemon_t *pokemon_crear_desde_string(const char *string)
{
	if (!string) {
		return NULL;
	}

	pokemon_t *pokemon = malloc(sizeof(pokemon_t));

	if (!pokemon) {
		return NULL;
	}

	int leidos = sscanf(string, FORMATO_LECTURA, &pokemon->id,
			    pokemon->nombre, &pokemon->salud,
			    pokemon->nombre_entrenador);

	if (leidos != 4) {
		pokemon_destruir(pokemon);
		return NULL;
	}

	return pokemon;
}

pokemon_t *pokemon_copiar(pokemon_t *poke)
{
	if (!poke) {
		return NULL;
	}

	pokemon_t *poke_aux = malloc(sizeof(pokemon_t));

	if (!poke_aux) {
		return NULL;
	}

	poke_aux->id = pokemon_id(poke);
	poke_aux->salud = pokemon_salud(poke);
	strcpy(poke_aux->nombre_entrenador, pokemon_entrenador(poke));
	strcpy(poke_aux->nombre, pokemon_nombre(poke));

	return poke_aux;
}

bool pokemon_son_iguales(pokemon_t *pokemon1, pokemon_t *pokemon2)
{
	if (!pokemon1 || !pokemon2) {
		return NULL;
	}

	if (strcmp(pokemon_nombre(pokemon1), pokemon_nombre(pokemon2)) == 0 &&
	    (pokemon_salud(pokemon1) == pokemon_salud(pokemon2)) &&
	    strcmp(pokemon_entrenador(pokemon1),
		   pokemon_entrenador(pokemon2)) == 0 &&
	    (pokemon_id(pokemon1) == pokemon_id(pokemon2))) {
		return true;
	} else {
		return false;
	}
}

char *pokemon_nombre(pokemon_t *pokemon)
{
	if (!pokemon) {
		return 0;
	} else {
		return pokemon->nombre;
	}
}

char *pokemon_entrenador(pokemon_t *pokemon)
{
	if (!pokemon) {
		return 0;
	} else {
		return pokemon->nombre_entrenador;
	}
}

size_t pokemon_salud(pokemon_t *pokemon)
{
	if (!pokemon) {
		return 0;
	} else {
		return pokemon->salud;
	}
}

size_t pokemon_id(pokemon_t *pokemon)
{
	if (!pokemon) {
		return 0;
	} else {
		return pokemon->id;
	}
}

void pokemon_destruir(pokemon_t *pkm)
{
	free(pkm);
}

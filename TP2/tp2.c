#include "src/tp1.h"
#include "src/menu.h"
#include "src/hash.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINEA 200

typedef struct hash_de_hospitales {
	hash_t *hospitales;
	char hospital_activo[MAX_LINEA];
} hash_de_hospitales_t;

typedef struct hospital_ad {
	hospital_t *hospital;
	char nombre_hospital[MAX_LINEA];
	bool activo;
} hospital_ad_t;

hospital_ad_t *crear_hospital_ad(hospital_t *hospital,
				 char nombre_hospital[MAX_LINEA])
{
	if (!hospital) {
		return NULL;
	}

	hospital_ad_t *hospital_ad = calloc(1, sizeof(hospital_ad_t));
	if (!hospital_ad) {
		return NULL;
	}

	hospital_ad->hospital = hospital;
	strcpy(hospital_ad->nombre_hospital, nombre_hospital);

	return hospital_ad;
}

hash_de_hospitales_t *crear_hash_de_hospitales(size_t capacidad)
{
	hash_de_hospitales_t *hash_hospitales =
		calloc(1, sizeof(hash_de_hospitales_t));

	if (!hash_hospitales) {
		return NULL;
	}

	hash_hospitales->hospitales = hash_crear(capacidad);
	if (!hash_hospitales->hospitales) {
		return NULL;
	}

	strcpy(hash_hospitales->hospital_activo, "");

	return hash_hospitales;
}

void destruir_hospital_ad(void *_hospital_ad)
{
	hospital_ad_t *hospital_ad = (hospital_ad_t *)_hospital_ad;
	hospital_destruir(hospital_ad->hospital);
	free(hospital_ad);
}

void destruir_hash_de_hospitales(hash_de_hospitales_t *hash_hospitales)
{
	hash_destruir_todo(hash_hospitales->hospitales, destruir_hospital_ad);
	free(hash_hospitales);
}

void cargar_hospital(void *menu, void *hospitales)
{
	if (!menu || !hospitales) {
		return;
	}

	hash_de_hospitales_t *hash_hospitales =
		(hash_de_hospitales_t *)hospitales;

	char linea[MAX_LINEA];
	char nombre[MAX_LINEA];
	char numero[MAX_LINEA];
	strcpy(nombre, "");
	strcpy(numero, "");

	printf("Ingrese el nombre del archivo del Hospital y un numero para identificarlo (ej: hospital_CABA.txt 4)\n");
	printf("Ingrese Hospital: ");

	fgets(linea, MAX_LINEA, stdin);

	int leidos = sscanf(linea, "%s %s", nombre, numero);

	if (leidos != 2) {
		return;
	}

	char ruta_archivo[MAX_LINEA] = "ejemplos/";

	strcat(ruta_archivo, nombre);

	hospital_t *hospital = hospital_crear_desde_archivo(ruta_archivo);

	hospital_ad_t *hospital_ad = crear_hospital_ad(hospital, nombre);

	printf("Se pudo cargar el Hospital correctamente.\n");

	hash_t *aux = hash_insertar(hash_hospitales->hospitales, numero,
				    hospital_ad, NULL);

	if (!aux) {
		free(hospital_ad);
		return;
	}

	if (!hospital) {
		return;
	}

	return;
}

bool imprimir_hospital(const char *numero, void *_hospital_ad, void *aux)
{
	if (!numero || !_hospital_ad) {
		return false;
	}

	hospital_ad_t *hospital_ad = (hospital_ad_t *)_hospital_ad;
	if (hospital_ad->activo) {
		printf("Hospital Activo: %s	  Numero: %s\n",
		       hospital_ad->nombre_hospital, numero);
	} else {
		printf("Hospital: %s   Numero: %s\n",
		       hospital_ad->nombre_hospital, numero);
	}

	return true;
}

void mostrar_hospitales_cargados(void *menu, void *hospitales)
{
	if (!menu || !hospitales) {
		return;
	}

	hash_de_hospitales_t *hash_hospitales =
		(hash_de_hospitales_t *)hospitales;

	printf("Hospitales cargados: \n");

	if (hash_cantidad(hash_hospitales->hospitales) == 0) {
		printf("No hay Hospitales cargados la base de datos\n");
	} else {
		hash_con_cada_clave(hash_hospitales->hospitales,
				    imprimir_hospital, NULL);
	}

	return;
}

void activar_hospital(void *menu, void *hospitales)
{
	if (!menu || !hospitales) {
		return;
	}

	hash_de_hospitales_t *hash_hospitales =
		(hash_de_hospitales_t *)hospitales;

	if (hash_cantidad(hash_hospitales->hospitales) == 0) {
		printf("No hay Hospitales para activar en la base de datos\n");
		return;
	}

	if (strcmp(hash_hospitales->hospital_activo, "") != 0) {
		printf("Ya hay un hospital activo\n");
		return;
	}

	char linea[MAX_LINEA];
	char numero_pedido[MAX_LINEA];
	strcpy(numero_pedido, "");

	printf("Inserte el numero de identificacion del hospital deseado: ");

	fgets(linea, MAX_LINEA, stdin);

	int leidos = sscanf(linea, "%s", numero_pedido);

	if (leidos != 1) {
		return;
	}

	hospital_ad_t *hospital_ad = (hospital_ad_t *)hash_obtener(
		hash_hospitales->hospitales, numero_pedido);

	if (!hospital_ad) {
		return;
	}

	hospital_ad->activo = true;

	strcpy(hash_hospitales->hospital_activo, numero_pedido);

	printf("Hospital Activado correctamente\n");

	return;
}

bool imprimir_pokemon(pokemon_t *pokemon, void *aux)
{
	if (!pokemon) {
		return false;
	}

	printf("Nombre del Pokemon: %s\n", pokemon_nombre(pokemon));

	return true;
}

void mostrar_pokemones(void *menu, void *hospitales)
{
	if (!menu || !hospitales) {
		return;
	}

	hash_de_hospitales_t *hash_hospitales =
		(hash_de_hospitales_t *)hospitales;

	hospital_ad_t *hospital_activo = (hospital_ad_t *)hash_obtener(
		hash_hospitales->hospitales, hash_hospitales->hospital_activo);

	if (!hospital_activo || hospital_activo->activo == false) {
		printf("No hay ningun Hospital activo\n");
		return;
	}

	printf("Pokemones en el hospital:\n");

	if (hospital_cantidad_pokemones(hospital_activo->hospital) == 0) {
		printf("No hay ningun pokemon en el hospital\n");
	} else {
		hospital_a_cada_pokemon(hospital_activo->hospital,
					imprimir_pokemon, NULL);
	}
	return;
}

bool imprimir_datos_pokemon(pokemon_t *pokemon, void *aux)
{
	if (!pokemon) {
		return false;
	}

	printf("Nombre: %s\n", pokemon_nombre(pokemon));
	printf("ID: %lu\n", pokemon_id(pokemon));
	printf("Salud: %lu\n", pokemon_salud(pokemon));
	printf("Nombre Entrenador: %s\n", pokemon_entrenador(pokemon));

	return true;
}

void mostrar_pokemones_con_datos(void *menu, void *hospitales)
{
	if (!menu || !hospitales) {
		return;
	}

	hash_de_hospitales_t *hash_hospitales =
		(hash_de_hospitales_t *)hospitales;

	hospital_ad_t *hospital_activo = (hospital_ad_t *)hash_obtener(
		hash_hospitales->hospitales, hash_hospitales->hospital_activo);

	if (!hospital_activo || hospital_activo->activo == false) {
		printf("No hay ningun Hospital activo\n");
		return;
	}

	printf("Datos de los Pokemones en el hospital:\n");

	if (hospital_cantidad_pokemones(hospital_activo->hospital) == 0) {
		printf("No hay ningun pokemon en el hospital\n");
	} else {
		hospital_a_cada_pokemon(hospital_activo->hospital,
					imprimir_datos_pokemon, NULL);
	}

	return;
}

void destruir_hospital(void *menu, void *hospitales)
{
	if (!menu || !hospitales) {
		return;
	}

	hash_de_hospitales_t *hash_hospitales =
		(hash_de_hospitales_t *)hospitales;

	hospital_ad_t *hospital_activo = hash_quitar(
		hash_hospitales->hospitales, hash_hospitales->hospital_activo);

	if (!hospital_activo || !hospital_activo->activo) {
		printf("No hay ningun hospital activado\n");
		return;
	}

	strcpy(hash_hospitales->hospital_activo, "");
	hospital_destruir(hospital_activo->hospital);
	free(hospital_activo);
	printf("Se elimino el Hospital\n");

	return;
}

void imprimir_txt(FILE *archivo)
{
	if (!archivo) {
		return;
	}

	char c = (char)fgetc(archivo);
	while (c != EOF) {
		printf("%c", c);
		c = (char)fgetc(archivo);
	}
}

void mostrar_ascii(void *menu, void *_ascii)
{
	if (!_ascii) {
		return;
	}
	FILE *ascii = (FILE *)_ascii;
	imprimir_txt(ascii);
	return;
}

void mostrar_asciis_intro()
{
	FILE *intro = fopen("ASCIIs/hospitalpokemonascii.txt", "r");
	if (!intro) {
		return;
	}

	mostrar_ascii(NULL, intro);
	fclose(intro);
}

void mostrar_asciis_gengar()
{
	FILE *gengar = fopen("ASCIIs/gengar.txt", "r");
	if (!gengar) {
		return;
	}

	mostrar_ascii(NULL, gengar);
	fclose(gengar);
}

void cerrar_programa(void *menu, void *hospitales)
{
	if (!menu || !hospitales) {
		return;
	}

	hash_de_hospitales_t *hash_hospitales =
		(hash_de_hospitales_t *)hospitales;

	destruir_hash_de_hospitales(hash_hospitales);
	menu_destruir((menu_t *)menu);

	printf("\n Cerrando el programa, gracias por hacer uso del programa. \n");
	mostrar_asciis_gengar();

	return;
}

void mostrar_mensaje_inicial()
{
	printf("\nBienvenido al circulo de Hospitales de Pokemones, en este programa, podra ver, agregar o manipular los Hospitales de su region, para comenzar, puede consultar los comandos disponibles del programa presionando la tecla 'H'\n");
}

void mostrar_mensaje_ayuda()
{
	printf("- S (salir/exit): Salir del programa.\n"
	       "- H (ayuda/help): Muestra un menú de ayuda con los comandos disponibles.\n"
	       "- C (cargar): Carga el hospital pasado con su numero de identificación.\n"
	       "- E (estado): Muestra un listado con los hospitales cargados (y el activo, si hay alguno).\n"
	       "- A (activar): Activa un Hospital a traves de su numero de identificación, para poder aplicar el resto de funciones en el mismo.\n"
	       "- M (mostrar): Muestra un listado con los nombres de todos los pokemones en el hospital.\n"
	       "- L (listar): Muestra un listado  de todos los pokemones en el hospital con sus atributos.\n"
	       "- D (destruir): Destruye el hospital activo.\n");
}

void mostrar_mensaje_comando_incorrecto()
{
	printf("El comando pedido, no existe, ingrese 'H' para ver todos los comandos disponibles");
}

void mostrar_mensaje_error_generico()
{
	printf("Hubo un error en esa accion y no se pudo efectuar");
}

menu_t *agregar_comandos(menu_t *menu, hash_de_hospitales_t *hash_hospitales)
{
	menu_agregar_comando(
		menu, "h", "Ayuda",
		"Muestra ayuda al usuario mediante un menu de opciones disponibles.",
		mostrar_mensaje_ayuda, hash_hospitales);
	menu_agregar_comando(
		menu, "s", "Salir",
		"Cierra el programa liberando toda la memoria utilizada.",
		cerrar_programa, hash_hospitales);
	menu_agregar_comando(menu, "c", "Cargar",
			     "Carga un Hospital desde un archivo.",
			     cargar_hospital, hash_hospitales);
	menu_agregar_comando(
		menu, "e", "Mostrar estado de los Hospitales",
		"Muestra una lista con los hospitales dentro de la base de datos.",
		mostrar_hospitales_cargados, hash_hospitales);
	menu_agregar_comando(
		menu, "a", "Activar Hospital",
		"Activa un Hospital pasandole un numero que lo identifica.",
		activar_hospital, hash_hospitales);
	menu_agregar_comando(
		menu, "m", "Mostrar Pokemones",
		"Muestra una lista de los pokemones que estan siendo atendidos en el Hospital.",
		mostrar_pokemones, hash_hospitales);
	menu_agregar_comando(
		menu, "l", "Datos Pokemones",
		"Muestra una lista de los pokemones que estan siendo atendidos en el Hospital con sus respectivos datos.",
		mostrar_pokemones_con_datos, hash_hospitales);
	menu_agregar_comando(menu, "d", "Destruir Hospital",
			     "Destruye el Hospital activo.", destruir_hospital,
			     hash_hospitales);

	return menu;
}

menu_t *crear_menu_hospital(hash_de_hospitales_t *hash_hospitales)
{
	menu_t *menu = menu_crear();
	if (!menu) {
		return NULL;
	}

	agregar_comandos(menu, hash_hospitales);

	if (menu_contiene(menu, "s") == false) {
		return NULL;
	}

	return menu;
}

char *asignar_comando(char *comando)
{
	if (!comando) {
		return NULL;
	}

	if (strcmp(comando, "ayuda") == 0 || strcmp(comando, "help") == 0) {
		strcpy(comando, "h");
	} else if (strcmp(comando, "salir") == 0 ||
		   strcmp(comando, "exit") == 0) {
		strcpy(comando, "s");
	} else if (strcmp(comando, "cargar") == 0 ||
		   strcmp(comando, "load") == 0) {
		strcpy(comando, "c");
	} else if (strcmp(comando, "estado") == 0 ||
		   strcmp(comando, "status") == 0) {
		strcpy(comando, "e");
	} else if (strcmp(comando, "activar") == 0 ||
		   strcmp(comando, "activate") == 0) {
		strcpy(comando, "a");
	} else if (strcmp(comando, "mostrar") == 0 ||
		   strcmp(comando, "pokemones") == 0 ||
		   strcmp(comando, "show") == 0) {
		strcpy(comando, "m");
	} else if (strcmp(comando, "listar") == 0 ||
		   strcmp(comando, "atributos") == 0 ||
		   strcmp(comando, "stats") == 0) {
		strcpy(comando, "l");
	} else if (strcmp(comando, "destruir") == 0 ||
		   strcmp(comando, "borrar") == 0 ||
		   strcmp(comando, "eliminar") == 0 ||
		   strcmp(comando, "destroy") == 0) {
		strcpy(comando, "d");
	}

	return comando;
}

char *pedir_instruccion(char *linea)
{
	printf("ingresar comando: \n");
	char *opcion = fgets(linea, MAX_LINEA, stdin);
	opcion[strcspn(opcion, "\n")] = 0;
	for (int i = 0; linea[i]; i++) {
		linea[i] = (char)tolower(linea[i]);
	}
	return asignar_comando(linea);
}

int main()
{
	hash_de_hospitales_t *hash_hospitales = crear_hash_de_hospitales(5);
	if (!hash_hospitales) {
		mostrar_mensaje_error_generico();
		return -1;
	}

	menu_t *menu = crear_menu_hospital(hash_hospitales);
	if (!menu) {
		mostrar_mensaje_error_generico();
		destruir_hash_de_hospitales(hash_hospitales);
		return -1;
	}

	mostrar_asciis_intro();

	mostrar_mensaje_inicial();

	bool termino = false;

	char linea[MAX_LINEA];

	while (!termino) {
		char *opcion = pedir_instruccion(linea);
		if (strcmp(opcion, "s") == 0) {
			termino = true;
		}
		if (!menu_ejecutar_comando(menu, linea)) {
			mostrar_mensaje_comando_incorrecto();
		}
	}

	return 0;
}

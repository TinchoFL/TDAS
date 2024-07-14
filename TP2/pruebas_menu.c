#include "pa2m.h"
#include "src/hash.h"
#include "src/menu.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void mostrar_mensaje_ayuda()
{
	printf("Se ejecuta la funcion ayuda :)\n");
}

void mostrar_mensaje_cargar()
{
	printf("Se ejecuta la funcion cargar :)\n");
}

void prueba_crear_menu()
{
	menu_t *menu = menu_crear();

	pa2m_afirmar(menu != NULL, "Se puede crear el Menu");
	pa2m_afirmar(menu_vacio(menu) == true, "El Menu esta vacio");

	menu_destruir(menu);
}

void prueba_agregar_menu()
{
	menu_t *menu = menu_crear();

	menu_agregar_comando(
		menu, "Salir", "s",
		"Cierra el programa liberando toda la memoria utilizada.",
		mostrar_mensaje_ayuda, NULL);
	pa2m_afirmar(menu_contiene(menu, "Salir") == true,
		     "El Menu contiene el comando pedido");

	menu_agregar_comando(menu, "cargar", "c", "Carga algo.",
			     mostrar_mensaje_cargar, NULL);
	pa2m_afirmar(menu_contiene(menu, "cargar") == true,
		     "El Menu contiene el comando pedido");

	menu_destruir(menu);
}

void prueba_eliminar_menu()
{
	menu_t *menu = menu_crear();

	menu_agregar_comando(
		menu, "Salir", "s",
		"Cierra el programa liberando toda la memoria utilizada.",
		mostrar_mensaje_cargar, NULL);
	pa2m_afirmar(menu_contiene(menu, "Salir") == true,
		     "El Menu contiene el comando pedido");

	menu_agregar_comando(menu, "cargar", "c", "Carga algo.",
			     mostrar_mensaje_cargar, NULL);
	pa2m_afirmar(menu_contiene(menu, "cargar") == true,
		     "El Menu contiene el comando pedido");

	menu_eliminar_comando(menu, "Salir");
	pa2m_afirmar(menu_contiene(menu, "Salir") == false,
		     "El comando eliminado ya no se encuentra en el Menu");

	menu_agregar_comando(
		menu, "Salir", "s",
		"Cierra el programa liberando toda la memoria utilizada.",
		mostrar_mensaje_cargar, NULL);
	pa2m_afirmar(
		menu_contiene(menu, "Salir") == true,
		"Se puede volver a agregar el comando previamente eliminado");

	menu_destruir(menu);
}

void pruebas_ejecucion()
{
	menu_t *menu = menu_crear();

	menu_agregar_comando(menu, "ayuda", "a", "muestra un mensaje.",
			     mostrar_mensaje_ayuda, NULL);
	pa2m_afirmar(menu_contiene(menu, "ayuda") == true,
		     "Se agrega comando 'ayuda'");
	menu_agregar_comando(menu, "cargar", "c", "Carga algo.",
			     mostrar_mensaje_cargar, NULL);
	pa2m_afirmar(menu_contiene(menu, "cargar") == true,
		     "Se agrega comando 'cargar'");

	menu_ejecutar_comando(menu, "ayuda");
	menu_ejecutar_comando(menu, "cargar");

	menu_destruir(menu);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de creacion de Menu");
	prueba_crear_menu();
	pa2m_nuevo_grupo("Pruebas de insertar y eliminar en Menu");
	prueba_agregar_menu();
	prueba_eliminar_menu();
	pa2m_nuevo_grupo("Pruebas de ejecucion de comandos");
	pruebas_ejecucion();

	return pa2m_mostrar_reporte();
}
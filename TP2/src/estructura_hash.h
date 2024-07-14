#ifndef ESTRUCTURA_HASH_H_
#define ESTRUCTURA_HASH_H_

#include "hash.h"

typedef struct nodo {
	char *clave;
	void *valor;
	struct nodo *siguiente;
} nodo_t;

struct hash {
	struct nodo **vector;
	size_t capacidad;
	size_t cantidad;
};

#endif // ABB_ESTRUCTURA_PRIVADA_H_
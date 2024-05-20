#ifndef UTILS_CLIENT_H_
#define UTILS_CLIENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<commons/string.h>
#include<commons/log.h>
#include <commons/config.h>

typedef enum {
    MENSAJE,
    PAQUETE,
    PCB,
    PC,
    PCB_FROM_MEM,
    WRITE,
    INSTRUCTION
} op_code;

typedef struct {
    u_int32_t size;
    u_int32_t offset;
    void *stream;
} t_buffer;

typedef struct {
    op_code codigo_operacion;
    t_buffer *buffer;
} t_paquete;

/**
* @fn    crear_conexion
* @brief Crea una conexión hacia la ip y puerto indicados
*/
int crear_conexion(char *ip, char *puerto);

t_paquete *crear_paquete(op_code op);

void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio);

void enviar_paquete(t_paquete *paquete, int socket_cliente);

void liberar_conexion(int socket_cliente);

void eliminar_paquete(t_paquete *paquete);

int conexion_by_config(t_config *config, char *ip_config, char *puerto_config);

void enviar_mensaje(char *mensaje, int socket_cliente);

#endif
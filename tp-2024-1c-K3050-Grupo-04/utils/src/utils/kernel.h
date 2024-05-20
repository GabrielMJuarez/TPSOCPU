// kernel.h tiene las estructuras y funciones relacionadas a la comunicación entre módulos.
#ifndef UTILS_KERNEL_H_
#define UTILS_KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <commons/log.h>
#include "client.h"

typedef struct {
    u_int32_t dato; // TODO: TBD, aqui deberia esta la lista de instrucciones a ejecutar
} t_register;

typedef struct {
    u_int32_t pid;
    u_int32_t pc;
    u_int32_t quantum;
    t_register *reg;
    u_int32_t instruccionesLength;
    char** instrucciones; //instrucciones a ejecutar, ej: MOV, RESIZE
} t_pcb;

/**
* @fn    serializar_pcb
* @brief Serializa la estructura pcb para poder transportarla a otros servicios.
*/
void serializar_pcb(t_pcb* pcb, t_buffer* buffer);

/**
* @fn    deserializar_pcb
* @brief Deserializa la estructura pcb para identificarla desde otros servicios.
*/
t_pcb* deserializar_pcb(void* stream);

t_pcb* nuevo_pcb(int pid, int pc, int quantum, char** instrucciones, int instruccionesLength);

//void eliminar_pcb(t_pcb *pcb);

#endif
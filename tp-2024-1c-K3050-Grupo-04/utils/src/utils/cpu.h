// kernel.h tiene las estructuras y funciones relacionadas a la comunicación entre módulos.
#ifndef UTILS_CPU_H_
#define UTILS_CPU_H_

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
    uint32_t PC;
    uint8_t AX;
    uint8_t BX;
    uint8_t CX;
    uint8_t EAX;
    uint8_t EBX;
    uint8_t ECX;
    uint8_t EDX;
    uint32_t SI;
    uint32_t DI;
} t_reg_cpu;

t_reg_cpu* nuevo_reg(uint8_t pc);

void serializar_reg(t_reg_cpu* reg, t_buffer* buffer);

t_reg_cpu* deserializar_reg(void* stream);

void eliminar_reg(t_reg_cpu *reg);

#endif
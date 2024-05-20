#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>
#include<commons/string.h>
#include <commons/config.h>
#include <utils/client.h>
#include "cpu.h"

t_reg_cpu* nuevo_reg(uint8_t pc) { // TODO: Una vez bien definida la struct. Pasar por param las props del pcb.
    t_reg_cpu *reg_cpu = malloc(sizeof(t_reg_cpu));

    reg_cpu->AX = 0;
    reg_cpu->BX = 0;
    reg_cpu->CX = 0;
    reg_cpu->DI = 0;
    reg_cpu->EAX = 0;
    reg_cpu->EBX = 0;
    reg_cpu->ECX = 0;
    reg_cpu->EDX = 0;
    reg_cpu->PC = pc;
    reg_cpu->SI = 0;    

    return reg_cpu;
}

/*void serializar_pcb(t_pcb *pcb, uint8_t *buffer, int *offset) {
    memcpy(buffer + *offset, &(pcb->pid), sizeof(int));
    *offset += sizeof(int);

    memcpy(buffer + *offset, &(pcb->pc), sizeof(int));
    *offset += sizeof(int);

    memcpy(buffer + *offset, &(pcb->quantum), sizeof(int));
    *offset += sizeof(int);

    if (pcb->reg != NULL) {
        memcpy(buffer + *offset, &(pcb->reg->dato), sizeof(int));
        *offset += sizeof(int);
    }
}

void deserializar_pcb(uint8_t *buffer, t_pcb *pcb, int *offset) {
    memcpy(&(pcb->pid), buffer + *offset, sizeof(int));
    *offset += sizeof(int);

    memcpy(&(pcb->pc), buffer + *offset, sizeof(int));
    *offset += sizeof(int);

    memcpy(&(pcb->quantum), buffer + *offset, sizeof(int));
    *offset += sizeof(int);

    pcb->reg = malloc(sizeof(t_register));
    if (pcb->reg == NULL) {
        return;
    }

    memcpy(&(pcb->reg->dato), buffer + *offset, sizeof(int));
    *offset += sizeof(int);
}*/

void serializar_reg(t_reg_cpu* reg, t_buffer* buffer){

    buffer->size = sizeof(u_int32_t) * 3 + sizeof(u_int8_t) * 7;

    buffer->offset = 0;
    buffer->stream = malloc(buffer->size);

    memcpy(buffer->stream + buffer->offset, &(reg->PC), sizeof(u_int32_t));
    buffer->offset += sizeof(u_int32_t);

    memcpy(buffer->stream + buffer->offset, &(reg->AX), sizeof(u_int8_t));
    buffer->offset += sizeof(u_int8_t);

    memcpy(buffer->stream + buffer->offset, &(reg->BX), sizeof(u_int8_t));
    buffer->offset += sizeof(u_int8_t);

    memcpy(buffer->stream + buffer->offset, &(reg->CX), sizeof(u_int8_t));
    buffer->offset += sizeof(u_int8_t);

    memcpy(buffer->stream + buffer->offset, &(reg->DI), sizeof(u_int8_t));
    buffer->offset += sizeof(u_int8_t);

    memcpy(buffer->stream + buffer->offset, &(reg->EAX), sizeof(u_int8_t));
    buffer->offset += sizeof(u_int8_t);
    
    memcpy(buffer->stream + buffer->offset, &(reg->ECX), sizeof(u_int8_t));
    buffer->offset += sizeof(u_int8_t);

    memcpy(buffer->stream + buffer->offset, &(reg->EDX), sizeof(u_int8_t));
    buffer->offset += sizeof(u_int8_t);

    memcpy(buffer->stream + buffer->offset, &(reg->SI), sizeof(u_int32_t));
    buffer->offset += sizeof(u_int32_t);

    memcpy(buffer->stream + buffer->offset, &(reg->DI), sizeof(u_int32_t));
    buffer->offset += sizeof(u_int32_t);
}

t_reg_cpu* deserializar_reg(void* stream) {
    t_reg_cpu* reg = nuevo_reg(0);

    int offset = 0;
    memcpy(&(reg->PC), stream + offset, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);

    memcpy(&(reg->AX), stream + offset, sizeof(u_int8_t));
    offset += sizeof(u_int8_t);
    
    memcpy(&(reg->BX), stream + offset, sizeof(u_int8_t));
    offset += sizeof(u_int8_t);

    memcpy(&(reg->CX), stream + offset, sizeof(u_int8_t));
    offset += sizeof(u_int8_t);

    memcpy(&(reg->DI), stream + offset, sizeof(u_int8_t));
    offset += sizeof(u_int8_t);

    memcpy(&(reg->EAX), stream + offset, sizeof(u_int8_t));
    offset += sizeof(u_int8_t);

    memcpy(&(reg->ECX), stream + offset, sizeof(u_int8_t));
    offset += sizeof(u_int8_t);

    memcpy(&(reg->EDX), stream + offset, sizeof(u_int8_t));
    offset += sizeof(u_int8_t);

    memcpy(&(reg->SI), stream + offset, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);

    memcpy(&(reg->DI), stream + offset, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);

    return reg;
}

void eliminar_reg(t_reg_cpu *reg) {
    free(reg);
}
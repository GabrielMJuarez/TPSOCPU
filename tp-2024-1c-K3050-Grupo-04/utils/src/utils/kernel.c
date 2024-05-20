#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>
#include<commons/string.h>
#include <commons/config.h>
#include <utils/client.h>

t_pcb* nuevo_pcb(int pid, int pc, int quantum, char** instrucciones, int instruccionesLength) {
    t_pcb* pcb = malloc(sizeof(t_pcb));
    if (pcb == NULL) {
        return NULL; // Error: No se pudo asignar memoria para la estructura pcb
    }

    pcb->pid = pid;
    pcb->pc = pc;
    pcb->quantum = quantum;

    // Asigna el arreglo de instrucciones
    pcb->instrucciones = malloc(instruccionesLength * sizeof(char*));
    if (pcb->instrucciones == NULL) {
        free(pcb); // Error: No se pudo asignar memoria para el arreglo de instrucciones
        return NULL;
    }

    // Copia las instrucciones proporcionadas al arreglo
    for (int i = 0; i < instruccionesLength; i++) {
        pcb->instrucciones[i] = strdup(instrucciones[i]);
        if (pcb->instrucciones[i] == NULL) {
            // Error: No se pudo asignar memoria para una instrucción
            for (int j = 0; j < i; j++) {
                free(pcb->instrucciones[j]);
            }
            free(pcb->instrucciones);
            free(pcb);
            return NULL;
        }
    }

    pcb->instruccionesLength = instruccionesLength;

    // Serializa el PCB para obtener un buffer
    t_buffer buffer;
    serializar_pcb(pcb, &buffer);

    // Deserializa el buffer para obtener un nuevo PCB
    t_pcb* new_pcb = deserializar_pcb(buffer.stream);
    free(buffer.stream); // Liberar la memoria utilizada por el buffer

    return new_pcb;
}

/*
t_pcb *nuevo_pcb(int pid) { // TODO: Una vez bien definida la struct. Pasar por param las props del pcb.
    t_pcb *pcb = malloc(sizeof(t_pcb));
    pcb->pid = pid;
    pcb->pc = 0;
    pcb->quantum = 0;

    t_register *reg = malloc(sizeof(t_register));
    reg->dato = 0;

    pcb->reg = reg;
    return pcb;
}
*/
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
/*
void eliminar_pcb(t_pcb *pcb) {
    // Primero, liberamos la memoria asignada para las instrucciones en reg, si es necesario
        if (pcb != NULL) {
        if (pcb->reg != NULL) {
            if (pcb->reg->instrucciones != NULL) {
        for (int i = 0; i < pcb->reg->instruccionesLength; i++) {
            free(pcb->reg->instrucciones[i]);
        }
        free(pcb->reg->instrucciones);
    }
    // Luego, liberamos la memoria asignada para el campo reg
    free(pcb->reg);
    // Finalmente, liberamos la memoria asignada para la estructura pcb
    free(pcb);
}
*/
void serializar_pcb(t_pcb* pcb, t_buffer* buffer) {
    buffer->offset = 0;

    // Calcula el tamaño total necesario para serializar el pcb
    size_t size = sizeof(u_int32_t) * 4; // Para pid, pc, quantum y instruccionesLength
    if (pcb->instrucciones != NULL) {
        for (int i = 0; i < pcb->instruccionesLength; i++) {
            size += sizeof(u_int32_t); // Para la longitud de cada cadena
            size += strlen(pcb->instrucciones[i]) + 1; // Para la cadena y su terminador nulo
        }
    }

    buffer->size = size;
    buffer->stream = malloc(size);

    // Serializa pid, pc, quantum y instruccionesLength
    memcpy(buffer->stream + buffer->offset, &(pcb->pid), sizeof(u_int32_t));
    buffer->offset += sizeof(u_int32_t);

    memcpy(buffer->stream + buffer->offset, &(pcb->pc), sizeof(u_int32_t));
    buffer->offset += sizeof(u_int32_t);

    memcpy(buffer->stream + buffer->offset, &(pcb->quantum), sizeof(u_int32_t));
    buffer->offset += sizeof(u_int32_t);

    memcpy(buffer->stream + buffer->offset, &(pcb->instruccionesLength), sizeof(u_int32_t));
    buffer->offset += sizeof(u_int32_t);

    // Serializa instrucciones si están presentes
    if (pcb->instrucciones != NULL) {
        for (int i = 0; i < pcb->instruccionesLength; i++) {
            // Serializa la longitud de la cadena
            u_int32_t instr_length = strlen(pcb->instrucciones[i]) + 1;
            memcpy(buffer->stream + buffer->offset, &instr_length, sizeof(u_int32_t));
            buffer->offset += sizeof(u_int32_t);

            // Serializa la cadena
            memcpy(buffer->stream + buffer->offset, pcb->instrucciones[i], instr_length);
            buffer->offset += instr_length;
        }
    }
}



t_pcb* deserializar_pcb(void* stream) {
    t_pcb* pcb = malloc(sizeof(t_pcb));
    if (pcb == NULL) {
        return NULL; // Error: No se pudo asignar memoria para la estructura pcb
    }

    int offset = 0;

    // Copia los valores del stream al PCB
    memcpy(&(pcb->pid), stream + offset, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);

    memcpy(&(pcb->pc), stream + offset, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);
    
    memcpy(&(pcb->quantum), stream + offset, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);

    memcpy(&(pcb->instruccionesLength), stream + offset, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);

    // Verifica si hay instrucciones para deserializar
    if (pcb->instruccionesLength > 0) {
        pcb->instrucciones = malloc(pcb->instruccionesLength * sizeof(char*));
        if (pcb->instrucciones == NULL) {
            free(pcb); // Error: No se pudo asignar memoria para las instrucciones
            return NULL;
        }

        // Copia cada instrucción del stream a t_pcb
        for (u_int32_t i = 0; i < pcb->instruccionesLength; i++) {
            u_int32_t instr_length;
            memcpy(&instr_length, stream + offset, sizeof(u_int32_t));
            offset += sizeof(u_int32_t);

            pcb->instrucciones[i] = malloc(instr_length);
            if (pcb->instrucciones[i] == NULL) {
                // Error: No se pudo asignar memoria para una instrucción
                for (u_int32_t j = 0; j < i; j++) {
                    free(pcb->instrucciones[j]);
                }
                free(pcb->instrucciones);
                free(pcb);
                return NULL;
            }
            
            memcpy(pcb->instrucciones[i], stream + offset, instr_length);
            offset += instr_length;
        }
    } else {
        pcb->instrucciones = NULL;
    }

    return pcb;
}

/*
t_pcb* deserializar_pcb(void* stream) {
    t_pcb* pcb = nuevo_pcb(0);

    int offset = 0;
    memcpy(&(pcb->pid), stream + offset, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);

    memcpy(&(pcb->pc), stream + offset, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);
    
    memcpy(&(pcb->quantum), stream + offset, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);

    pcb->reg = malloc(sizeof(t_register));
    if (pcb->reg == NULL) {
        return;
    }

    memcpy(&(pcb->reg->dato), stream + offset, sizeof(u_int32_t));

    return pcb;
}
*/
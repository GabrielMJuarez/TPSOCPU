#include "client.h"


void *serializar_paquete(t_paquete *paquete, int bytes) {
    void *magic = malloc(bytes);
    int desplazamiento = 0;

    memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
    desplazamiento += sizeof(int);
    memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
    desplazamiento += sizeof(int);
    memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
    desplazamiento += paquete->buffer->size;

    return magic;
}

int crear_conexion(char *ip, char *puerto) {
    struct addrinfo hints;
    struct addrinfo *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &server_info);

    int socket_cliente = 0;

    struct addrinfo *p;
    for (p = server_info; p != NULL; p = p->ai_next) {
        if ((socket_cliente = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (connect(socket_cliente, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_cliente);
            continue;
        }

        break;
    }

    if (p == NULL) {
        return -1;
    }

    freeaddrinfo(server_info);

    return socket_cliente;
}

int conexion_by_config(t_config *config, char *ip_config, char *puerto_config) {
    char *ip = config_get_string_value(config, ip_config);
    char *puerto = config_get_string_value(config, puerto_config);
    return crear_conexion(ip, puerto);
}

void enviar_mensaje(char *mensaje, int socket_cliente) {
    t_paquete *paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = MENSAJE;
    paquete->buffer = malloc(sizeof(t_buffer));
    paquete->buffer->size = strlen(mensaje) + 1;
    paquete->buffer->stream = malloc(paquete->buffer->size);
    memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

    int bytes = paquete->buffer->size + 2 * sizeof(int);

    void *a_enviar = serializar_paquete(paquete, bytes);

    send(socket_cliente, a_enviar, bytes, 0);

    free(a_enviar);
    eliminar_paquete(paquete);
}


void crear_buffer(t_paquete *paquete) {
    paquete->buffer = malloc(sizeof(t_buffer));
    paquete->buffer->size = 0;
    paquete->buffer->stream = NULL;
}

t_paquete *crear_paquete(op_code op) {
    t_paquete *paquete = malloc(sizeof(t_paquete));
    paquete->codigo_operacion = op;
    crear_buffer(paquete);
    return paquete;
}

void agregar_a_paquete(t_paquete *paquete, void *valor, int tamanio) {
    paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

    memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
    memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

    paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete *paquete, int socket_cliente) {
    int bytes = paquete->buffer->size + 2 * sizeof(int);
    void *a_enviar = serializar_paquete(paquete, bytes);

    send(socket_cliente, a_enviar, bytes, 0);

    free(a_enviar);
}

void eliminar_paquete(t_paquete *paquete) {
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);
}

void liberar_conexion(int socket_cliente) {
    close(socket_cliente);
}
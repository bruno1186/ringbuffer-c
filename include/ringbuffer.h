#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Ring buffer (fila circular) de bytes para streaming de eventos.
 * Semantica FIFO. Nao faz alocacoes durante push/pop.
 * Nao e thread-safe por si so: sincronizacao fica a cargo do chamador.
 */
typedef struct {
    unsigned char *data; /* buffer de armazenamento           */
    size_t capacity;     /* numero maximo de bytes            */
    size_t head;         /* indice de leitura                 */
    size_t tail;         /* indice de escrita                 */
    size_t size;         /* bytes atualmente armazenados      */
} ringbuffer_t;

/* Cria um ring buffer com a capacidade dada. Retorna NULL em falha. */
ringbuffer_t *rb_create(size_t capacity);

/* Libera um ring buffer criado por rb_create. Aceita NULL. */
void rb_destroy(ringbuffer_t *rb);

/* Numero de bytes armazenados. */
size_t rb_size(const ringbuffer_t *rb);

/* Capacidade total em bytes. */
size_t rb_capacity(const ringbuffer_t *rb);

/* true se nao ha bytes armazenados. */
bool rb_is_empty(const ringbuffer_t *rb);

/* true se o buffer esta cheio. */
bool rb_is_full(const ringbuffer_t *rb);

/*
 * Escreve len bytes. Retorna o numero de bytes efetivamente escritos
 * (pode ser menor que len se o buffer encher).
 */
size_t rb_write(ringbuffer_t *rb, const unsigned char *src, size_t len);

/*
 * Le ate len bytes para dst. Retorna o numero de bytes lidos
 * (pode ser menor que len se houver menos dados disponiveis).
 */
size_t rb_read(ringbuffer_t *rb, unsigned char *dst, size_t len);

/* Descarta todo o conteudo, mantendo a capacidade. */
void rb_clear(ringbuffer_t *rb);

#endif /* RINGBUFFER_H */

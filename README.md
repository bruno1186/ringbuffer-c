# ringbuffer-c

![CI](https://github.com/bruno1186/ringbuffer-c/actions/workflows/ci.yml/badge.svg)

Biblioteca de **ring buffer** (fila circular) de bytes em **C11**, pensada para
**streaming de eventos de alta vazao** (telemetria, filas de mensagens, buffers de
I/O). API pequena, sem alocacoes no caminho quente e com testes proprios.

> Casos de uso de referencia: **fintech** (fila de eventos/transacoes) e **telemetria**.

## Caracteristicas

- **C11 portavel**, compila com **gcc** e **clang** sob `-Wall -Wextra -Werror`.
- **Sem alocacao** em `rb_write` / `rb_read` (apenas em create/destroy).
- **Semantica FIFO** com wraparound correto.
- Testado tambem com **AddressSanitizer / UBSan** no CI.

## API

```c
ringbuffer_t *rb_create(size_t capacity);
void          rb_destroy(ringbuffer_t *rb);
size_t        rb_write(ringbuffer_t *rb, const unsigned char *src, size_t len);
size_t        rb_read(ringbuffer_t *rb, unsigned char *dst, size_t len);
size_t        rb_size(const ringbuffer_t *rb);
size_t        rb_capacity(const ringbuffer_t *rb);
bool          rb_is_empty(const ringbuffer_t *rb);
bool          rb_is_full(const ringbuffer_t *rb);
void          rb_clear(ringbuffer_t *rb);
```

`rb_write` / `rb_read` retornam o numero de bytes efetivamente processados, o que
permite ao chamador tratar buffer cheio/vazio sem excecoes.

## Como compilar e testar

```bash
make test                 # compila e roda os testes
make test CC=clang        # usando clang
make clean
```

## Estrutura

```
include/ringbuffer.h      # API publica
src/ringbuffer.c          # implementacao
tests/test_ringbuffer.c   # suite de testes (retorna != 0 em falha)
Makefile                  # build + test
```

## Stack

C11 | gcc / clang | Make | AddressSanitizer + UBSan | GitHub Actions

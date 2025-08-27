/*
 * Exercício 2 - Leitura Básica de Arquivo
 * 
 * OBJETIVO: Implementar leitura de arquivo usando syscalls
 * 
 * 1. Compile:  gcc src/ex2_leitura.c -Wall -g -o ex2_leitura
 * 2. Execute:  ./ex2_leitura
 * 3. Observe:  strace -e openat,read,close ./ex2_leitura
 */

#include <fcntl.h>    // open(), flags O_*
#include <unistd.h>   // read(), close()
#include <stdio.h>    // printf(), perror()
#include <errno.h>    // errno

#define BUFFER_SIZE 128

int main(void) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_lidos;
    int fd;
    
    printf("=== Exercício 2: Leitura de Arquivo ===\n\n");
    
    // Abrir arquivo para leitura
    fd = open("dados/teste1.txt", O_RDONLY);
    if (fd < 0) {
        perror("Erro ao abrir arquivo");
        return 1;
    }
    
    printf("Arquivo aberto! File descriptor: %d\n", fd);
    
    // Ler até BUFFER_SIZE - 1 para sobrar 1 byte pro terminador
    bytes_lidos = read(fd, buffer, BUFFER_SIZE - 1);
    if (bytes_lidos < 0) {
        perror("Erro na leitura");
        close(fd);
        return 1;
    }
    
    // Terminador nulo para tratar como string
    buffer[bytes_lidos] = '\0';
    
    // Exibir resultados
    printf("Bytes lidos: %zd\n", bytes_lidos);
    printf("Conteúdo:\n%s\n", buffer);
    
    // Fechar o arquivo
    if (close(fd) < 0) {
        perror("Erro ao fechar arquivo");
        return 1;
    }
    
    printf("Arquivo fechado!\n\n");
    printf("Execute: strace -e openat,read,close ./ex2_leitura\n");
    return 0;
}

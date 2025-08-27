/*
 * Exercício 4 - Cópia de Arquivo
 * 
 * OBJETIVO: Implementar programa de cópia usando syscalls
 * 
 * 1. Compile:  gcc src/ex4_copia.c -Wall -g -o ex4_copia
 * 2. Execute:  ./ex4_copia
 * 3. Verifique: diff dados/origem.txt dados/destino.txt
 * 4. Analise:  strace -e openat,read,write,close ./ex4_copia
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#define BUFFER_SIZE 256

int main(void) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_lidos;
    int fd_origem, fd_destino;
    long total_bytes = 0;
    int total_operacoes = 0;
    
    clock_t inicio = clock();
    
    printf("=== Exercício 4: Cópia de Arquivo ===\n");
    printf("Copiando: dados/origem.txt -> dados/destino.txt\n\n");
    
    // 1) Abrir origem
    fd_origem = open("dados/origem.txt", O_RDONLY);
    if (fd_origem < 0) {
        perror("Erro ao abrir origem");
        return 1;
    }
    
    // 2) Criar/Truncar destino: rw- r-- r--
    fd_destino = open("dados/destino.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_destino < 0) {
        perror("Erro ao criar destino");
        close(fd_origem);
        return 1;
    }
    
    // 3) Loop de cópia
    while ((bytes_lidos = read(fd_origem, buffer, BUFFER_SIZE)) > 0) {
        total_operacoes++;
        
        // 4) Escrever (tratar escrita parcial)
        ssize_t escritos = 0;
        while (escritos < bytes_lidos) {
            ssize_t w = write(fd_destino, buffer + escritos, (size_t)(bytes_lidos - escritos));
            if (w < 0) {
                perror("Erro na escrita");
                close(fd_origem);
                close(fd_destino);
                return 1;
            }
            escritos += w;
        }
        
        // 6) Atualizar contador
        total_bytes += escritos;
        
        if (total_operacoes % 20 == 0) {
            printf("Progresso: %ld bytes...\n", total_bytes);
        }
    }
    
    // 7) Verificar erro de leitura
    if (bytes_lidos < 0) {
        perror("Erro na leitura");
        // Mesmo com erro de leitura, tente fechar FDs
        close(fd_origem);
        close(fd_destino);
        return 1;
    }
    
    // 8) Fechar arquivos
    if (close(fd_origem) < 0) {
        perror("Erro ao fechar origem");
        // segue tentando fechar o outro
    }
    if (close(fd_destino) < 0) {
        perror("Erro ao fechar destino");
    }
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("\n=== Estatísticas ===\n");
    printf("Bytes copiados: %ld\n", total_bytes);
    printf("Operações (leituras): %d\n", total_operacoes);
    printf("Tempo: %.6f segundos\n", tempo);
    
    if (tempo > 0.0) {
        printf("Throughput: %.2f KB/s\n", (total_bytes / 1024.0) / tempo);
    }
    
    printf("\nVerificação: diff dados/origem.txt dados/destino.txt\n");
    return 0;
}

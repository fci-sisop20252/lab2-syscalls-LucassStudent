# 📝 Relatório do Laboratório 2 - Chamadas de Sistema

---

## 1️⃣ Exercício 1a - Observação printf() vs 1b - write()

### 💻 Comandos executados:
```bash
strace -e write ./ex1a_printf
strace -e write ./ex1b_write
```

### 🔍 Análise

**1. Quantas syscalls write() cada programa gerou?**
- ex1a_printf: 1 syscalls
- ex1b_write: 7 syscalls

**2. Por que há diferença entre os dois métodos? Consulte o docs/printf_vs_write.md**

```
printf() é uma função de biblioteca que escreve em buffer interno. O conteúdo só vai para o kernel quando o buffer é liberado (flush), normalmente no '\n' final ou quando o buffer enche. Já write() é uma syscall direta, cada chamada vai imediatamente para o kernel. Por isso, printf() pode gerar menos syscalls que write().
```

**3. Qual método é mais previsível? Por quê você acha isso?**

```
write() é mais previsível, porque cada chamada corresponde a uma syscall exata. printf() depende do buffering interno da biblioteca C, que pode variar de acordo com o ambiente (terminal, arquivo, pipe).
```

---

## 2️⃣ Exercício 2 - Leitura de Arquivo

### 📊 Resultados da execução:
- File descriptor: 3
- Bytes lidos: 127

### 🔧 Comando strace:
```bash
strace -e openat,read,close ./ex2_leitura
```

### 🔍 Análise

**1. Qual file descriptor foi usado? Por que não começou em 0, 1 ou 2?**

```
0,1,2 são reservados para stdin, stdout e stderr. O próximo fd livre é atribuído ao open(), normalmente 3.
```

**2. Como você sabe que o arquivo foi lido completamente?**

```
Quando read() retorna 0, significa que chegou ao fim do arquivo (EOF).
```

**3. Por que verificar retorno de cada syscall?**

```
Porque syscalls podem falhar por vários motivos (arquivo não existe, permissão negada, disco cheio, etc.). Sempre é preciso checar retorno para evitar comportamentos inesperados.
```

---

## 3️⃣ Exercício 3 - Contador com Loop

### 📋 Resultados (BUFFER_SIZE = 64):
- Linhas: 25 (esperado: 25)
- Caracteres: 1300
- Chamadas read(): 21
- Tempo: 0.000097 segundos

### 🧪 Experimentos com buffer:

| Buffer Size | Chamadas read() | Tempo (s) |
|-------------|-----------------|-----------|
| 16          |        82       | 0.001378  |
| 64          |        21       | 0.000494  |
| 256         |        6        | 0.000299  |
| 1024        |        7        | 0.000274  |

### 🔍 Análise

**1. Como o tamanho do buffer afeta o número de syscalls?**

```
Quanto maior o buffer, menos chamadas read() são necessárias para ler o arquivo inteiro
```

**2. Todas as chamadas read() retornaram BUFFER_SIZE bytes? Discorra brevemente sobre**

```
Não. A última chamada pode retornar menos bytes, se o arquivo não for múltiplo do BUFFER_SIZE.
```

**3. Qual é a relação entre syscalls e performance?**

```
Mais syscalls = mais overhead de transição usuário = kernel = maior tempo total. Reduzir o número de syscalls com buffers maiores melhora a performance (tempo menor)
```

---

## 4️⃣ Exercício 4 - Cópia de Arquivo

### 📈 Resultados:
- Bytes copiados: 1364
- Operações: 6
- Tempo: 0.000205 segundos
- Throughput: 6497.71 KB/s

### ✅ Verificação:
```bash
diff dados/origem.txt dados/destino.txt
```
Resultado: [X] Idênticos [ ] Diferentes

### 🔍 Análise

**1. Por que devemos verificar que bytes_escritos == bytes_lidos?**

```
Garantir que bytes_escritos == bytes_lidos previne cópia incompleta ou corrompida.
```

**2. Que flags são essenciais no open() do destino?**

```
O_WRONLY | O_CREAT | O_TRUNC
```

**3. O número de reads e writes é igual? Por quê?**

```
O número de writes pode ser igual ou maior que o de reads, pois cada write pode não gravar todos os bytes lidos, exigindo chamadas adicionais.
```

**4. Como você saberia se o disco ficou cheio?**

```
Se write() retornar -1 com errno == ENOSPC, o disco está cheio.
```

**5. O que acontece se esquecer de fechar os arquivos?**

```
Arquivos abertos não são fechados, consumindo recursos e podendo causar cópia incompleta.
```

---

## 🎯 Análise Geral

### 📖 Conceitos Fundamentais

**1. Como as syscalls demonstram a transição usuário → kernel?**

```
Syscalls mostram a transição usuário → kernel, permitindo que o programa acesse recursos do sistema de forma segura.
```

**2. Qual é o seu entendimento sobre a importância dos file descriptors?**

```
File descriptors identificam arquivos e recursos abertos, permitindo ler, escrever e fechar de forma segura e organizada.
```

**3. Discorra sobre a relação entre o tamanho do buffer e performance:**

```
Buffers maiores reduzem syscalls e overhead, melhorando a performance; buffers pequenos tornam a operação mais lenta.
```

### ⚡ Comparação de Performance

```bash
# Teste seu programa vs cp do sistema
time ./ex4_copia
time cp dados/origem.txt dados/destino_cp.txt
```

**Qual foi mais rápido?** user

**Por que você acha que foi mais rápido?**


Porque o tempo "user" mede apenas o processamento em modo usuário, sem incluir o overhead de chamadas ao kernel, que geralmente é mais lento.


---

## 📤 Entrega
Certifique-se de ter:
- [X] Todos os códigos com TODOs completados
- [X] Traces salvos em `traces/`
- [X] Este relatório preenchido como `RELATORIO.md`

```bash
strace -e write -o traces/ex1a_trace.txt ./ex1a_printf
strace -e write -o traces/ex1b_trace.txt ./ex1b_write
strace -o traces/ex2_trace.txt ./ex2_leitura
strace -c -o traces/ex3_stats.txt ./ex3_contador
strace -o traces/ex4_trace.txt ./ex4_copia
```
# Bom trabalho!

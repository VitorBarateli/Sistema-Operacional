#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//Quantidade de bytes que vai ler do arquivo por vez
#define BUFFER_SIZE 1024    //Lê em blocos ao inves de caractere por caractere

int procurar_palavra(const char* line, const char* word){
    return strstr(line, word) != NULL;  //strstr: busca pela primeira ocorrencia da palavra word
}

//Funçao simular o grep. Ler o arquivo e buscar a palavra
void grep(const char *filename, const char *word){
    int fd = open(filename, O_RDONLY);  //Abre o arquivo no modo somente leitura

    if (fd == -1){
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];   //Array armazenar os dados lidos ado arquivo
    char line[BUFFER_SIZE];     //Array armazenar linha sendo construida a partir dos caracteres lidos
    int bytes_read = 0;         //Variavel que armazena o numero de bytes que foram lidos
    int line_pos = 0;           //Contador que indica a posiçao atual na line onde o proximo caractere lido sera armazenado

    //Loop que le os dados do arquivo em fd ate o numero de bytes do BUFFER_SIZE e armazena no buffer e depois atualiza a quantidade de bytes do bytes_read
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0){
        for (int i = 0; i < bytes_read; i++){   //Percorre cada byte/caractere
            if (buffer[i] == '\n' || line_pos >= BUFFER_SIZE - 1){  //Se o caractere atual for \n ou se a linha atingiu o tamanho máximo, a linha é finalizada com \0 para transformar o array de caracteres nos padroes de uma string
                line[line_pos] = '\0';
                if (procurar_palavra(line, word)){  //Chama a funçao para verificar se a palavra desejada esta no bloco de codigos
                    printf("%s\n", line);
                }
                line_pos = 0;
            } else {    //Adicona o caracter no array line para ir formando a linha de codigo
                line[line_pos++] = buffer[i];
            }
        }
    }

    if (bytes_read == -1){  //Se ocorrer um erro durante a leitura do arquivo, exibe uma mensagem e encerra o programa.
        perror("Erro ao ler o arquivo");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
}

int main (int argc, char *argv[]){
    if (argc != 3) {    //Verificação de argumentos: Verifica se o usuário forneceu exatamente 2 argumentos (palavra e nome do arquivo)
        fprintf(stderr, "Uso: %s <palavra> <arquivo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    grep(argv[2], argv[1]); //argv[1] = palavra buscada     argv[2] = nome do arquivo na qual vai buscar a palavra
    return 0;
}
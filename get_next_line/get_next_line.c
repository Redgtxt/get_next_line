//#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef SIZE
#define SIZE 1
#endif
   // printf("bytesRead: %zu\n",bytesRead);
        // printf("Buffer %s\n",buffer);
        //Funcao que passa o conteudo do buffer para o pointer
char	*get_next_line(int fd)
{
    static char buffer[SIZE];
    char *ptr;
    size_t bytesRead;
    size_t i;
    size_t j;

    ptr = NULL;
    bytesRead = 1;
    ptr = malloc(SIZE * sizeof(char));
    if(!ptr)
        return NULL;

    i = 0;
    while ((bytesRead = read(fd,buffer,SIZE)) > 0)
    {
        j = 0;
        while (j < bytesRead)
        {
            if (buffer[j] == '\0')
           {
                ptr[i++] = '\0'; // Termina a string
                return ptr; // Retorna a linha
            }

             ptr[i++] = buffer[j];
             j++;
        }
    }


    if (bytesRead == 0) {
        ptr[i] = '\0'; // Termina a string se atingiu o fim do arquivo
        return ptr;
    }

    free(ptr);
    return NULL;
}

// char fill_buffer(char *buffer)
// {
//     while(*buffer != '\0' || *buffer != '\n')
//     {
//         if(*buffer != buffer)
//         *buffer++;
//     }

// }
int	main(void)
{
	int		fd;
	char	*line;


	// Abre o arquivo "file.txt" para leitura
	fd = open("file.txt", O_RDONLY);
    line = get_next_line(fd);
	// Lê uma linha por vez e imprime na saída padrão
		printf("%s\n", line);


	// Fecha o arquivo
	close(fd);
    free(line);
	return (0);
}

#include "get_next_line.h"
/*
char	*get_next_line(int fd)
{
    static char buffer[SIZE];
    char *ptr;
    size_t bytesRead;
    size_t i;
    size_t j;

    ptr = NULL;
    bytesRead = 1;
    ptr = ft_calloc(SIZE + 1 , sizeof(char));
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
*/

void    print_newline_helper(char *buffer)
{

    while (*buffer &&  *buffer != '\0')
    {
        if (*buffer == '\n')
        {
            *buffer= '\\';
        }
        printf("%c",*buffer);
        buffer++;
    }
}


static char *read_from_file(int fd)
{
    int bytesRead;
     char *buffer;
     static int count = 1;


    printf("ft_calloc#[%d]---", count++);
    buffer = ft_calloc(BUFFER_SIZE + 1,sizeof(char));
    if(!buffer)
        return NULL;
    bytesRead = read(fd,buffer,BUFFER_SIZE);
    print_newline_helper(buffer);
    if(bytesRead <= 0)
    {
        free(buffer);
        return NULL;
    }

    return buffer;
}

char	*get_next_line(int fd)
{
    char *buffer;

    buffer = read_from_file(fd);
    return buffer;
}
int	main(void)
{
int    fd;
  char  *line;
  int  count;

  count = 0;
  fd = open("file.txt", O_RDONLY);
  if(fd == -1)
  {
    printf("Erro a abrir o ficheiro");
    return 1;
  }
  while(1)
  {
    line = get_next_line(fd);
    if(line == NULL)
        break;
    count++;
    printf("[%d]:%s\n", count, line); //count is to show you the line numbers
    free(line);
    line = NULL;
  }


  close(fd);
  return (0);
}

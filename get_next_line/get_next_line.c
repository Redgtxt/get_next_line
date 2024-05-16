#include "get_next_line.h"

char	*read_line(int fd, char *buffer_line)
{
	int	bytesRead;

	buffer_line = calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer_line)
		return (NULL);
	bytesRead = read(fd, buffer_line, BUFFER_SIZE);
	if (bytesRead <= 0)
	{
		free(buffer_line);
		return (NULL);
	}
	return (buffer_line);
}

char	*get_next_line(int fd)
{
	 char	*line;
     static char *buffer;
     
    line = read_line(fd,buffer);

    return(line);
}

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	line = get_next_line(fd);
	printf("%s", line);
	free(line);
    line = get_next_line(fd);
	printf("%s", line);
	free(line);
}

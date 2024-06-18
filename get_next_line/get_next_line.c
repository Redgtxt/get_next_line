/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguerrei < hguerrei@student.42lisboa.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:05:17 by hguerrei          #+#    #+#             */
/*   Updated: 2024/06/18 11:28:23 by hguerrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char		*line;
	static char	buffer[BUFFER_SIZE + 1];

	line = NULL;
	if(BUFFER_SIZE <= 0)
		return NULL;
	while (buffer[0] || (read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		line = ft_strjoin(line, buffer);
		if (flagfunc(buffer))
			break ;
	}
	return (line);
}

int	main(void)
{
  int fd = open("teste.txt", O_RDONLY);
  char *line = get_next_line(fd);

	while(line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	// printf("line: %s", line);
	// free(line);
	// printf("line: %s", line);
	// free(line);
	// printf("line: %s", line);
	// free(line);
  close(fd);
}

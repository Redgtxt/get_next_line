/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguerrei < hguerrei@student.42lisboa.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:05:17 by hguerrei          #+#    #+#             */
/*   Updated: 2024/06/11 14:22:02 by hguerrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	flagfunc(char *buffer)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	j = 0;
	flag= 0;
	while (buffer[j] != '\0')
	{
		if (flag == 1)
			buffer[i++] = buffer[j];
		if (buffer[j] == '\n')
			flag = 1;
		buffer[j++] = '\0';
	}
	return (flag);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	buffer[BUFFER_SIZE + 1];

	line = NULL;
	while (buffer[0] || (read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		// printf("Conteudo line:\n%s\n", buffer);
		line = ft_strjoin(line, buffer);
		if(flagfunc(buffer))
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
		 printf("line %s", line);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
		// 		printf("line: %s", line);
		// free(line);
		// 		printf("line: %s", line);
		// free(line);
		// 		printf("line: %s", line);
		// free(line);


    //line = get_next_line(fd);

  close(fd);
}


	// TEST(/* 1 */ gnl(fd, "01234567890123456789012345678901234567890\n");
	// 	 /* 2 */ gnl(fd, "987654321098765432109876543210987654321098\n");
	// 	 /* 3 */ gnl(fd, "0123456789012345678901234567890123456789012\n");
	// 	 /* 4 */ gnl(fd, "987654321098765432109876543210987654321098\n");
	// 	 /* 5 */ gnl(fd, "01234567890123456789012345678901234567890");
	// 	 /* 6 */ gnl(fd, NULL);) cout << ENDL; close(fd);


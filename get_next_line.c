/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelangl <tdelangl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:41:16 by tdelangl          #+#    #+#             */
/*   Updated: 2025/06/16 21:41:16 by tdelangl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(char *stash)
{
	int		len;
	char	*line;

	len = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	if (stash[len] == '\n')
		len++;
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	ft_strlcpy(line, stash, len + 1);
	return (line);
}

static char	*clean_stash(char *stash)
{
	int		i;
	int		j;
	char	*new_stash;
	int		len;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free(stash), NULL);
	i++;
	len = ft_strlen(stash);
	new_stash = malloc(sizeof(char) * (len - i + 1));
	if (!new_stash)
		return (free(stash), NULL);
	j = 0;
	while (stash[i])
		new_stash[j++] = stash[i++];
	new_stash[j] = '\0';
	free(stash);
	return (new_stash);
}

static char	*read_and_stash(int fd, char *stash)
{
	char	*buf;
	int		rd;
	char	*tmp;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	rd = 1;
	while (!ft_strchr(stash, '\n') && rd > 0)
	{
		rd = read(fd, buf, BUFFER_SIZE);
		if (rd < 0)
			return (free(buf), free(stash), NULL);
		buf[rd] = '\0';
		tmp = stash;
		stash = ft_strjoin(stash, buf);
		if (!stash)
			return (free(buf), NULL);
		free(tmp);
	}
	free(buf);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_and_stash(fd, stash);
	if (!stash || !*stash)
		return (free(stash), stash = NULL, NULL);
	line = extract_line(stash);
	stash = clean_stash(stash);
	return (line);
}
/*
int main(void)
{
	char *line;

	printf("FD invalide (-1)\n");
	line = get_next_line(-1);
	if (line == NULL)
		printf("OK : get_next_line(-1) retourne NULL\n");
	else
	{
		printf("KO : get_next_line(-1) aurait dû retourner NULL\n");
		free(line);
	}

	const char *files[] = {
		"empty.txt",
		"short_line.txt",
		"long_line.txt",
		"exact_buffer.txt",
		"plus_one.txt",
		"minus_one.txt",
		"multiple_lines.txt",
		"only_nl.txt",
		"everything.txt"
	};
	const char *descriptions[] = {
		"Empty",
		"Hi!",
		"aaaaa...",
		"taille BUFFER_SIZE",
		"taille BUFFER_SIZE + 1",
		"taille BUFFER_SIZE - 1",
		"Multiple lines",
		"only \\n",
		"Everything"
	};

	for (int i = 0; i < 9; i++)
	{
		printf("\nFichier : %s | Description : %s\n", files[i], descriptions[i]);
		int fd = open(files[i], O_RDONLY);
		if (fd < 0)
		{
			perror("open");
			continue;
		}
		int line_nb = 1;
		while ((line = get_next_line(fd)) != NULL)
		{
			printf("Ligne %d : %s", line_nb++, line);
			free(line);
		}
		close(fd);
	}

	printf("\nstdin:\n");
	while ((line = get_next_line(STDIN_FILENO)) != NULL)
	{
		printf("your input : %s", line);
		free(line);
	}

	return 0;
}
*/
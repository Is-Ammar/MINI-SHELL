/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:03:01 by iammar            #+#    #+#             */
/*   Updated: 2025/04/18 14:23:40 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../smash.h"

int	count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		if (*s == c)
		{
			in_word = 0;
		}
		s++;
	}
	return (count);
}

char	*get_next_word(const char **s, char c)
{
	const char	*start;
	int			length;
	char		*word;
	int			i;

	while (**s && **s == c)
		(*s)++;
	start = *s;
	while (**s && **s != c)
		(*s)++;
	length = *s - start;
	word = malloc(length + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < length)
	{
		word[i] = start[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

void	free_split(char **result)
{
	int	j;

	j = 0;
	while (result[j])
	{
		free(result[j]);
		j++;
	}
	free(result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		word_count;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (i < word_count)
	{
		result[i] = get_next_word(&s, c);
		if (!result[i])
		{
			free_split(result);
			return (NULL);
		}
		i++;
	}
	result[word_count] = NULL;
	return (result);
}
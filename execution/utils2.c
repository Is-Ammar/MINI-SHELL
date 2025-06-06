/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habdella <habdella@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:03:01 by iammar            #+#    #+#             */
/*   Updated: 2025/06/06 08:27:54 by habdella         ###   ########.fr       */
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

char	*get_next_word(t_shell *shell, const char **s, char c)
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
	word = ft_malloc(shell, length + 1, 0);
	i = 0;
	while (i < length)
	{
		word[i] = start[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(t_shell *shell, char const *s, char c)
{
	char	**result;
	int		word_count;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = ft_malloc(shell, (word_count + 1) * sizeof(char *), 0);
	while (i < word_count)
	{
		result[i] = get_next_word(shell, &s, c);
		if (!result[i])
			return (NULL);
		i++;
	}
	result[word_count] = NULL;
	return (result);
}

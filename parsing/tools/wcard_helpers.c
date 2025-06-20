/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wcard_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by habdella          #+#    #+#             */
/*   Updated: 2025/06/20 15:54:37 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	first_case(int **matrix, char *pattern, int plen, char *mask)
{
	int		i;

	i = 1;
	while (i <= plen)
	{
		if (pattern[i - 1] == '*' && mask[i - 1] == '1')
			matrix[0][i] = matrix[0][i - 1];
		i++;
	}
	return ;
}

int	pattern_evaluator(char *str, char *pattern, int **matrix, char *mask)
{
	int		i;
	int		j;
	int		str_len;
	int		pattern_len;

	str_len = ft_strlen(str);
	pattern_len = ft_strlen(pattern);
	matrix[0][0] = 1;
	first_case(matrix, pattern, pattern_len, mask);
	i = 1;
	while (i <= str_len)
	{
		j = 1;
		while (j <= pattern_len)
		{
			if (pattern[j - 1] == '*' && mask[j - 1] == '1')
				matrix[i][j] = (matrix[i][j - 1] || matrix[i - 1][j]);
			else if (str[i - 1] == pattern[j - 1])
				matrix[i][j] = matrix[i - 1][j - 1];
			j++;
		}
		i++;
	}
	return (matrix[str_len][pattern_len]);
}

char	*get_mask_stars(t_shell *shell, char *pattern)
{
	char	*mask;
	int		i;
	int		j;

	i = 0;
	mask = ft_strdup(shell, "");
	while (pattern[i])
	{
		j = i;
		while (pattern[i] && !ft_strchr("\"$'", pattern[i]))
			i++;
		if (j != i)
			mask = ft_strjoin(shell, mask \
			, set_val(shell, ft_strduplen(shell, &pattern[j], i - j), '1'));
		if (pattern[i] == '\'')
			mask = ft_strjoin(shell, mask \
			, set_val(shell, single_quote(shell, pattern, &i), '0'));
		else if (pattern[i] == '"')
			mask = ft_strjoin(shell, mask \
			, set_val(shell, double_quote(shell, pattern, &i), '0'));
		else if (pattern[i] == '$')
			mask = ft_strjoin(shell, mask, set_val(shell \
			, dollar_sign(shell, pattern, &i, FALSE), '1'));
	}
	return (mask);
}

int	search_match(t_shell *shell, char *pattern, char *str, char *old_val)
{
	int		i;
	int		str_len;
	int		pattern_len;
	int		**matrix;
	char	*mask_stars;

	i = 0;
	str_len = ft_strlen(str);
	pattern_len = ft_strlen(pattern);
	mask_stars = get_mask_stars(shell, old_val);
	matrix = ft_malloc(shell, sizeof(int *) * (str_len + 1), 0);
	while (i <= str_len)
	{
		matrix[i] = ft_malloc(shell, sizeof(int) * (pattern_len + 1), 0);
		memset(matrix[i], 0, sizeof(int) * (pattern_len + 1));
		i++;
	}
	if (pattern_evaluator(str, pattern, matrix, mask_stars))
		return (1);
	return (0);
}

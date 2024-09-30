#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	size_t	i;

	i = 0;
	while (s[i])
		++i;
	str = malloc(sizeof(char) * (i + 1));
	if (!str || !s || !f)
		return (NULL);
	i = -1;
	while (s[++i])
		str[i] = s[i];
	str[i] = '\0';
	i = -1;
	while (str[++i])
		str[i] = f(i, str[i]);
	return (str);
}

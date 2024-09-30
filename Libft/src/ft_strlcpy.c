#include "libft.h"

size_t	ft_strlcpy(char *dest, char const *src, size_t size)
{
	unsigned int	i;

	i = 0;
	if (size != 0)
	{
		while (src[i] && i < (size - 1))
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}

// #include <stdio.h>

// int main()
// {
// 	char dst[] = "sjhgdfjhsdgfjhsdgfjhsgfhjsdgfjhsdgf";
// 	char src[] = "Bonjour je suis un super beau gosse";
// 	printf("%ld\n", ft_strlcpy(dst, src, 20));
// 	return 0;
// }
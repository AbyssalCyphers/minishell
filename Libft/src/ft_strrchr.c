#include "libft.h"

char	*ft_strrchr(const char *string, int searchedChar)
{
	int	i;

	i = 0;
	while (string[i])
		++i;
	if (searchedChar == 0)
		return ((char *)&string[i]);
	while (--i >= 0)
		if (string[i] == (unsigned char)searchedChar)
			return ((char *)&string[i]);
	return (NULL);
}

// int main(void)
// {
// 	printf("%s\n", ft_strrchr("je cherche une lettre", 'e'));
// 	printf("%s\n", strrchr("je cherche une lettre", 'e'));
// 	return 0;
// }

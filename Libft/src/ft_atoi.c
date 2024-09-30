#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int		nbr;
	int		signe;
	int		i;
	char	*str;

	i = 0;
	str = (char *)nptr;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	signe = 1;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			signe = -signe;
	nbr = 0;
	while (str[i] >= '0' && str[i] <= '9')
		nbr = nbr * 10 + (str[i++] - 48);
	return (nbr * signe);
}

// #include <stdio.h>
// 
// int main()
// {
	// printf("%d\n", ft_atoi("-2147483648"));
// }

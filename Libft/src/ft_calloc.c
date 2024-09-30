#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t			i;
	unsigned char	*tmp;

	if (count != 0 && size != 0 && size > 2147483647 / count)
		return (NULL);
	i = -1;
	tmp = malloc(count * size);
	if (!tmp)
		return (NULL);
	while (++i < count * size)
		tmp[i] = 0;
	return (tmp);
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_shape
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char 	color;
}				t_shape;

typedef struct s_zone
{
	int		width;
	int		height;
	char	back;
}				t_zone;

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

int 	clear_all(FILE *file, char *drawing)
{
	if (drawing)
		free(drawing);
	fclose(file);
	return (1);
}

int check_zone(t_zone *zone)
{
	return (zone->width >= 0 && zone->width <= 300 && zone->height >= 0 && zone->height <= 300);
}

int		get_canvas(FILE *file, t_zone *zone)
{
	int ret;

	if ((ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->back)) != 3)
		return (0);
	if (!check_zone(zone))
		return (0);
	if (ret == -1)
		return (0);
	return (1);
}

char *paint_canvas(t_zone *zone)
{
	int i;
	char *str;

	i = 0;
	if (!(str = malloc(zone->width * zone->height)))
		return (NULL);
	while (i < zone->width * zone->height)
	{
		str[i] = zone->back;
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	get_shapes(FILE *file, t_zone *zone, char **drawing)
{
	t_shape tmp;
	int ret;

	while ((ret = fcanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.width, &tmp.height, &tmp.color)) == 6)
	{
		if (!check_shape(&tmp))
			return (0);
	}
}

/*
** Definimos estructuras necesarias para printar: una para el fondo y otra para cada rectangulo
** Iniciamos los valores del fondo y revisamos que los argumentos sean correctos
** Abrimos el archivo, comprobando errores
** Definimos el lienzo y rellenamos un array con los caracteres que haran de fondo
** Buscamos las formas que vamos a imprimir
** Printamos todo
** Limpiamos, cerramos y liberamos
*/ 

int main (int argc, char **argv)
{
	t_zone zone;
	char *drawing;
	FILE *file;

	drawing = NULL;
	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	if (!(file = fopen(argv[1], "r")))
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (clear_all(file, drawing)); 
	}
	if (!get_canvas(file, &zone))
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (clear_all(file, drawing));
	}
	if (!(drawing = paint_canvas(&zone)))
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (clear_all(file, drawing));
	}
	if (!get_shapes(file, &zone, drawing))
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (clear_all(file, drawing));
	}
	printf("%s\n", drawing);
	return (0);


}
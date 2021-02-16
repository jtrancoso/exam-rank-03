#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct	s_zone
{
	int		width;
	int		height;
	char	background;
}				t_zone;

typedef struct s_shape
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	color;	
}				t_shape;

int		ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	clear_all(FILE *file, char *drawing)
{
	fclose(file);
	if (drawing)
		free(drawing);
}

int	check_zone(t_zone *zone)
{
	return (zone->width > 0 && zone->width <= 300 && zone->height > 0 && zone->height <= 300);
}

char *paint_canvas(t_zone *zone)
{
	char *drawing;
	int i;

	if (!(drawing = malloc(zone->width * zone->height + 1)))
		return (NULL);
	i = 0;
	while (i < zone->height * zone->width)
	{
		drawing[i] = zone->background;
		i++;
	}
	drawing[i] = '\0';
	return (drawing);
}

int		get_zone(FILE *file, t_zone *zone)
{
	int ret;
	
	if ((ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
		return (0);
	if (!check_zone(zone))
		return (0);
	if (ret == -1)
		return (0);
	return (1);
}

int		check_shape(t_shape *shape)
{
	return (shape->width > 0.00000000 && shape->height > 0.00000000 && (shape->type == 'r' || shape->type == 'R'));
}

int		draw_shapes(FILE *file, t_zone *zone, char **drawing)
{
	int ret;
	t_shape tmp;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.width, &tmp.height, &tmp.color) != 6))
	{
		if (!check_shape(tmp))
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

	zone.width = 0;
	zone.height = 0;
	zone.background = 0;
	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	if (!(file = fopen(argv[1], "r")))
	{
		printf("hola\n");
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	if (!get_zone(file, &zone))
	{
		clear_all(file, drawing);
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	if (!(drawing = paint_canvas(&zone)))
	{
		clear_all(file, drawing);
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	if (!draw_shapes(file, &zone, &drawing))
	{
		clear_all(file, drawing);
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	printf("%d, %d, %c\n", zone.width, zone.height, zone.background);
	printf("len: %d\n", ft_strlen(drawing));
	write(1, drawing, ft_strlen(drawing));
	printf("\n");
	return (0);



}
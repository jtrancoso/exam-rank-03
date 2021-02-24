#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct	s_canvas
{
	int		width;
	int		height;
	char	back;
}				t_canvas;

typedef struct	s_circle
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	color;
}				t_circle;

int	ft_strlen(char *s)
{
	int i = 0;

	while (s[i])
		i++;
	return (i);
}

int		check_zone(t_canvas *canvas)
{
	return (canvas->width > 0 && canvas->width <= 300 && canvas->height <= 300 && canvas->height > 0);
}

int		get_canvas(t_canvas *canvas, FILE *file)
{
	int ret;

	if ((ret = fscanf(file, "%d %d %c\n", &canvas->width, &canvas->height, &canvas->back)) != 3)
		return (0);
	if (!check_zone(canvas))
		return (0);
	if (ret == -1)
		return (0);
	return (1);
}

char *paint_canvas(t_canvas *canvas)
{
	char *drawing;
	int i;

	if (!(drawing = malloc(canvas->width * canvas->height + 1)))
		return (NULL);
	i = 0;
	while (i < canvas->width * canvas->height)
	{
		drawing[i] = canvas->back;
		i++;
	}
	drawing[i] = '\0';
	return (drawing);
}

int		check_shape(t_circle *circle)
{
	return (circle->radius > 0.00000000 && (circle->type == 'c' || circle->type == 'C'));
}

int		in_circle(t_circle *circle, float x, float y)
{
	float distance;

	distance = sqrtf((x - circle->x) * (x - circle->x) + (y - circle->y) * (y - circle->y));
	if (distance <= circle->radius)
	{
		if ((circle->radius - distance) < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}

void	draw_shape(t_canvas *canvas, t_circle *circle, char **drawing)
{
	int i;
	int j;
	int ret;

	i = 0;
	while (i < canvas->height)
	{
		j = 0;
		while (j < canvas->width)
		{
			ret = in_circle(circle, j, i);
			if ((circle->type == 'c' && ret == 2) || (circle->type == 'C' && ret))
				(*drawing)[(i * canvas->width) + j] = circle->color;
			j++;
		}
		i++;
	}
}

int get_shape(t_canvas *canvas, FILE *file, char *drawing)
{
	t_circle tmp;
	int ret;

	while ((ret = fscanf(file, "%c %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.radius, &tmp.color)) == 5)
	{
		//printf("%c %f %f %f %c\n", tmp.type, tmp.x, tmp.y, tmp.radius, tmp.color);
		if (!check_shape(&tmp))
			return (0);
		draw_shape(canvas, &tmp, &drawing);
	}
	if (ret != -1)
		return (0);
	return (1);
}

void	draw_drawing(t_canvas *canvas, char *drawing)
{
	int i = 0;

	while (i < canvas->height)
	{
		write(1, drawing + (i * canvas->width), canvas->width);
		write(1, "\n", 1);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_canvas canvas;
	FILE	*file;
	char	*drawing;

	drawing = NULL;
	canvas.back = 0;
	canvas.height = 0;
	canvas.width = 0;
	if (argc != 2)
	{
		printf("Error: argument\n");
		return (1);
	}
	if (!(file = fopen(argv[1], "r")))
	{
		printf("Error: Operation file corrupted\n");
		return (1);
	}
	if (!get_canvas(&canvas, file))
	{
		printf("Error: Operation file corrupted\n");
		fclose(file);
		return (1);
	}
	if (!(drawing = paint_canvas(&canvas)))
	{
		printf("Error: Operation file corrupted\n");
		fclose(file);
		free(drawing);
		return (1);
	}
	if (!get_shape(&canvas, file, drawing))
	{
		printf("Error: Operation file corrupted\n");
		fclose(file);
		free(drawing);
		return (1);
	}
	draw_drawing(&canvas, drawing);
	return (0);
}

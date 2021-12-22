#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int w;
  int h;
  char back;
} t_zone;

typedef struct {
  char fill;
  float x;
  float y;
  float radius;
  char c;
} t_shape;

int error(char *msg) {
  while (*msg)
    write(1, msg++, 1);
  return 1;
}

int get_zone(t_zone *zone, FILE *file) {
  if (fscanf(file, "%d %d %c\n", &zone->w, &zone->h, &zone->back) != 3 ||
      !(zone->w > 0 && zone->w <= 300) || !(zone->h > 0 && zone->h <= 300))
    return 1;
  return 0;
}

void print_buffer(char *buffer, t_zone zone) {
  for (int i = 0; i < zone.h; i++) {
    for (int j = 0; j < zone.w; j++)
      write(1, &buffer[i * zone.w + j], 1);
    write(1, "\n", 1);
  }
}

int in_cercle(float x, float y, t_shape shape) {
  float distance;
  distance = sqrtf(powf(x - shape.x, 2.) + powf(y - shape.y, 2.));
  if (distance <= shape.radius) {
    if ((shape.radius - distance) < 1.00000000)
      return (2);
    return (1);
  }
  return (0);
}

void render_shape(t_shape shape, char *buffer, t_zone zone) {
  for (int i = 0; i < zone.h; i++) {
    for (int j = 0; j < zone.w; j++) {
      const int ret = in_cercle(j, i, shape);
      if ((shape.fill == 'c' && ret == 2) || (shape.fill == 'C' && ret))
        buffer[(i * zone.w) + j] = shape.c;
    }
  }
}

int render(FILE *file, char *buffer, t_zone zone) {
  t_shape shape;
  int ret = 0;
  while ((ret = fscanf(file, "%c %f %f %f %c\n", &shape.fill, &shape.x,
                       &shape.y, &shape.radius, &shape.c)) != -1) {
    if (ret != 5 || shape.radius <= 0.0f ||
        (shape.fill != 'C' && shape.fill != 'c'))
      return 1;
    render_shape(shape, buffer, zone);
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2)
    return error("Error: Argument\n");
  FILE *file = fopen(argv[1], "r");
  if (!file)
    return error("Error: Operation file corrupted\n");
  t_zone zone = {.w = 0, .h = 0, .back = '\0'};
  if (get_zone(&zone, file))
    return error("Error: Operation file corrupted\n");
  char buffer[zone.h * zone.w];
  memset(buffer, zone.back, sizeof(buffer));
  if (render(file, buffer, zone))
    return error("Error: Operation file corrupted\n");
  print_buffer(buffer, zone);
  return (0);
}

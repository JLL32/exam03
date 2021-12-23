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
  float w;
  float h;
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

int in_rectangle(float x, float y, t_shape shape) {
  if ((x < shape.x) || (shape.x + shape.w < x) || (y < shape.y) ||
      (shape.y + shape.h < y))
    return (0);
  if ((x - shape.x < 1.0f) || ((shape.x + shape.w) - x < 1.0f) ||
      (y - shape.y < 1.0f) || ((shape.y + shape.h) - y < 1.0f))
    return (2);
  return (1);
}

void render_shape(t_shape shape, char *buffer, t_zone zone) {
  for (int i = 0; i < zone.h; i++) {
    for (int j = 0; j < zone.w; j++) {
      const int ret = in_rectangle(j, i, shape);
      if ((shape.fill == 'r' && ret == 2) || (shape.fill == 'R' && ret))
        buffer[(i * zone.w) + j] = shape.c;
    }
  }
}

int render(FILE *file, t_zone zone) {
  t_shape shape;
  int ret = 0;
  char buffer[zone.h * zone.w];
  memset(buffer, zone.back, sizeof(buffer));

  while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &shape.fill, &shape.x,
                       &shape.y, &shape.w, &shape.h, &shape.c)) != -1) {
    if (ret != 6 || shape.w <= 0 || shape.h <= 0 ||
        (shape.fill != 'r' && shape.fill != 'R'))
      return 1;
    render_shape(shape, buffer, zone);
  }
  print_buffer(buffer, zone);
  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2)
    return error("Error: Argument\n");

  FILE *file = fopen(argv[1], "r");
  if (!file)
    return error("Error: Operation file corrupted\n");

  t_zone zone = {.w = 0, .h = 0, .back = '\0'};
  if (get_zone(&zone, file)) {
    fclose(file);
    return error("Error: Operation file corrupted\n");
  }

  if (render(file, zone)) {
    fclose(file);
    return error("Error: Operation file corrupted\n");
  }

  fclose(file);
  return (0);
}

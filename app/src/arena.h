#pragma once

#include "raylib.h"

typedef struct {
  Rectangle area;
  Color floor_color;
  Color sky_color;

} Arena;

Arena app_arena_new(int w, int h, float y);
void app_arena_draw(const Arena* arena);

#if defined(APP_ARENA_IMPL)

Arena app_arena_new(int w, int h, float y) {
  return (Arena){
      .area =
          (Rectangle){
              .width  = w,
              .height = h,
              .x      = 0,
              .y      = y,
          },
      .floor_color = LIGHTGRAY,
      .sky_color   = SKYBLUE,
  };
}

void app_arena_draw(const Arena* arena) {

  DrawRectangleRec(arena->area, arena->floor_color);

  DrawLine(0, (int)arena->area.y, arena->area.width, (int)arena->area.y, DARKGRAY);
}

#endif

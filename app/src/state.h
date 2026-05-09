#pragma once

#include "raylib.h"

typedef enum {
  STATE_IDLE = 0,
  STATE_JUMP,
  STATE_RUN
} PlayerState;

typedef struct {
  const Rectangle* p_frames;
  const int frame_count;
} AnimationClip;

// Global State
#define APP_MAX_SPRITES 1
#define APP_MAX_SFX 1
typedef struct {
  Texture2D p_sprites[APP_MAX_SPRITES];
  Sound p_sfx[APP_MAX_SFX];
} AppState;

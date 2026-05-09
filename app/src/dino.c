#pragma once

#include <assert.h>
#include <stddef.h>

#include "raylib.h"

#include "arena.h"
#include "game.h"
#include "macros.h"
#include "state.h"

#define DINO_SCALE 0.5f
#define DINO_FRAME_WIDTH 680
#define DINO_FRAME_HEIGHT 472
#define DINO_RENDER_WIDTH (DINO_FRAME_WIDTH * DINO_SCALE)    // 340
#define DINO_RENDER_HEIGHT (DINO_FRAME_HEIGHT * DINO_SCALE)  // DINO_RENDER_HEIGHT

typedef struct {
  const AnimationClip animations[3];

  Vector2 pos;
  Vector2 vel;

  Texture2D* p_sprites;
  Sound* p_jump_sfx;

  Rectangle jump_button;

  PlayerState state;

  int current_frame;
  int frames_counter;
  int frames_speed;

  bool is_on_ground;

} Dino;

Dino app_dino_new(Texture2D* p_sprites, Sound* p_jump_sfx) {
  assert(p_sprites != NULL);

  // Idle Frames (10 frames)
  static const Rectangle idle_frames[] = {
      {0, 0, 680, 472},      {680, 0, 680, 472},   {1360, 0, 680, 472}, {2040, 0, 680, 472},
      {2720, 0, 680, 472},   {3400, 0, 680, 472},  {0, 472, 680, 472},  {680, 472, 680, 472},
      {1360, 472, 680, 472}, {2040, 472, 680, 472}};

  // Jump Frames (12 frames)
  static const Rectangle jump_frames[] = {
      {2720, 472, 680, 472}, {3400, 472, 680, 472}, {0, 944, 680, 472},     {680, 944, 680, 472},
      {1360, 944, 680, 472}, {2040, 944, 680, 472}, {2720, 944, 680, 472},  {3400, 944, 680, 472},
      {0, 1416, 680, 472},   {680, 1416, 680, 472}, {1360, 1416, 680, 472}, {2040, 1416, 680, 472}};

  // Run Frames (8 frames)
  static const Rectangle run_frames[] = {{2720, 1416, 680, 472}, {3400, 1416, 680, 472},
                                         {0, 1888, 680, 472},    {680, 1888, 680, 472},
                                         {1360, 1888, 680, 472}, {2040, 1888, 680, 472},
                                         {2720, 1888, 680, 472}, {3400, 1888, 680, 472}};

  float btnSize                       = GetRenderHeight() * 0.15f;
  return (Dino){
      .animations     = {{idle_frames, 10}, {jump_frames, 12}, {run_frames, 8}},
      .p_sprites      = p_sprites,
      .p_jump_sfx     = p_jump_sfx,
      .jump_button    = (Rectangle){(float)GetRenderWidth() - btnSize - 60,
                                    (float)GetRenderHeight() - btnSize - 60, btnSize, btnSize},
      .pos            = (Vector2){(float)GetRenderWidth() / 2 - 340,
                                  (float)GetRenderHeight() / 2 - DINO_RENDER_HEIGHT},
      .vel            = {0},
      .state          = STATE_IDLE,
      .current_frame  = 0,
      .frames_counter = 0,
      .frames_speed   = 8,
  };
}

void app_dino_update(Dino* self, const Arena* arena) {
  const AnimationClip active_clip = self->animations[self->state];
  const float delta_time          = GetFrameTime();
  const int target_fps            = GetFPS();

  ++self->frames_counter;

  self->vel.y += GRAVITY * delta_time;
  self->pos.y += self->vel.y * delta_time;

  self->vel.x = 0;
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    self->vel.x = PLAYER_SPEED;
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    self->vel.x = -PLAYER_SPEED;

  ANDROID_ONLY({
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      Vector2 touch_pos = GetMousePosition();

      // Only move if we aren't specifically hitting the jump button
      // (prevents stopping while jumping)
      if (!CheckCollisionPointRec(touch_pos, self->jump_button)) {
        if (touch_pos.x > GetScreenWidth() / 2.0f) {
          self->vel.x = PLAYER_SPEED;
        } else {
          self->vel.x = -PLAYER_SPEED;
        }
      }
    }
  });

  self->pos.x += self->vel.x * delta_time;

  float sprite_bottom = self->pos.y + DINO_RENDER_HEIGHT;

  if (sprite_bottom >= arena->area.y) {
    self->pos.y        = arena->area.y - DINO_RENDER_HEIGHT;
    self->vel.y        = 0;
    self->is_on_ground = true;
  } else {
    self->is_on_ground = false;
  }

  if (self->pos.x < 0)
    self->pos.x = 0;
  if (self->pos.x > arena->area.width - DINO_RENDER_WIDTH)
    self->pos.x = (float)arena->area.width - DINO_RENDER_WIDTH;

  switch (self->state) {
    case STATE_JUMP: {
      // Logic: Only exit JUMP if landed AND animation reached near-end
      const AnimationClip jump_clip = self->animations[STATE_JUMP];
      bool anim_finished            = (self->current_frame >= jump_clip.frame_count - 1);

      if (self->is_on_ground && anim_finished) {
        if (self->vel.x != 0)
          self->state = STATE_RUN;
        else
          self->state = STATE_IDLE;

        self->current_frame  = 0;
        self->frames_counter = 0;
      }

    } break;

    case STATE_RUN:
    case STATE_IDLE: {
      PlayerState next_state = self->state;

      // Jump Logic (Desktop)
      if (IsKeyPressed(KEY_SPACE) && self->is_on_ground) {
        next_state = STATE_JUMP;
      }

      // Jump Logic (Android Button)
      ANDROID_ONLY({
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && self->is_on_ground) {
          if (CheckCollisionPointRec(GetMousePosition(), self->jump_button)) {
            next_state = STATE_JUMP;
          }
        }
      });

      // State Transition Application
      if (next_state == STATE_JUMP) {
        self->state = STATE_JUMP;
        self->vel.y = JUMP_FORCE;
        PlaySound(*self->p_jump_sfx);
        self->current_frame  = 0;
        self->frames_counter = 0;
      } else {
        // Switch between Idle and Run based on velocity
        PlayerState movement_state = (self->vel.x != 0) ? STATE_RUN : STATE_IDLE;
        if (movement_state != self->state) {
          self->state          = movement_state;
          self->current_frame  = 0;
          self->frames_counter = 0;
        }
      }
    } break;
  }

  if (self->frames_counter >= (GetFPS() / self->frames_speed)) {
    self->frames_counter = 0;
    ++self->current_frame;
    if (self->current_frame >= active_clip.frame_count)
      self->current_frame = 0;
  }
}

void app_dino_draw(Dino* self) {
  const float pad_y        = 25.f;
  const Rectangle dest_rec = {
      self->pos.x,
      self->pos.y,
      DINO_RENDER_WIDTH,
      DINO_RENDER_HEIGHT + pad_y,
  };
  const Vector2 origin            = {0, 0};
  const AnimationClip active_clip = self->animations[self->state];

  DrawTexturePro(*self->p_sprites, active_clip.p_frames[self->current_frame], dest_rec, origin,
                 0.0f, WHITE);

  ANDROID_ONLY({
    // Draw the button shadow/background (Semi-transparent gray)
    DrawRectangleRounded(self->jump_button, 0.5f, 10, Fade(GRAY, 0.5f));

    // Draw an outline so it's visible on light backgrounds
    DrawRectangleRoundedLines(self->jump_button, 0.5f, 2, Fade(WHITE, 0.8f));

    // Optional: Draw a simple arrow or label
    DrawText("^", self->jump_button.x + (self->jump_button.width / 2) - 10,
             self->jump_button.y + 10, 40, WHITE);
  });
}

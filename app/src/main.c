#include "raylib.h"

#define APP_ARENA_IMPL
#include "arena.h"
#include "dino.c"

#include "macros.h"

int main(void) {
  // 1024 576
  int screen_width  = 1024;
  int screen_height = 576;

  EMSCRIPTEN_ONLY({
    screen_width  = 1920;
    screen_height = 1080;
  });

  ClearWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED | FLAG_MSAA_4X_HINT);

  InitWindow(screen_width, screen_height, APP_NAME);
  InitAudioDevice();

  SetTargetFPS(60);

  AppState app = {
      .p_sprites =
          {
              LoadTexture(APP_ASSETS_DIR "anims/dino_spritesheet.png"),
          },
      .p_sfx =
          {
              LoadSound(APP_ASSETS_DIR "sfx/dragon-studio-cartoon-jump-463196.mp3"),
          },
  };

  Dino dino   = app_dino_new(&app.p_sprites[0], &app.p_sfx[0]);

  Arena arena = app_arena_new(GetRenderWidth(), GetScreenHeight(), GetRenderHeight() * .9);

  while (!WindowShouldClose()) {

    if (IsWindowResized()) {
      arena = app_arena_new(GetRenderWidth(), GetScreenHeight(), GetRenderHeight() * .9);
    }

    // UPDATE (ON EVENT)
    app_dino_update(&dino, &arena);

    // DRAW
    BeginDrawing();

    const char* nama_nim = "NAMA: RADEN DWITAMA BALIANO; NIM: 240030323";
    DrawText(nama_nim, (GetRenderWidth() / 2.f) - (MeasureText(nama_nim, 20) / 2.f),
             GetRenderHeight() / 2.f, 20, DARKGRAY);

    ClearBackground(SKYBLUE);

    app_dino_draw(&dino);
    app_arena_draw(&arena);

    DrawText("Hold SPACE: Jump | ARROWS: Run | None: Idle", 10, 10, 20, DARKGRAY);
    EMSCRIPTEN_ONLY({ DrawText("Click MOUSE LEFT: Enable audio", 10, 10 + 24, 20, DARKGRAY); });
    DrawText(TextFormat("State: %i | Frame: %i", dino.state, dino.current_frame),
             GetRenderWidth() * .76, 40, 20, MAROON);

    EndDrawing();
  }

  for (int i = 0; i < APP_MAX_SPRITES; ++i) {
    UnloadTexture(app.p_sprites[i]);
  }

  for (int i = 0; i < APP_MAX_SFX; ++i) {
    UnloadSound(app.p_sfx[i]);
  }
  CloseAudioDevice();
  CloseWindow();

  return 0;
}

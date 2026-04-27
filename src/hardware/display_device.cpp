#include "display_device.h"

#include <TFT_eSPI.h>
#include <lvgl.h>

#include "board/board_config.h"

namespace {

#define DRAW_BUF_SIZE \
  (BoardConfig::TFT_HOR_RES * BoardConfig::TFT_VER_RES / 10 * \
   (LV_COLOR_DEPTH / 8))

uint32_t draw_buf[DRAW_BUF_SIZE / 4];

uint32_t lvgl_tick() { return millis(); }

}  // namespace

bool DisplayDevice::begin() {
  lv_init();
  lv_tick_set_cb(lvgl_tick);

  lv_display_t* disp = lv_tft_espi_create(
      BoardConfig::TFT_HOR_RES, BoardConfig::TFT_VER_RES, draw_buf,
      sizeof(draw_buf));
  lv_display_set_rotation(disp, BoardConfig::TFT_ROTATION);

  _guiMutex = xSemaphoreCreateMutex();
  if (_guiMutex == nullptr) {
    Serial.println("Failed to create GUI mutex");
    return false;
  }

  return true;
}

bool DisplayDevice::lock(TickType_t timeoutTicks) {
  return _guiMutex != nullptr &&
         xSemaphoreTake(_guiMutex, timeoutTicks) == pdTRUE;
}

bool DisplayDevice::lockForMs(uint32_t timeoutMs) {
  return lock(pdMS_TO_TICKS(timeoutMs));
}

void DisplayDevice::unlock() {
  if (_guiMutex != nullptr) {
    xSemaphoreGive(_guiMutex);
  }
}

void DisplayDevice::handleTimers() { lv_timer_handler(); }

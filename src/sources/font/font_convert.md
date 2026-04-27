# LVGL font conversion

Online tool:

https://lvgl.io/tools/fontconverter

Local `ui_zh_20` generation command:

```bash
npx --yes lv_font_conv \
  --bpp 1 \
  --size 20 \
  --no-compress \
  --font .pio/libdeps/airm2m_core_esp32c3_dev/lvgl/scripts/built_in_font/SourceHanSansSC-Normal.otf \
  --symbols '正在启动请稍候等待配网置设备亮度模式主题深色浅单击切换双恢复出厂暂未用返回天气页来同步中已更新预报℃云周西体重阵扬特细霾少热六降雾月严雹冰薄一知极暴浓雷有速晴四年感间强南日北伴毛湿尘三小东/冷沙风二温级夹五端多阴雨到大冻浮雪无持续向霜干今明后杭州余：- WiF0123456789失败先连接密码浏览器访问长按确认与自低高' \
  --range 0-127 \
  --format lvgl \
  --lv-font-name ui_zh_20 \
  -o src/sources/font/ui_zh_20.c
```

When adding static Chinese UI text, add any new glyphs to `--symbols` and
regenerate `ui_zh_20.c`.

After regeneration, keep the existing `__has_include("lvgl.h")` include guard at
the top of `ui_zh_20.c` so PlatformIO can resolve the LVGL header.

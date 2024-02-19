#ifndef ST7735_FONTS_H_
#define ST7735_FONTS_H_

#include <stdint.h>

typedef struct{
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
}Font_t;


extern Font_t Font_7x10;
extern Font_t Font_11x18;
extern Font_t Font_16x26;

#endif /* ST7735_FONTS_H_ */

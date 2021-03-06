// IMG_TEMPERATURE
#define IMG_TEMPERATURE_width 16
#define IMG_TEMPERATURE_height 16
const char IMG_TEMPERATURE_bits[] = {
  0x80, 0x03, 0x40, 0x04, 0xC0, 0x04, 0x40, 0x04, 0x40, 0x04, 0xC0, 0x04, 
  0x40, 0x04, 0x40, 0x04, 0xC0, 0x04, 0x40, 0x04, 0x60, 0x0D, 0x30, 0x19, 
  0x90, 0x13, 0x90, 0x13, 0x30, 0x18, 0xE0, 0x0F, };
// IMG_WIND
#define IMG_WIND_width 16
#define IMG_WIND_height 16
const char IMG_WIND_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x84, 0x40, 0x02, 0x21, 0x00, 0x1E, 
  0x78, 0x00, 0x84, 0x40, 0x02, 0x21, 0x00, 0x1E, 0x78, 0x00, 0x84, 0x40, 
  0x02, 0x21, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, };
// IMG_HUMIDITY
#define IMG_HUMIDITY_width 16
#define IMG_HUMIDITY_height 16
const char IMG_HUMIDITY_bits[] = {
  0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0xC0, 0x01, 0xC0, 0x03, 0x40, 0x02, 
  0x60, 0x06, 0x60, 0x04, 0x30, 0x0C, 0x10, 0x08, 0x18, 0x18, 0x08, 0x10, 
  0x08, 0x10, 0x18, 0x18, 0x30, 0x0C, 0xE0, 0x07, };
// IMG_HPA
#define IMG_HPA_width 16
#define IMG_HPA_height 16
const char IMG_HPA_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x84, 0x10, 0x08, 0x21, 0x10, 0x42, 0x10, 0x42, 
  0x10, 0x42, 0x08, 0x21, 0x84, 0x10, 0x42, 0x08, 0x42, 0x08, 0x42, 0x08, 
  0x94, 0x52, 0x18, 0x63, 0x9C, 0x73, 0x00, 0x00, };
// IMG_DIRECTION
#define IMG_DIRECTION_width 16
#define IMG_DIRECTION_height 16
const char IMG_DIRECTION_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x70, 0x18, 0x70, 0x18, 
  0xF0, 0x18, 0xF0, 0x19, 0xB0, 0x1B, 0x30, 0x1F, 0x30, 0x1E, 0x30, 0x1C, 
  0x30, 0x1C, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, };
// IMG_CLOCK
#define IMG_CLOCK_width 16
#define IMG_CLOCK_height 16
const char IMG_CLOCK_bits[] = {
  0xE0, 0x07, 0xF8, 0x1F, 0x3C, 0x3C, 0x8E, 0x71, 0x86, 0x61, 0x87, 0xE1, 
  0x83, 0xC1, 0x83, 0xCF, 0x83, 0xCF, 0x03, 0xC0, 0x07, 0xE0, 0x06, 0x60, 
  0x0E, 0x70, 0x3C, 0x3C, 0xF8, 0x1F, 0xE0, 0x07, };

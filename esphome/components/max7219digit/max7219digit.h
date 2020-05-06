#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/spi/spi.h"

#ifdef USE_TIME
#include "esphome/components/time/real_time_clock.h"
#endif

namespace esphome {
namespace max7219digit {

class MAX7219Component;

using max7219_writer_t = std::function<void(MAX7219Component &)>;

class MAX7219Component : public PollingComponent,
                         public display::DisplayBuffer,
                         public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_LOW,
                                               spi::CLOCK_PHASE_LEADING, spi::DATA_RATE_1MHZ> {
 public:
  void set_writer(max7219_writer_t &&writer);

  void setup() override;

  void dump_config() override;

  void update() override;

  float get_setup_priority() const override;

  void display();

  void invert_on_off(bool on_off);
  void invert_on_off();

  void turn_on_off(bool on_off);

  void draw_absolute_pixel_internal(int x, int y, int color) override;
  int get_height_internal() override;
  int get_width_internal() override;

  void set_intensity(uint8_t intensity);
  void set_num_chips(uint8_t num_chips);
  void set_offset(uint8_t offset);

  void sendChar (const byte chip, const byte data);
  void send64pixels (const byte chip, const byte pixels [8]);

  void scroll_left (uint8_t stepsize);

  /// Evaluate the printf-format and print the result at the given position.
  uint8_t printdigitf(uint8_t pos, const char *format, ...) __attribute__((format(printf, 3, 4)));
  /// Evaluate the printf-format and print the result at position 0.
  uint8_t printdigitf(const char *format, ...) __attribute__((format(printf, 2, 3)));

  /// Print `str` at the given position.
  uint8_t printdigit(uint8_t pos, const char *str);
  /// Print `str` at position 0.
  uint8_t printdigit(const char *str);

#ifdef USE_TIME
  /// Evaluate the strftime-format and print the result at the given position.
  uint8_t strftimedigit(uint8_t pos, const char *format, time::ESPTime time) __attribute__((format(strftime, 3, 0)));

  /// Evaluate the strftime-format and print the result at position 0.
  uint8_t strftimedigit(const char *format, time::ESPTime time) __attribute__((format(strftime, 2, 0)));
#endif

 protected:
  void send_byte_(uint8_t a_register, uint8_t data);
  void send_to_all_(uint8_t a_register, uint8_t data);

  uint8_t intensity_{15};  /// Intensity of the display from 0 to 15 (most)
  uint8_t num_chips_{1};
  //uint8_t offset_char=0;
  uint8_t max_x=0;
  uint8_t offset_chips=0;
  bool invert=false;
  //uint8_t *buffer_;
  //uint8_t *bufferold_{nullptr};
  uint8_t stepsleft;
  size_t get_buffer_length_();
  optional<max7219_writer_t> writer_{};
};

}  // namespace max7219
}  // namespace esphome

#include "esphome.h"
enum DISPLAY_STATE {
  TIME,
  DATE,
  TEMP,
  HUMIDITY,
  BLANK,
  CONDITIONING,
};

class Nixie : public Component, public Switch {
 public:
  void setup() override {
    // SPI.begin();
    // This will be called once to set up the component
    // think of it as the setup() call in Arduino
  }
  void set_state(DISPLAY_STATE state){
    display_state = state;
  }
  void write_state(bool state) override {
    enabled = state;
    write();

    // Acknowledge new state by publishing it
    publish_state(state);
  }
  void write(){
    if(enabled){
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    SPI.write(buf[0]);
    SPI.write(buf[1]);
    SPI.write(buf[2]);
    SPI.endTransaction();
    }
    else{
      blank();
    }
  }
  void blank(){
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    SPI.write(0xFF);
    SPI.write(0xFF);
    SPI.write(0xFF);
    SPI.endTransaction();
  }
  void set_time(uint8_t h, uint8_t m, uint8_t s){
    ESP_LOGD("custom", "set time!");
    buf[0] = ((s % 10)<<4 & 0xf0) | (s/10) & 0x0f;
    buf[1] = ((m % 10)<<4 & 0xf0) | (m/10) & 0x0f;
    buf[2] = ((h % 10)<<4 & 0xf0) | (h/10) & 0x0f;
    write();
  }
  static Nixie* get(
      const custom_component::CustomComponentConstructor& c) {
      return static_cast<Nixie*>(c.get_component(0));
  }
 private:
  bool enabled;
  DISPLAY_STATE display_state;
  uint8_t buf[3];
};

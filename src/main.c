#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/pcm.h"
#include <math.h>

#define SAMPLE_RATE 44100
#define TWO_PI (2.0 * 3.14159265358979323846)

// Define GPIO pins for PCM5102 connections
#define SCK_PIN 2
#define BCK_PIN 3
#define DIN_PIN 4
#define LCK_PIN 5
#define GND_PIN 6
#define VIN_PIN 7

int main() {
    stdio_init_all();

    // Initialize GPIO pins for PCM5102
    gpio_init(SCK_PIN);
    gpio_init(BCK_PIN);
    gpio_init(DIN_PIN);
    gpio_init(LCK_PIN);
    gpio_init(GND_PIN);
    gpio_init(VIN_PIN);

    gpio_set_dir(SCK_PIN, GPIO_OUT);
    gpio_set_dir(BCK_PIN, GPIO_OUT);
    gpio_set_dir(DIN_PIN, GPIO_OUT);
    gpio_set_dir(LCK_PIN, GPIO_OUT);
    gpio_set_dir(GND_PIN, GPIO_OUT);
    gpio_set_dir(VIN_PIN, GPIO_OUT);

    // Set GPIO pins to appropriate values
    gpio_put(SCK_PIN, 0);
    gpio_put(BCK_PIN, 0);
    gpio_put(DIN_PIN, 0);
    gpio_put(LCK_PIN, 0);
    gpio_put(GND_PIN, 0);
    gpio_put(VIN_PIN, 1);

    // Set up PCM5102
    pcm_config cfg = pcm_default_config();
    cfg.sck = SCK_PIN;
    cfg.bck = BCK_PIN;
    cfg.din = DIN_PIN;
    cfg.lck = LCK_PIN;
    pcm_init(&cfg);

    // Set up PWM
    gpio_set_function(0, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(0);
    pwm_set_clkdiv(slice_num, 125.0f);
    pwm_set_wrap(slice_num, 1 << 16);
    pwm_set_enabled(slice_num, true);

    // Set up sine wave
    float frequency = 440.0;
    float phase = 0.0;
    float phase_increment = TWO_PI * frequency / SAMPLE_RATE;

    while (true) {
        // Generate sine wave sample
        uint16_t sample = (uint16_t)(sin(phase) * 32767 + 32768);

        // Output sample to PCM5102 and PWM
        pcm_put_sample_blocking(sample);
        pwm_set_chan_level(slice_num, PWM_CHAN_A, sample);

        // Update phase
        phase += phase_increment;
        if (phase >= TWO_PI) {
            phase -= TWO_PI;
        }
    }

    return 0;
}

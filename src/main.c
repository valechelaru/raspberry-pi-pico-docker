#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <math.h>

#define SAMPLE_RATE 44100
#define TWO_PI (2.0 * 3.14159265358979323846)

// Define PWM pins for PCM5102 connections
#define DATA_PIN 0
#define BCLK_PIN 2
#define LRCLK_PIN 3
#define RCK_PIN 4
#define SCK_PIN 5

// Generate PCM data stream for PCM5102 board
void output_pcm_sample(uint16_t sample) {
    uint16_t pwm_val = ((sample & 0xFF) << 4) | ((sample >> 8) & 0xF);
    pwm_set_gpio_level(DATA_PIN, pwm_val);
}

int main() {
    stdio_init_all();

    // Set up PWM for generating PCM data stream
    gpio_set_function(DATA_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(DATA_PIN);
    pwm_set_clkdiv(slice_num, 4.0f);
    pwm_set_wrap(slice_num, 4096);
    pwm_set_enabled(slice_num, true);

    // Initialize PCM5102 control pins
    gpio_init(BCLK_PIN);
    gpio_set_dir(BCLK_PIN, GPIO_OUT);
    gpio_put(BCLK_PIN, 0);
    gpio_init(LRCLK_PIN);
    gpio_set_dir(LRCLK_PIN, GPIO_OUT);
    gpio_put(LRCLK_PIN, 0);
    gpio_init(RCK_PIN);
    gpio_set_dir(RCK_PIN, GPIO_OUT);
    gpio_put(RCK_PIN, 0);
    gpio_init(SCK_PIN);
    gpio_set_dir(SCK_PIN, GPIO_OUT);
    gpio_put(SCK_PIN, 0);

    // Set up sine wave
    float frequency = 440.0;
    float phase = 0.0;
    float phase_increment = TWO_PI * frequency / SAMPLE_RATE;

    // Generate sine wave samples and output to PCM5102
    while (true) {
        // Generate sine wave sample
        uint16_t sample = (uint16_t)(sin(phase) * 32767 + 32768);

        // Output sample to PCM5102
        output_pcm_sample(sample);

        // Update control signals
        gpio_put(SCK_PIN, 1);
        gpio_put(SCK_PIN, 0);
        gpio_put(BCLK_PIN, 1);
        gpio_put(BCLK_PIN, 0);

        // Update LRCK every other sample (for stereo)
        static uint8_t lr_state = 0;
        if (++lr_state >= 2) {
            lr_state = 0;
            gpio_put(LRCLK_PIN, 1);
            gpio_put(LRCLK_PIN, 0);
            gpio_put(RCK_PIN, 1);
            gpio_put(RCK_PIN, 0);
        }

        // Update phase
        phase += phase_increment;
        if (phase >= TWO_PI) {
            phase -= TWO_PI;
        }
    }
}

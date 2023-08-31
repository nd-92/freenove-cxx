#include "Freenove_WS2812_Lib_for_Raspberry_Pi.hpp"
Freenove_WS2812::Freenove_WS2812(unsigned int gpio_pin, unsigned int led_count, unsigned int led_type)
{
    ledstring.channel[0].gpionum = static_cast<int>(gpio_pin);
    ledstring.channel[0].count = static_cast<int>(led_count);
    ledstring.channel[0].strip_type = static_cast<int>(led_type);
    ws2811_init(&ledstring);
}

template <class valueType>
valueType Freenove_WS2812::constrain(valueType value, valueType min, valueType max)
{
    if (value > max)
    {
        return max;
    }
    else if (value < min)
    {
        return min;
    }
    else
    {
        return value;
    }
}

void Freenove_WS2812::set_Led_Tpye(unsigned int type)
{
    ledstring.channel[0].strip_type = static_cast<int>(type);
}

void Freenove_WS2812::set_Led_Brightness(rgbIndex brightness)
{
    brightness = constrain<rgbIndex>(brightness, 0, 255);
    ledstring.channel[0].brightness = static_cast<rgbIndex>(brightness);
}

void Freenove_WS2812::set_Led_Color(unsigned int number, rgbIndex r, rgbIndex g, rgbIndex b)
{
    unsigned long color;
    r = constrain<rgbIndex>(r, 0, 255);
    g = constrain<rgbIndex>(g, 0, 255);
    b = constrain<rgbIndex>(b, 0, 255);
    color = (r << 16) | (g << 8) | b;
    ledstring.channel[0].leds[number] = color;
}

void Freenove_WS2812::show()
{
    ws2811_render(&ledstring);
}
void Freenove_WS2812::clear()
{
    for (int i = 0; i < ledstring.channel[0].count; i++)
    {
        ledstring.channel[0].leds[i] = 0;
    }
    ws2811_render(&ledstring);
    // ws2811_fini(&ledstring);
}

#include "adc_sample.h"
#include "adc.h"
#include "cmsis_os.h"
#include "bsp_log.h"

fp32 voltage = 0, temperature = 0;
volatile fp32 voltage_vrefint_proportion = 8.0586080586080586080586080586081e-4f;

void ADC_sample_Task(void const *argument)
{
    while (1)
    {
        voltage = get_battery_voltage();
        temperature = get_temprate();
        LOGINFO("adc_value: %d", (int) voltage);
        vTaskDelay(100);
    }
}


static uint16_t adcx_get_chx_value(ADC_HandleTypeDef *ADCx, uint32_t ch)
{
    static ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ch;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES; // ADC_SAMPLETIME_3CYCLES;

    if (HAL_ADC_ConfigChannel(ADCx, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_ADC_Start(ADCx);

    HAL_ADC_PollForConversion(ADCx, 10);
    return (uint16_t)HAL_ADC_GetValue(ADCx);
}
void init_vrefint_reciprocal(void)
{
    uint8_t i = 0;
    uint32_t total_adc = 0;
    for (i = 0; i < 200; i++)
    {
        total_adc += adcx_get_chx_value(&hadc1, ADC_CHANNEL_VREFINT);
    }
    voltage_vrefint_proportion = 200 * 1.2f / total_adc;
}

fp32 get_battery_voltage(void)
{
    fp32 voltage;
    uint16_t adcx = 0;

    adcx = adcx_get_chx_value(&hadc3, ADC_CHANNEL_8);
    voltage = (fp32)adcx * voltage_vrefint_proportion * 10.090909090909090909090909090909f;

    return voltage;
}

fp32 get_temprate(void)
{
    uint16_t adcx = 0;
    fp32 temperate;

    adcx = adcx_get_chx_value(&hadc1, ADC_CHANNEL_TEMPSENSOR);
    temperate = (fp32)adcx * voltage_vrefint_proportion;
    temperate = (temperate - 0.76f) * 400.0f + 25.0f;

    return temperate;
}



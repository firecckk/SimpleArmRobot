#include "ADC.h"

MedianFilter filterObject(60, 10); // window size, init value

// apply filter to ADC
int ADC_filter(int adc_raw) {
  int filtered_adc = filterObject.in(adc_raw);
  return(filtered_adc);
}

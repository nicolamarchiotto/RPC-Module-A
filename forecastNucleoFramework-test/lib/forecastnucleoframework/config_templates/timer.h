#ifndef CONFIG_TIMER_H
#define CONFIG_TIMER_H

#define TICKER_HW_TIMER $timer[hw_timer]$
#define TICKER_HW_TIMER_IRQn $timer[hw_timer]$_IRQn
#define TICKER_CLOCK_FREQ $timer[clock_freq]$

#define ENABLE_TICKER __HAL_RCC_$timer[hw_timer]$_CLK_ENABLE

#endif // CONFIG_TIMER_H
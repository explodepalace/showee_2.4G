#ifndef __PERF_TIME_H__
#define __PERF_TIME_H__

#include "stm32f10x.h"

#define PERF_TIME_ENABLE		1

#if PERF_TIME_ENABLE

static __inline void perf_time_init( void )
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= 1;
}

static __inline uint32_t perf_time_now_tick( void )
{
	return DWT->CYCCNT;
}

static __inline uint32_t perf_time_tick_to_us( uint32_t Tick )
{
	return (uint32_t)(((uint64_t)Tick * 1000000ULL) / SystemCoreClock);
}

static __inline uint32_t perf_time_tick_elapsed_us( uint32_t StartTick, uint32_t EndTick )
{
	return perf_time_tick_to_us( EndTick - StartTick );
}

static __inline uint32_t perf_time_now_us( void )
{
	return perf_time_tick_to_us( perf_time_now_tick( ) );
}

static __inline uint32_t perf_time_elapsed_us( uint32_t StartUs, uint32_t EndUs )
{
	return EndUs - StartUs;
}

#else

#define perf_time_init()					((void)0)
#define perf_time_now_tick()				0
#define perf_time_tick_to_us(Tick)			0
#define perf_time_tick_elapsed_us(StartTick, EndTick)	0
#define perf_time_now_us()					0
#define perf_time_elapsed_us(StartUs, EndUs)	0

#endif

#endif

//******************************************************************************
//*
//*     FULLNAME:  Single-Chip Microcontroller Real-Time Operating System
//*
//*     NICKNAME:  scmRTOS
//*
//*     PROCESSOR: AVR (Atmel)
//*
//*     TOOLKIT:   avr-gcc (GNU)
//*
//*     PURPOSE:   Project Level Target Extensions Config
//*
//*     Version: 5.0.0
//*
//*
//*     Copyright (c) 2003-2015, scmRTOS Team
//*
//*     Permission is hereby granted, free of charge, to any person
//*     obtaining  a copy of this software and associated documentation
//*     files (the "Software"), to deal in the Software without restriction,
//*     including without limitation the rights to use, copy, modify, merge,
//*     publish, distribute, sublicense, and/or sell copies of the Software,
//*     and to permit persons to whom the Software is furnished to do so,
//*     subject to the following conditions:
//*
//*     The above copyright notice and this permission notice shall be included
//*     in all copies or substantial portions of the Software.
//*
//*     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//*     EXPRESS  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//*     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//*     IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//*     CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//*     TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
//*     THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//*
//*     =================================================================
//*     Project sources: https://github.com/scmrtos/scmrtos
//*     Documentation:   https://github.com/scmrtos/scmrtos/wiki/Documentation
//*     Wiki:            https://github.com/scmrtos/scmrtos/wiki
//*     Sample projects: https://github.com/scmrtos/scmrtos-sample-projects
//*     =================================================================
//*
//******************************************************************************
//*     avr-gcc port by Oleksandr O. Redchuk, Copyright (c) 2007-2015

#ifndef  scmRTOS_TARGET_CFG_H
#define  scmRTOS_TARGET_CFG_H

#include <avr/io.h>
#include <avr/interrupt.h>


#if scmRTOS_CONTEXT_SWITCH_SCHEME == 1

#  if defined(SPM_READY_vect)
#    define CONTEXT_SWITCH_ISR_VECTOR  SPM_READY_vect
#    define SPM_CONTROL_REG SPMCSR
#  elif defined(SPM_RDY_vect)
#    define CONTEXT_SWITCH_ISR_VECTOR  SPM_RDY_vect
#    define SPM_CONTROL_REG SPMCR
#  else
#    error "SPM ready interrupt vector not defined"
#  endif

#endif

#ifndef __ASSEMBLER__

//------------------------------------------------------------------------------
//
//       System Timer stuff
//
//
#if defined(TIMER2_COMP_vect)
#  define SYSTEM_TIMER_VECTOR TIMER2_COMP_vect
#else
#  error "Timer2 compare vector not defined"
#endif

#if defined(TIMSK)
#  define TIMER2_IE_REG TIMSK
#else
#  error "Timer2 interrupt mask register not defined"
#endif

#if defined(TCCR2)
#  define TIMER2_CS_REG TCCR2
#else
#  error "Timer2 control register for CS bits not defined"
#endif

#if defined(OCR2)
#  define TIMER2_OC_REG OCR2
#else
#  error "Timer2 compare register not defined"
#endif

#define LOCK_SYSTEM_TIMER()    ( TIMER2_IE_REG &= ~(1 << TOIE2) )
#define UNLOCK_SYSTEM_TIMER()  ( TIMER2_IE_REG |=  (1 << TOIE2) )


//------------------------------------------------------------------------------
//
//       Context Switch ISR stuff
//
//   SPM_READY interrupt is the lowest possible priority interrupt. Moreover, it is rarely
// or even never used in application section. So, this interrupt is ideal solution for
// software interrupt emulation.
//   By default SPM_READY interrupt perform the context switching, enabling the interrupt
// will force context switching. But SPM_READY interrupt must be disabled manually when 
// switching ends, so we provide ContextSwitchUserHook() (see below).
//   ContextSwitchUserHook() called from OS::TKernel::ContextSwitchHook(stack_item_t* sp)
//  (see OS_Kernel.h)
//   You can change or delete the hook if you are use another vector for context switching.
// Do not forget to set scmRTOS_CONTEXT_SWITCH_USER_HOOK_ENABLE to 0 in scmRTOS_CONFIG.h when
// the hook not defined.
//

namespace OS
{

	#if scmRTOS_CONTEXT_SWITCH_SCHEME == 1

	// enable and raise SPM interrupt
	INLINE void raise_context_switch() { SPM_CONTROL_REG |= (1 << SPMIE);  }
	
	// disable SPM interrupt
	INLINE void block_context_switch() { SPM_CONTROL_REG &= ~(1 << SPMIE); }

	class TNestedISRW
	{
		public:
		INLINE TNestedISRW() : State(SPM_CONTROL_REG) { block_context_switch(); sei(); }
		INLINE ~TNestedISRW()
		{
			cli();
			SPM_CONTROL_REG = State;
		}
		private:
		uint8_t State;
	};

	#define ENABLE_NESTED_INTERRUPTS() OS::TNestedISRW NestedISRW

	#  if scmRTOS_CONTEXT_SWITCH_USER_HOOK_ENABLE != 1
	#    error "scmRTOS_CONTEXT_SWITCH_USER_HOOK_ENABLE must be 1 for SPM_READY interrupt context switcher"
	#  endif

	#else // scmRTOS_CONTEXT_SWITCH_SCHEME

	#define ENABLE_NESTED_INTERRUPTS() sei()

	#endif // scmRTOS_CONTEXT_SWITCH_SCHEME
}

#endif // __ASSEMBLER__


#endif // scmRTOS_TARGET_CFG_H
//-----------------------------------------------------------------------------
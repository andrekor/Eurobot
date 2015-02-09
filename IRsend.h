// defines for timer2 (8 bits)
#if defined(IR_USE_TIMER2)
#define TIMER_ENABLE_PWM     (TCCR2A |= _BV(COM2B1))
#define TIMER_DISABLE_INTR   (TIMSK2 = 0)
#define TIMER_ENABLE_PWM     (TCCR2A |= _BV(COM2B1))

// defines for timer1 (16 bits)
#elif defined(IR_USE_TIMER1)
#define TIMER_ENABLE_PWM     (TCCR1A |= _BV(COM1A1))
#if defined(__AVR_ATmega8P__) || defined(__AVR_ATmega8__)
	#define TIMER_DISABLE_INTR   (TIMSK = 0)
#else
	#define TIMER_DISABLE_INTR   (TIMSK1 = 0)
#define TIMER_DISABLE_PWM    (TCCR1A &= ~(_BV(COM1A1)))
#define TIMER_ENABLE_PWM     (TCCR1A |= _BV(COM1A1))

// defines for timer3 (16 bits)
#elif defined(IR_USE_TIMER3)
#define TIMER_ENABLE_PWM     (TCCR3A |= _BV(COM3A1))
#define TIMER_DISABLE_INTR   (TIMSK3 = 0)
#define TIMER_DISABLE_PWM    (TCCR3A &= ~(_BV(COM3A1)))
#define TIMER_ENABLE_PWM     (TCCR3A |= _BV(COM3A1))

// defines for timer4 (10 bits, high speed option)
#elif defined(IR_USE_TIMER4_HS)
#define TIMER_ENABLE_PWM     (TCCR4A |= _BV(COM4A1))
#define TIMER_DISABLE_INTR   (TIMSK4 = 0)
#define TIMER_DISABLE_PWM    (TCCR4A &= ~(_BV(COM4A1)))
#define TIMER_ENABLE_PWM     (TCCR4A |= _BV(COM4A1))

// defines for timer4 (16 bits)
#elif defined(IR_USE_TIMER4)
#define TIMER_ENABLE_PWM     (TCCR4A |= _BV(COM4A1))
#define TIMER_DISABLE_INTR   (TIMSK4 = 0)
#define TIMER_DISABLE_PWM    (TCCR4A &= ~(_BV(COM4A1)))
#define TIMER_ENABLE_PWM     (TCCR4A |= _BV(COM4A1))

// defines for timer5 (16 bits)
#elif defined(IR_USE_TIMER5)
#define TIMER_ENABLE_PWM     (TCCR5A |= _BV(COM5A1))
#define TIMER_DISABLE_INTR   (TIMSK5 = 0)
#define TIMER_DISABLE_PWM    (TCCR5A &= ~(_BV(COM5A1)))
#define TIMER_ENABLE_PWM     (TCCR5A |= _BV(COM5A1))

// defines for special carrier modulator timer
#elif defined(IR_USE_TIMER_CMT)
#define TIMER_ENABLE_PWM     CORE_PIN5_CONFIG = PORT_PCR_MUX(2)|PORT_PCR_DSE|PORT_PCR_SRE
#define TIMER_DISABLE_INTR   NVIC_DISABLE_IRQ(IRQ_CMT)
#define TIMER_DISABLE_PWM    CORE_PIN5_CONFIG = PORT_PCR_MUX(1)|PORT_PCR_DSE|PORT_PCR_SRE
#define TIMER_ENABLE_PWM     CORE_PIN5_CONFIG = PORT_PCR_MUX(2)|PORT_PCR_DSE|PORT_PCR_SRE
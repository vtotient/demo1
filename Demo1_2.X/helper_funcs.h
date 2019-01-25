#define LED_BLUE LATBbits.LATB14
#define LED_GREEN LATDbits.LATD7
#define LED_RED LATDbits.LATD5
#define PIN_D9 LATDbits.LATD9

typedef union
{
    uint8_t all_flags;

    struct 
    {
        uint8_t led_flag : 1,
                timer  : 1,
                btn_pressed : 1,
                reliable : 1,
                toggle : 1,
                spare4 : 1,
                spare5 : 1,
                spare6 : 1;
    };

}FLAGS;

typedef enum
{
  ONE_TWO = 0x0,
  ONE_FOUR = 0x1,
  ONE_EIGHT = 0x2,
  ONE_SIXTEEN = 0x3,
} CLK_RATIO;

typedef enum
{
    BUTTON_COLOR_RED = 0,
    BUTTON_COLOR_GREEN = 1,
    BUTTON_COLOR_BLUE = 2
} BUTTON_COLOR;


/* Function prototypes */
void lib_stall(int time);
void lib_blink(FLAGS *flag);
void DEBOUNCE_Task(FLAGS *time_elapsed);
void set_RGB_LED(int pot);
bool config_PWM(PWM_GENERATOR pwmx, uint16_t dc, uint16_t phase, uint16_t per);
void enable_PWM_CLK_DIV(PWM_GENERATOR pmx);
void set_PWM_CLK_DIV(CLK_RATIO ratio);
typedef union
{
    uint8_t all_flags;

    struct 
    {
        uint8_t led_flag : 1,
                timer  : 1,
                spare1 : 1,
                spare2 : 1,
                spare3 : 1,
                spare4 : 1,
                spare5 : 1,
                spare6 : 1;
    };

}FLAGS;

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
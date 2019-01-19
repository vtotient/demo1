#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifndef DEBOUNCE_CONFIG_BUTTON_COUNT
#define DEBOUNCE_CONFIG_BUTTON_COUNT 4
#endif

#ifndef DEBOUNCE_CONFIG_DEFAULT_SAMPLE_WINDOW
#define DEBOUNCE_CONFIG_DEFAULT_SAMPLE_WINDOW 50        //50ms by default
#endif

struct debounced_button
{
    bool (*IsPressedFunction)(void);
    void (*CallbackFunction)(bool);
    uint16_t sampleWindow;
    bool pressState;
    uint16_t count;
};

static struct debounced_button buttons[DEBOUNCE_CONFIG_BUTTON_COUNT];

#ifdef DEBOUNCE_CONFIG_PARAMETER_CHECK
static bool validateButtonHandle(struct debounced_button * buttonHandle)
{
    uint8_t i;

    for(i=0; button<DEBOUNCE_CONFIG_BUTTON_COUNT; i++)
    {
        if(&buttons[i] == buttonHandle)
        {
            return true;
        }
    }

    return false;
}
#endif

void DEBOUNCE_Initialize(void)
{
    memset(buttons, 0, sizeof(buttons));
}

void DEBOUNCE_Tasks(void)
{   
    uint8_t button;
    struct debounced_button *pButton = &buttons[0];
    
    for(button=0; button<DEBOUNCE_CONFIG_BUTTON_COUNT; button++)
    {
        pButton = &buttons[button];
        
        if(pButton->IsPressedFunction == NULL)
        {
            continue;
        }
        
        if(pButton->IsPressedFunction())
        {
            if(pButton->count < pButton->sampleWindow)
            {                
                if(pButton->pressState == false)
                {
                    if(pButton->count == (pButton->sampleWindow - 1) )
                    {
                        if(pButton->CallbackFunction != NULL)
                        {
                            pButton->CallbackFunction(true);
                        }
                        pButton->pressState = true;
                    }
                }
                
                pButton->count++;
            }
        }
        else
        {
            if(pButton->count > 0)
            {                
                if(pButton->pressState == true)
                {
                    if(pButton->count == 1)
                    {
                        if(pButton->CallbackFunction != NULL)
                        {
                            pButton->CallbackFunction(false);
                        }
                        pButton->pressState = false;
                    }
                }
                
                pButton->count--;
            }
        }
    }
}

struct debounced_button* DEBOUNCE_AddButton(bool (*IsPressedFunction)(void))
{    
    uint8_t button;
    struct debounced_button* pButton = NULL;
    
    if( IsPressedFunction == NULL )
    {
        return NULL;
    }
    
    for(button=0; button<DEBOUNCE_CONFIG_BUTTON_COUNT; button++)
    {
        if(buttons[button].IsPressedFunction == NULL)
        {      
            buttons[button].pressState = IsPressedFunction();
            buttons[button].sampleWindow = DEBOUNCE_CONFIG_DEFAULT_SAMPLE_WINDOW;
            buttons[button].CallbackFunction = NULL;
            buttons[button].count = 0;
            
            /* The IsPressedFunction is last because this is what officially
             * "adds" the button. */
            buttons[button].IsPressedFunction = IsPressedFunction;     
            
            pButton = &buttons[button];
            break;
        }
    }
    
    return pButton;
}

void DEBOUNCE_SetCallback(struct debounced_button *buttonHandle, void (*CallbackFunction)(bool))
{   
#ifdef DEBOUNCE_CONFIG_PARAMETER_CHECK
    if(validateButtonHandle(buttonHandler) == false)
    {
        return false;
    }
#endif
    
    buttonHandle->CallbackFunction = CallbackFunction;
}

void DEBOUNCE_SetSampleWindow(struct debounced_button * buttonHandle, uint16_t milliseconds)
{    
#ifdef DEBOUNCE_CONFIG_PARAMETER_CHECK
    if(validateButtonHandle(buttonHandler) == false)
    {
        return false;
    }
#endif
    
    buttonHandle->sampleWindow = milliseconds;
}

bool DEBOUNCE_IsPressed(struct debounced_button * buttonHandle)
{
#ifdef DEBOUNCE_CONFIG_PARAMETER_CHECK
    if(validateButtonHandle(buttonHandler) == false)
    {
        return false;
    }
#endif
    
    return buttonHandle->pressState;
}

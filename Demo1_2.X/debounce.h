#include <stdbool.h>
#include <stdint.h>

struct debounced_button;

void DEBOUNCE_Initialize(void);
void DEBOUNCE_Tasks(void);

//return: NULL on error, button instance handle otherwise.
struct debounced_button* DEBOUNCE_AddButton(bool (*IsPressed)(void));

void DEBOUNCE_SetCallback(struct debounced_button*, void (*Callback)(bool));
void DEBOUNCE_SetSampleWindow(struct debounced_button*, uint16_t milliseconds);
bool DEBOUNCE_IsPressed(struct debounced_button*);


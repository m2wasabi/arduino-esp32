#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"

#include "./nefry/NefryWebServer.h"
#include "./nefry/Nefry.h"
#include "./nefry/NefryWiFi.h"
#include "./nefry/NefryConfig.h"
#include "./nefry/NefryConsole.h"
#include "./nefry/NefryUpdate.h"
#include "./nefry/NefryDisplay.h"

#if CONFIG_AUTOSTART_ARDUINO

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

void loopTask(void *pvParameters)
{
	while (Nefry.getBootMode() == -1) { delay(1); }
	while (Nefry.getBootMode() == 0) {
		Nefry.pollingSW();
		delay(1);
	}
	if (Nefry.getWriteMode() != true) {
		delay(500);
		setup();
		if(Nefry.isConfigureChanged() == true) {
			Nefry.reset();
		}
	}
	for(;;) {
		delay(5);
        micros(); //update overflow
		if (Nefry.getWriteMode() != true && Nefry.getNefryState() != 1) {
			loop();
		}
    }
}

void NefryBackEnd(void *pvParameters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Nefry.nefry_init();
	if(Nefry.getWifiEnabled()){
		NefryWeb.begin();
		NefryWiFi.beginWeb();
		NefryConfig.beginWeb();
		NefryWebServer.begin();
		NefryConsole.beginWeb();
		NefryUpdate.setupWebLocalUpdate();
		delay(500);
	}
	for (;;) {
		delay(1);
		if (Nefry.getNefryState() == 1) {
			if(Nefry.getWifiEnabled()){
				NefryWebServer.run();
			}
		}
		else {
			vTaskDelayUntil(&xLastWakeTime, 10 / portTICK_PERIOD_MS);
			if(Nefry.getWifiEnabled()){
				NefryWeb.run();
				NefryWebServer.run();
			}
			Nefry.pollingSW();
			Nefry.nefry_loop();
		}
	}
}
void NefryLedBlink(void *pvParameters)
{
	for (;;) {
		delay(10);
		Nefry.LedBlinkTask();
		delay(10);
		NefryDisplay.autoScrollTask();
	}
}

extern "C" void app_main()
{
    initArduino();
	xTaskCreatePinnedToCore(&NefryBackEnd, "NefryBackEnd", 4096, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(loopTask,      "loopTask",     8192, NULL, 2, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(&NefryLedBlink,"NefryLedBlink",2048, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
}

#endif

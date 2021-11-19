#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define TFT_GREY 0x5AEB // New colour

const uint8_t LED_PIN = PB3;
int count = 0;

//------------------------------------------------------------------------------
// high priority for blinking LED
static void vLEDOnBoardFlashTask(void *pvParameters) {
  UNUSED(pvParameters);
  pinMode(LED_BUILTIN, OUTPUT);

  // Flash led every 1000 ms.
  for (;;) {
    // Turn LED on.
    digitalWrite(LED_BUILTIN, HIGH);

    // Sleep for 500 milliseconds.
    vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

    // Turn LED off.
    digitalWrite(LED_BUILTIN, LOW);

    // Sleep for 500 milliseconds.
    vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
  }
}
//------------------------------------------------------------------------------
static void vLEDFlashTask(void *pvParameters) {
  UNUSED(pvParameters);
  pinMode(LED_PIN, OUTPUT);

  
  for (;;) {
    // Turn LED of.
    digitalWrite(LED_PIN, LOW);

    // Sleep for 30000 milliseconds.
    vTaskDelay((120000L * configTICK_RATE_HZ) / 1000L);

    // Turn LED on.
    digitalWrite(LED_PIN, HIGH);

    // Sleep for 25000 milliseconds.
    vTaskDelay((10000L * configTICK_RATE_HZ) / 1000L);
  }
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(2);
  // wait for Leonardo
  while(!Serial) {}

  // create blink task
  xTaskCreate(vLEDOnBoardFlashTask,
    "Task1",
    configMINIMAL_STACK_SIZE + 50,
    NULL,
    tskIDLE_PRIORITY + 2,
    NULL);

  // create print task
  xTaskCreate(vLEDFlashTask,
    "Task2",
    configMINIMAL_STACK_SIZE + 50,
    NULL,
    tskIDLE_PRIORITY + 1,
    NULL);

  // start FreeRTOS
  vTaskStartScheduler();

  // should never return
  Serial.println(F("Die"));
  while(1);
}
//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
  // Fill screen with random colour so we can see the effect of printing with and without 
  // a background colour defined
  tft.fillScreen(random(0xFFFF));
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'tft.println'
  //  or stay on the line is there is room for the text with tft.print)
  tft.setCursor(0, 0, 2);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_WHITE,TFT_BLACK); tft.setTextSize(2); 
  // We can now plot text on screen using the "print" class
  tft.println("Contador");
 
  
  // Set the font colour to be yellow with no background, set to font 7
  tft.setTextColor(TFT_YELLOW); tft.setTextFont(7);
  tft.println(count);
  
  
  delay(125000);
  count ++;
  
}
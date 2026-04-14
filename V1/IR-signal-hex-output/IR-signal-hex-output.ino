#include <IRrecv.h>
#include <IRutils.h>

#define IR_PIN 0
#define BUF_SIZE 1024
#define IR_TIMEOUT 15
#define MIN_UNKNOWN 12

IRrecv irrecv(IR_PIN, BUF_SIZE, IR_TIMEOUT, true);
decode_results results;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== IR Hex Logger — ESP32-C3 ===");
  Serial.printf("Listening on GPIO%d\n\n", IR_PIN);
  irrecv.setUnknownThreshold(MIN_UNKNOWN);
  irrecv.enableIRIn();
}

void loop() {
  if (!irrecv.decode(&results)) {
    delay(10);
    return;
  }

  if (results.decode_type != UNKNOWN && !results.repeat) {
    String protocol = typeToString(results.decode_type, false);
    uint8_t bytes = (results.bits + 7) / 8;

    // Convert protocol name to uppercase for the macro name
    protocol.toUpperCase();
    protocol.replace(" ", "_");

    Serial.printf("#define %s_VALUE 0x%0*llX\n", protocol.c_str(), bytes * 2, results.value);
  }

  irrecv.resume();
}
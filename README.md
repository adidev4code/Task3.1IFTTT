# SIT210 – Task 3.1P: Creating an IFTTT Trigger

## 1. System Description (how the system works)
The system monitors sunlight for a terrarium using an Arduino Nano 33 IoT and a light-dependent resistor (LDR). The Arduino continuously reads an analog voltage produced by the LDR and a fixed 10 kΩ resistor (voltage divider). When the measured light level crosses a predefined threshold, the Arduino sends an HTTPS webhook to the IFTTT Webhooks service. IFTTT receives the webhook and performs the configured action (mobile notification or email), thereby notifying the user that sunlight has started or stopped.

### a) Function of hardware components
- **Arduino Nano 33 IoT**: central controller and Wi-Fi interface; samples the analog sensor, executes logic (threshold + hysteresis), and sends HTTPS webhooks to IFTTT.  
- **LDR (photoresistor)**: sensor whose resistance varies with light; used to measure ambient light intensity.  
- **10 kΩ resistor**: forms a voltage divider with the LDR to produce a measurable analog voltage at the Arduino analog input.  
- **LED (optional)**: local visual indicator showing detected sunlight state (ON when sunlight detected).  
- **Breadboard and jumper wires**: provide prototyping connections and mechanical stability for the circuit.

### b) IFTTT trigger mechanism
Two events are used:
- `terrarium_sunlight_on` — triggered when the measured light rises above the selected threshold.
- `terrarium_sunlight_off` — triggered when the measured light falls below the selected threshold.

Each webhook call includes the sensor reading as `value1` so IFTTT can include this value in the notification.

### c) Notification mechanism
When IFTTT receives a `terrarium_sunlight_on` or `terrarium_sunlight_off` event, the corresponding IFTTT applet performs the configured action: send a push notification via the IFTTT mobile app or send an email. The notification text can include `{{Value1}}`, allowing the message to show the numeric sensor reading and help validate the event.

## 2. Testing procedure (concise — less than two paragraphs)
Measure the analog readings from the sensor in bright sunlight and in shade using the Serial Monitor. Calculate a threshold as a value roughly midway between the typical bright and shade readings, and set the `THRESHOLD` constant in the sketch. Run the device and perform at least five ON/OFF cycles by alternately exposing and covering the LDR; observe the Serial Monitor and confirm that each state change prints the expected detection messages and that the matching IFTTT notification is received.

Acceptable performance is defined as receiving at least four out of five correct notifications within a few seconds of each physical change and stable operation (no repeated false triggers) over a continuous monitoring period (e.g., 30 minutes). Capture the Serial Monitor log and screenshots of received notifications as submission evidence.





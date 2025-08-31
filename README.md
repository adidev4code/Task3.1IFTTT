# SIT210 – Task 3.1P: Creating an IFTTT Trigger

## 1. System Description

The project monitors sunlight exposure for a terrarium using an **Arduino Nano 33 IoT** and a **light-dependent resistor (LDR)**. The Arduino measures light intensity through a voltage divider made from the LDR and a fixed resistor. When sunlight starts or stops (based on a defined threshold), the Arduino sends a **Webhook request** to the IFTTT Webhooks service.

Each webhook transmits three values:

* **Value1** → total sunlight time accumulated for the day (minutes).
* **Value2** → duration of the most recent sunlight burst (seconds).
* **Value3** → sensor reading (proxy for lux) at the transition.

IFTTT receives the webhook and triggers the configured applets, which generate real-time mobile notifications to keep the user informed of sunlight conditions for the terrarium.

---

### a) Function of hardware components

* **Arduino Nano 33 IoT**: central microcontroller; reads the sensor, tracks sunlight timing, and sends HTTPS webhook requests to IFTTT using onboard Wi-Fi.
* **LDR (photoresistor)**: sensor whose resistance decreases as light increases, enabling sunlight measurement.
* **10 kΩ resistor**: forms a voltage divider with the LDR, producing an analog voltage the Arduino can measure on pin A0.
* **Breadboard and jumper wires**: provide electrical connections and a stable prototyping setup.
* **(Optional) LED**: can be used for local indication of sunlight state.

---

### b) IFTTT trigger mechanism

Two events are implemented:

* `sunlight_started` → sent when the sensor reading rises above the set threshold (sunlight detected).
* `sunlight_stopped` → sent when the reading drops below the threshold (sunlight ended).

Each event sends a JSON payload with `value1`, `value2`, and `value3`. IFTTT substitutes these into notification templates (e.g., “🌞 Terrarium sunlight STARTED – Total today: {{Value1}} minutes”).

---

### c) Notification mechanism

When an event is triggered, IFTTT delivers a **push notification** via the IFTTT mobile app (or optionally email). The notifications are customized to include `{{Value1}}`, `{{Value2}}`, and `{{Value3}}`, giving the user meaningful real-time data on terrarium sunlight exposure.

---

## 2. Hardware Setup

### Components

* Arduino Nano 33 IoT
* LDR (photoresistor)
* 10 kΩ resistor
* Breadboard + jumper wires
* USB cable for programming/power

### Circuit Wiring (Voltage Divider Method)

```
[Arduino Nano 33 IoT]    
   3.3V ──────┐
              │
              └── LDR ────── A0
                          │
                          └── 10kΩ resistor ────── GND
```

* One leg of the **LDR** → 3.3V.
* Other leg of the **LDR** → Analog input A0 **and** one side of the **10 kΩ resistor**.
* Other side of the **resistor** → GND.

This creates a voltage divider:

* In bright sunlight → LDR resistance drops → A0 voltage increases.
* In shade/dark → LDR resistance rises → A0 voltage decreases.

The Arduino uses these voltage changes to decide when sunlight starts and stops.

---

## 3. Testing procedure

1. Use the Arduino IDE Serial Monitor to observe raw sensor values under bright sunlight and in shaded/dark conditions.
2. Select a threshold roughly midway between these readings and set it in the sketch.
3. Run the system and alternate between exposing the LDR to sunlight and covering it, creating several start/stop cycles.
4. Verify through Serial Monitor that the Arduino prints correct detection messages and that IFTTT delivers matching notifications with the expected data values.
5. Successful operation is defined as:

   * Notifications received consistently within a few seconds of sunlight changes.
   * Total sunlight time (`Value1`) increases correctly across multiple bursts.
   * Last burst duration (`Value2`) matches the actual exposure time.
   * Sensor value (`Value3`) corresponds with the measured light level at each event.

Stability is further confirmed by leaving the system running for at least 30 minutes without false triggers. Evidence includes Serial Monitor logs and screenshots of the received IFTTT notifications.


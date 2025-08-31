# SIT210 – Task 3.1P: Creating an IFTTT Trigger

## 1. System Description

This system monitors the amount of sunlight reaching a terrarium using an **Arduino Nano 33 IoT** and a **BH1750 digital light sensor**. The Arduino continuously measures the light intensity in lux. When the measured value crosses a set threshold (300 lx), the Arduino sends a secure HTTPS request to the **IFTTT Webhooks service**.

The IFTTT platform receives the webhook and runs a pre-defined applet, which sends a **notification to the user’s mobile device**. This helps the user track when sunlight starts or stops reaching the terrarium.

---

### a) Function of hardware components

* **Arduino Nano 33 IoT**: Works as the main controller. It connects to Wi-Fi, reads the BH1750 sensor, makes threshold decisions, and sends HTTPS webhook requests to IFTTT.
* **BH1750 light sensor**: Measures ambient light intensity in lux with high accuracy and provides digital readings over the I²C bus.
* **Breadboard and jumper wires**: Allow connections between Arduino and the sensor during prototyping.

---

### b) IFTTT trigger mechanism

Two Webhook events are defined and connected to IFTTT applets:

* **sunlight\_started** → triggered when measured lux rises above the threshold (300 lx).
* **sunlight\_stopped** → triggered when measured lux falls below the threshold (300 lx).

Each event sends three values:

* **Value1** → Total sunlight minutes accumulated during the day.
* **Value2** → Duration of the last sunlight burst (in seconds).
* **Value3** → Light intensity in lux at the moment of the event.

---

### c) Notification mechanism

When an event is triggered, IFTTT sends a push notification to the user’s mobile device. The message template is set inside the applet and includes the values passed from Arduino.

* Example notification for sunlight start:

  ```
  ☀️ Sunlight detected!
  Total today: {{Value1}} minutes
  Last burst: {{Value2}} seconds
  Lux at start: {{Value3}}
  ```

* Example notification for sunlight stop:

  ```
  🌙 Sunlight stopped!
  Total today: {{Value1}} minutes
  Last burst: {{Value2}} seconds
  Lux at stop: {{Value3}}
  ```

This ensures the user receives real-time updates about sunlight availability and cumulative exposure.

---

## 2. Testing Procedure

1. Upload the final sketch to the Arduino Nano 33 IoT.
2. Open the Serial Monitor to view lux readings from the BH1750 sensor.
3. Place the sensor in shade and then expose it to bright light to simulate sunlight ON/OFF.
4. Observe the following behavior:

   * When lux rises above 300 → Serial Monitor prints `☀️ Sunlight detected!`, and the Arduino triggers the `sunlight_started` event.
   * When lux falls below 300 → Serial Monitor prints `🌙 Sunlight stopped!`, and the Arduino triggers the `sunlight_stopped` event.
5. Confirm that push notifications are received on the IFTTT mobile app.
6. Perform at least **five cycles** of light ON/OFF.

   * Acceptable performance = at least **four correct notifications** received and **no false triggers** during a continuous observation period.

---

## 3. Example Serial Monitor Output

```
Connected to WiFi!
BH1750 ready!
Light: 2810.00 lx
☀️ Sunlight detected!
Connecting to IFTTT for event: sunlight_started
Event: sunlight_started
Status: 200
Response: Congratulations! You've fired the sunlight_started event
Light: 2584.17 lx
```

---

## 4. Hardware Setup

### Wiring Connections (Arduino Nano 33 IoT → BH1750 sensor)

* **3.3V** → VCC
* **GND** → GND
* **A4 (SDA)** → SDA
* **A5 (SCL)** → SCL

The circuit requires no additional resistors because the BH1750 communicates digitally over I²C.

---

## 5. Results and Conclusion

The system successfully detects sunlight start and stop events, sends HTTPS requests to IFTTT, and receives notifications on a mobile device. The Serial Monitor provides real-time lux readings, ensuring transparency and verification of the trigger logic.

This solution demonstrates how **Arduino + IoT cloud services (IFTTT)** can be integrated to create a real-time monitoring system for environmental conditions.

---


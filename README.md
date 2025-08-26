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
The Arduino issues an HTTPS GET request to the IFTTT Webhooks endpoint using the pattern:

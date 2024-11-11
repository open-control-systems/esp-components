# Senserion SHT41 Sensor Verification

This project is designed to verify temperature and humidity readings for the SHT41 sensor.

**Set Up the Hardware:**

- Connect the sensor to the ESP32 board.

**Configure the Software:**

- Ensure you have the ESP-IDF framework installed.
- Configure your `sdkconfig` as per your hardware setup.

**Build and Flash:**

```bash
idf.py build
idf.py flash
```

## Configuration

The firmware can be configured using the following command:

```bash
idf.py menuconfig
```

There are many configuration options, see options prefixed with "OCS_TOOLS_SHT41_".

## Usage

**Power On the Device:**

Ensure the ESP32 board is powered on and connected to your computer.

**Monitor the Serial Output:**

```bash
idf.py monitor
```

This will display the result of the temperature and humidity readings. Example output:

```txt
I (386) sht41_verifier: temperature=22.69 humidity=52.06
I (1396) sht41_verifier: temperature=22.68 humidity=52.07
I (2406) sht41_verifier: temperature=22.69 humidity=52.06
I (3416) sht41_verifier: temperature=22.71 humidity=52.08
I (4426) sht41_verifier: temperature=22.70 humidity=52.07
I (5436) sht41_verifier: temperature=22.69 humidity=52.07
I (6446) sht41_verifier: temperature=22.71 humidity=52.10
I (7456) sht41_verifier: temperature=22.69 humidity=52.08
I (8466) sht41_verifier: temperature=22.70 humidity=52.09
I (9476) sht41_verifier: temperature=22.67 humidity=52.05
I (10486) sht41_verifier: temperature=22.69 humidity=52.06
I (11496) sht41_verifier: temperature=22.70 humidity=52.10
I (12506) sht41_verifier: temperature=22.68 humidity=52.07
I (13516) sht41_verifier: temperature=22.71 humidity=52.03
I (14526) sht41_verifier: temperature=22.69 humidity=52.07
I (15526) sht41_verifier: verification finished: total=15 failed=0
```

## License

This project is licensed under the MPL 2.0 License - see the LICENSE file for details.

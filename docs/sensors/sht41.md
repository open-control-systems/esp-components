## Introduction

[SHT41](https://sensirion.com/products/catalog/SHT41) is a digital humidity and temperature sensor. SHT41 is connected to a board with I2C interface. See the [datasheet](https://sensirion.com/media/documents/33FD6951/662A593A/HT_DS_Datasheet_SHT4x.pdf) for more details.

## Telemetry Fields

- Flat formatting:

```json
{
    "sensor_sht41_heating_count": 7,
    "sensor_sht41_humidity": 54.87,
    "sensor_sht41_temperature": 21.92
}
```

- Nested formatting:

```json
{
    "SHT41": {
        "heating_count": 7,
        "humidity": 54.87,
        "temperature": 21.92
    }
}
```
## HTTP API

- `bonsai-firmware.local/sensor/sht41/reset` - reset the sensor.
- `bonsai-firmware.local/sensor/sht41/heat` - activate internal heater, use with caution, the heater is designed for a maximum duty cycle of 10%, meaning the total heater-on-time should not be longer than 10% of the sensor’s lifetime.

## Firmware Configuration Options

**Sensor Configuration**

- CONFIG_BONSAI_FIRMWARE_SENSOR_SHT41_READ_INTERVAL

**Depends on the I2C Configuration**

- CONFIG_BONSAI_FIRMWARE_I2C_MASTER_SDA_GPIO
- CONFIG_BONSAI_FIRMWARE_I2C_MASTER_SCL_GPIO

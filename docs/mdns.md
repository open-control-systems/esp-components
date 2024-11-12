## Introduction

It's been decided to use mDNS for the following reasons:
- If the IP address is changed, it will be required to connect to the device to get its new IP address
- It will be a bit annoying to type the full IP address into the browser address bar each time to reach the device

mDNS allows you to assign a memorable hostname to the device. Later this hostname can be used instead of the IP address to send HTTP requests to the device.

## Firmware Configuration Options

- CONFIG_OCS_NETWORK_MDNS_HOSTNAME
- CONFIG_OCS_NETWORK_MDNS_INSTANCE_NAME

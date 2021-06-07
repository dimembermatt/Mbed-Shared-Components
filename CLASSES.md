# Class Design

This document describes the classes defined in Array-Shared-Components and how
they interact with each other.

```cpp
// Inheritance
InterruptDevice
|* inherited by
| ------ Sensor
|        |* inherited by
|        | ------ AdcSensor
|        |        |* inherited by
|        |        | ------ CurrentAdcSensor
|        |        | ------ VoltageAdcSensor
|        |
|        | ------ SpiSensor
|        |        |* inherited by
|        |        | ------ TemperatureSpiSensor
|        |
|        | ------ I2cSensor
|                 |* inherited by
|                 | ------ IrradianceI2cSensor
|
| ------ SerialDevice
| ------ CanDevice

Filter
|* inherited by
| ------ EMAFilter
| ------ KalmanFilter
| ------ MedianFilter
| ------ SMAFilter

// Utilization
ComDevice
|* utilizes
| ------ SerialDevice
| ------ CanDevice

SerialDevice
|* utilizes
| ------ Message

CanDevice
|* utilizes
| ------ Message
| ------ CanIdList

Message
```

---

## InterruptDevice

The InterruptDevice is a inherited class for devices that must occur on a
staggered rate; classes that inherit InterruptDevice should define an internal
`handler` method which dictates what the device does when an interrupt occurs.

All interrupt devices have access to a `startUs`, `startMs`, and `stop` API
that the class instance can utilize to manage interrupt events. All interrupt
devices have the same priority.

### SerialDevice

The SerialDevice is an inherited class of InterruptDevice; it acts at a fixed
interrupt rate to capture serial messages from the device buffer and relocate it
to an internal buffer. The internal buffer saturates at a maximum buffer size.

The SerialDevice has an asynchronous method to send a Message type message, and
to retrieve the oldest message in the internal buffer (in FIFO style).

### CanDevice

The CanDevice is an inherited class of InterruptDevice; it acts at a fixed
interrupt rate to capture CAN messages from the device buffer and relocates it
into an internal buffer. The internal buffer saturates at a maximum buffer size.

The CanDevice has an asynchronous method to send a Message type message, and to
retrieve the oldest message in the internal buffer (in FIFO style). The
CanDevice also has the ability to add a set of CAN ID filters, defined in
CanIdList.h, that will automatically purge irrelevant messages from the internal
buffer.

### Sensor

The Sensor subclass represents devices that share the following API:

- setFilter
- getValue
- clearHistory
- handler
- inherited InterruptDevice methods

Sensors synchronously collect data, are able to filter data, and derived classes
preprocess and inject the data. They can retrieve sensor data from a
asynchronous getter method.

#### AdcSensor, SpiSensor, I2cSensor

These Sensor subclasses differ mainly from each other by the method in which
they capture the data, implemented by their `handler` method. Their constructors
require different pins.

- AdcSensor: VoltageSensor, CurrentSensor
- SpiSensor: TemperatureSpiSensor
- I2cSensor: IrradianceI2cSensor

---

## ComDevice

The ComDevice class, or Communication Device class, is an abstraction layer for
Serial and CAN communication. It generates an instance of a SerialDevice or
CanDevice based on initial arguments, and exposes their shared API for users.
The ComDevice utilizes the Message class as the communication protocol of choice.

---

## Message

The Message class implements a shared standard for messages used by ComDevice,
SerialDevice, and CanDevice. It seamlessly converts a message ID and message
data parameter into a format that can translate into type 1 or type 2 DeSeCa
packet configuration formats. Instances of this class is used by ComDevices,
SerialDevices, and CanDevices to transmit data.

---

## CanIdList

The CanIdList hold definitions for various CAN IDs. In particular, CAN IDs for
the solar array and MPPT team are defined here.
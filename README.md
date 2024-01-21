# Embedded-Challenge

Objective:
Embedded system design focuses on gathering useful data, processing that data, and
providing a useful representation of information. In the past decade we have seen an
explosion of wearable health devices ranging from heart rate sensors to step counters
to distance trackers. These devices are designed to help us meet our fitness goals and
help us keep in good physical shape.

The objective of this semester’s embedded challenge is to build a wearable
speedometer which can calculate velocity by measuring angular velocities available
from our built-in gyroscope (L3GD20) - without a GPS. Our gyroscope is able to measure
3-axis angular velocity. Strategically placing the sensor on the legs or feet can capture
the angular velocities and with a bit of processing can convert those angular velocities
to linear forward velocity and calculate distance traveled (using only a gyro!)

Required Parts:
1. STM32F429 Discovery Board with built in gyroscope
2. Power supply/USB power bank
3. A way to store 20 seconds of velocity data, sampled at 0.5 second intervals
(storing in memory is fine)

The challenge involves the following procedure:
1. Attach your STM32F429 and power supply to any suitable part of your body
2. Interface the gyro using SPI to capture the angular velocities
3. Write the code to sample the angular velocities every 0.5s.
4. Convert those measurements to linear forward velocity.
5. Record 20 seconds worth of data that can be exported through the USB or
extracted from memory somehow.
6. Calculate the overall distance traveled during the 20 seconds of measurements.

Restrictions:
● This is either a 2-student project OR an individual project to be done
independently by each student or student group.
● No other components may be used other than those specified above.
● The PlatformIO programming environment must be used.
● You will be allowed to use any drivers/HAL functions available through the IDE

Grading Criteria:
● 20% - Ability to successfully and continuously measure gyro values from the
angular velocity sensor
● 30% - Ability to convert measured data to forward movement velocity
● 20% - Ability to calculate distance traveled
● 15% - Creativity
● 15% - Well written and organized code

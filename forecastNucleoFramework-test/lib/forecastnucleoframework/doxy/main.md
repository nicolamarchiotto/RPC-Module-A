# ForecastNucleoFramework

Welcome to the documentation for the Forecast Nucleo Framework.
This framework aims to create a simple and fast way to implement benchmarking applications on [stm32Nucleo F446fe](https://os.mbed.com/platforms/ST-Nucleo-F446RE/) shields. It is built uppon the framwork [mbed](https://os.mbed.com/).

The framework provide a simple way to run a controller algorithm on a specific hardware. To help the user to create is own test the Framework provide also a  \ref forecast::App::execControlLoop(ulong) "App::execControlLoop" function that aims to run the control loop maintaining a specific frequency. 

The loop is composed by the following point: 
- ```Update the readings from the hw sensors```
- ```Generate the reference for the control algoritm``` 
- ```Exec the control algoritm``` 
- ```Update the torque of the motor with the one provided by the algorithm```
- ```Log the desired values to the pc to allow the user to benchmark the algorithm```

## Hardware interface and sensor readings

To allow the user to get access to the sensors, the user during the reference genration, the control and during the logging function has access to a constant pointer to the class \ref forecast::Hardware witch allow the user to get the last read value from the sensors and to current time.

## Control implementation and reference generation

To use the framework the user will need to create a class that inherits from the interface \ref forecast::Controller interface.

The methods of this implementation will be called to perform the control during the control loop by teh application.
To generate the reference to the user will have to write a function that provide a reference for the control algorithm foreach loop cycle.

## Logging functionalities





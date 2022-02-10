# MiniXController

The MiniXController software is for the [Amptek MiniX X-Ray Tube](https://www.amptek.com/products/mini-x2-x-ray-tube). 

The goal of this project was to add some features to the already-existing software to improve UX during experiments. The new features include:
* An experiment duration timer that automatically stops the MiniX once the experiment is complete
* An automated procedure to warm-up the MiniX upon first use
* A service hour tracker  

## Using the software

For safety & precaution, the Debug configuration is preset to include a custom environment (MiniXMock) to simulate responses from the MiniX specifically made for development. 
The Release file can be used directly with the MiniX X-ray tube and is currently in use at Concordia University's Lanthanide Research Lab.

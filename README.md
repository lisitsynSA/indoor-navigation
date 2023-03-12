# indoor-navigation

In this project mathematical model for solving the problem of internal positioning was implemented. According to the signal level received by the camera, a probabilistic distribution is built around it, taking into account the following factors: the directional pattern of the cameras, errors in removing the signal level, the presence of obstacles (attenuation and reflection of the signal), the speed limit of the beacon. Further, the probability distributions of each chamber are multiplied and the maximum is found by the gradient descent method. The search for the extremum starts from the previous location of the beacon.

![image](https://user-images.githubusercontent.com/13435493/224559751-feb49aad-b4d3-4bfd-b760-f219acc38eb6.png)


![image](https://user-images.githubusercontent.com/13435493/224559538-f696ac59-9348-411a-a8da-bac5bbd26739.png)

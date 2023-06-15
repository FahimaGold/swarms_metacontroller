# Swarms Robot Metacontroller

## What is this metacontroller?

It is known that offline control design methods generate control software during design time, test it on simulation, and then deploy it on real robots. [More about offline design methods](https://www.frontiersin.org/articles/10.3389/frobt.2019.00059/full). It is also worth to note that a control software generated during design time and then deplyoyed, might not be the most appropriate controller all of the time since the conditions of the environment tend to often change, and here a question arise: What about having the possiblities to allow robots to choose the most suitable controller for the given conditions? 


## What is this metacontroller?

This metacontroller demonstrates how to allow robots to adapt their behaviour when some conditions change. It is supposed to be deloyed on each robot in the swarm.

### What is the idea?

This metacontroller uses a pre-generated software control for the robot swarms, store them in a file that will be stored on each robot, and during runtime, it monitors if certain conditions exceed a predefined threshold, and if it is the case, the metacontroller picks the right controller configuration and deploys it on the robot. 

This metacontroller is supposed to easily integrate with any control software, to use it, and choose the right configurations in order to have the most appropriate behaviour for the robots.

## How is it built?

This metacontroller uses:
- [AutoMoDe](https://github.com/demiurge-project/ARGoS3-AutoMoDe) as a robot controller. AutoMoDe is an offline design method that uses a probabilistic finite state machine to design control software. We used [Chocolate](https://iridia.ulb.ac.be/IridiaTrSeries/rev/IridiaTr2014-015r003.pdf) variant.
- [ARGoS3] https://github.com/ilpincy/argos3. Argos3 is a simulator that is used in many fields, including robot swarms, and it is based on c++.
- Dependendencies required for AutoMoDe [dependencies](https://iridia.ulb.ac.be/wiki/Getting_started_for_Demiurge).
- For the loop function, I used the one used in AutoMoDe and I have made modifications to it according the my experiments. You find the modified version [here](https://github.com/FahimaGold/experiments-loop-functions).

## How to install it

- Install the dependencies mentioned earlier and follow the steps to install them [follow these instructions](https://iridia.ulb.ac.be/wiki/Getting_started_for_Demiurge).

- Clone this repository
- `cd SWARMS_METACONTROLLER`
- `mkdir build && cd build`
- `cmake ..`
- `make`

## How to run it

from the `SWARMS_METACONTROLLER` folder run:
`./bin/metacontroller_main -c src/example/example.argos -s 0   --fsm-config --nstates 1 --s0 3`

where `s`is the random seed of argos3, and --fsm-config the FSM configuration. It can be anything respecting the encoded format used in AutoMoDe.


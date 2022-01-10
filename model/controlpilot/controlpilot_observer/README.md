![](img/controlpilot_observer.svg)

## Examples


### ControlPilotObserver remains in a wait state if started.


#### Steps


```javascript
Start state machine ControlPilotObserver.
```

#### Result
```javascript
ControlPilotObserver.Initial- ControlPilotObserver.Wait+ 
```

#### How to run this example from the shell
```bash
ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_1.ceps 
```


### ControlPilotObserver needs a trigger to start/stop  'observing' .


#### Steps


```javascript
Start state machine ControlPilotObserver.
```

```javascript
Trigger Event evControlPilotObserverStart.
```

```javascript
Trigger Event evControlPilotObserverStop.
```

#### Result
```javascript
ControlPilotObserver.Initial- ControlPilotObserver.Wait+ 
ControlPilotObserver.Wait- ControlPilotObserver.Observe+ ControlPilotObserver.Observe.Initial+ 
ControlPilotObserver.Observe.Initial- ControlPilotObserver.Observe.Unknown+ 
ControlPilotObserver.Wait+ ControlPilotObserver.Observe- ControlPilotObserver.Observe.Unknown- 
```

#### How to run this example from the shell
```bash
ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_2.ceps 
```


### ControlPilotObserver 'observes' a state if a sufficient number of equivalent and consecutive measurements are detected.


#### Steps


```javascript
ControlPilotObserver_ExternallyGeneratedSamplingEvents = 1 /*The default behaviour of ControlPilotObserver
is to periodically generate sampling events, we provide these events.*/ 
```

```javascript
ControlPilotObserver_DetectStateThreshold = 2 /*ControlPilotObserver_DetectStateThreshold+1 is the number
of consecutive equivalent measurements required to detect a state.*/ 
```

```javascript
ControlPilotWireMeasure = (ControlPilotStateBMin+as_int(((ControlPilotStateBMax-ControlPilotStateBMin)/2))) /*
Constant voltage signal indicating state B.*/ 
```

```javascript
Start state machine ControlPilotObserver.
```

```javascript
Trigger Event evControlPilotObserverStart.
```

```javascript
Trigger Event evControlPilotObserver_Measure.
```

```javascript
Trigger Event evControlPilotObserver_Measure.
```

```javascript
Trigger Event evControlPilotObserver_Measure.
```

#### Result
```javascript
ControlPilotObserver.Initial- ControlPilotObserver.Wait+ 
ControlPilotObserver.Wait- ControlPilotObserver.Observe+ ControlPilotObserver.Observe.Initial+ 
ControlPilotObserver.Observe.Initial- ControlPilotObserver.Observe.UnknownB+ 
ControlPilotObserver.Observe.UnknownB- ControlPilotObserver.Observe.B+ 
```

#### How to run this example from the shell
```bash
ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_3.ceps 
```


![](img/controlpilot_observer.svg)

## Simulations

ControlPilotObserver_ExternallyGeneratedSamplingEvents = 1<br/>ControlPilotObserver_DetectStateThreshold = 2<br/>ControlPilotWireMeasure = ControlPilotStateBMin+as_int(ControlPilotStateBMax\-ControlPilotStateBMin/2)<br/>
### [Simulation] ControlPilotObserver 'observes' a state.

#### Steps
&nbsp;`Start` state machine *ControlPilotObserver* .<br/>&nbsp;`Trigger Event` *evControlPilotObserverStart* .<br/>&nbsp;`Trigger Event` *evControlPilotObserver_Measure* .<br/>&nbsp;`Trigger Event` *evControlPilotObserver_Measure* .<br/>&nbsp;`Trigger Event` *evControlPilotObserver_Measure* .<br/>
#### Result
```javascript
ControlPilotObserver.Initial- ControlPilotObserver.Wait+ 
ControlPilotObserver.Wait- ControlPilotObserver.Observe+ ControlPilotObserver.Observe.Initial+ 
ControlPilotObserver.Observe.Initial- ControlPilotObserver.Observe.UnknownB+ 
ControlPilotObserver.Observe.UnknownB- ControlPilotObserver.Observe.B+ 
```

#### How to run this simulation from the shell
```bash
ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_1.ceps 
```


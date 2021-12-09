# v2g-guru-slac
SLAC (Signal Level Attenuation Characterization) simulation and implementation according to ISO 15118-3

## What this project is about
- A clean, comprehensive, and __executable__ specification of the EV/EVSE matching process as described in ISO 15118-3. 
- A portable, robust and correct implementation in idiomatic C++20.

### SLAC Model
The project includes an executable SLAC model (simulation) which needs https://github.com/cepsdev/machines4ceps.git as runtime environment - any unixish OS will run the simulation. SLAC MMEs are directly supported, i.e. the model operates with real SLAC messages. However, SLAC messages are send/received via an SCTP connection (That's TCP with preserved message boundaries - essentially) this approach has major benefits, e.g. the SLAC model can run as a cloud service, but it requires a tiny little bit of effort to make it work with real hardware (the details of such a 'routing' mechanism are straightforward and depend on your particular situation). Hence the sctp user-space libraries are required, although sctp is part of the Linux Kernel since at least version 2.6 the required libraries for user-space applications are not a default part of most distributions. For Ubuntu derived Linux distributions the sctp user libraries can be obtained by installing the package libsctp-dev (e.g. for Ubuntu execute the following command with elevated privileges: # apt install libsctp-dev). 

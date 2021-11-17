# v2g-guru-slac
SLAC implementation according to ISO 15118-3

## Requirements
### Tests
The project includes a SLAC simulation for tests that relies on https://github.com/cepsdev/machines4ceps.git. Furthermore, the sctp user-space libraries are required, although sctp is part of the Linux Kernel since at least version 2.6 the required libraries for user-space applications are not a default part of most distributions. For Ubuntu derived Linux distributions the sctp user libraries can be obtained by installing the package libsctp-dev (e.g. for Ubuntu execute the following command with elevated privileges: # apt install libsctp-dev). 

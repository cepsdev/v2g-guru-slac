#!/bin/bash
mv README.md README.old.md -f  2> /dev/null
ceps ../../common.ceps ../controlpilot_interface.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps --dot_gen && dot out.dot -Tsvg -o controlpilot_observer.svg;
PIC="controlpilot_observer.svg"
cat >> README.md << EOF
![]($PIC)
EOF
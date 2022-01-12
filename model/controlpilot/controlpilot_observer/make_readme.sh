#!/bin/bash
rm README.md
ceps ../../common.ceps ../controlpilot_interface.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps --dot_gen && dot out.dot -Tsvg -o img/controlpilot_observer.svg;
rm out.dot
PIC="img/controlpilot_observer.svg"
cat >> README.md << EOF
![]($PIC)

## Examples

EOF
ceps readme_scenario_1.ceps --pe --format markdown_github --ignore_simulations >> README.md
cat >> README.md << EOF

#### Result
\`\`\`javascript
EOF

ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_1.ceps >> README.md 
cat >> README.md << EOF
\`\`\`
EOF

cat >> README.md << EOF

#### How to run this example from the shell
\`\`\`bash
ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_1.ceps 
\`\`\`

EOF


ceps readme_scenario_2.ceps --pe --format markdown_github --ignore_simulations >> README.md
cat >> README.md << EOF

#### Result
\`\`\`javascript
EOF

ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_2.ceps >> README.md 
cat >> README.md << EOF
\`\`\`
EOF

cat >> README.md << EOF

#### How to run this example from the shell
\`\`\`bash
ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_2.ceps 
\`\`\`

EOF


ceps readme_scenario_3.ceps --pe --format markdown_github --ignore_simulations >> README.md
cat >> README.md << EOF

#### Result
\`\`\`javascript
EOF

ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_3.ceps >> README.md 
cat >> README.md << EOF
\`\`\`
EOF

cat >> README.md << EOF

#### How to run this example from the shell
\`\`\`bash
ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_3.ceps 
\`\`\`

EOF

####### scenario 4a/b

ceps ../../common.ceps ../controlpilot_interface.ceps controlpilot_observer_interface.ceps readme_scenario_4a.ceps --dot_gen && dot out.dot -Tsvg -o img/readme_scenario_4a.svg;
rm out.dot
PIC="img/readme_scenario_4a.svg"
cat >> README.md << EOF
### A helper state machine

![]($PIC)

The depicted state machine serves us as a switch, which sets the measured voltage on the control pilot to a 'state C' compatible voltage level. 
The mechanics is simple: the state machine has a single transition with the sole purpose to trigger the action 
\`doSetControlPilotWireVoltage\` whenever the event evSetControlPilotStateToC has occured.
EOF
ceps readme_scenario_4b.ceps --pe --format markdown_github --ignore_simulations >> README.md
cat >> README.md << EOF

#### Result
\`\`\`javascript
EOF

ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_4a.ceps readme_scenario_4b.ceps >> README.md 
cat >> README.md << EOF
\`\`\`
EOF

cat >> README.md << EOF

#### How to run this example from the shell
\`\`\`bash
ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_4a.ceps readme_scenario_4b.ceps >> README.md 
\`\`\`

EOF



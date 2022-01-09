#!/bin/bash
rm README.md
ceps ../../common.ceps ../controlpilot_interface.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps --dot_gen && dot out.dot -Tsvg -o img/controlpilot_observer.svg;
rm out.dot
PIC="img/controlpilot_observer.svg"
cat >> README.md << EOF
![]($PIC)

## Simulations

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

#### How to run this simulation from the shell
\`\`\`bash
ceps ../../common.ceps ../../timing.ceps ../controlpilot_interface.ceps ../controlpilot.ceps controlpilot_observer_interface.ceps controlpilot_observer.ceps readme_scenario_1.ceps 
\`\`\`

EOF


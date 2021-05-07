// -----------------------------------------------------------------------------
// RPN RULES
// -----------------------------------------------------------------------------

#ifndef RPN_RULES_SUPPORT
#define RPN_RULES_SUPPORT           0               // Enable RPN Rules (8.6Kb)
#endif

#ifndef RPN_DELAY
#define RPN_DELAY                   100             // Execute rules after 100ms without messages
#endif

void _rpnDump();
void rpnSetup();
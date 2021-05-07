#include <Arduino.h>

void otaPrintError();
bool otaFinalize(size_t size, int reason, bool evenIfRemaining = false);

// Helper methods from UpdaterClass that need to be called manually for async mode,
// because we are not using Stream interface to feed it data.
bool otaVerifyHeader(uint8_t* data, size_t len);

void otaProgress(size_t bytes, size_t each = 8192u);
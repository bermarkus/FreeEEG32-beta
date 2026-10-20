#ifndef FREEEEG32_FIRMWARE_IDENTITY_H
#define FREEEEG32_FIRMWARE_IDENTITY_H
#define FREEEEG32_VERSION "qos-5"
/* build.py sets a fingerprint of the prepared sources, compiler and build flags. */
#ifndef FREEEEG32_BUILD_ID
#define FREEEEG32_BUILD_ID "development"
#endif
#define FREEEEG32_BUILD_TAG "FreeEEG32-build:" FREEEEG32_BUILD_ID
#endif

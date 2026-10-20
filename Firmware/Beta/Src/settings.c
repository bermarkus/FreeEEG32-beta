#include "settings.h"
#include <string.h>
#define SETTINGS_MAGIC 0x33474643u

static uint32_t crc32(const void *data, unsigned length)
{
    const uint8_t *p = data;
    uint32_t crc = ~0u;
    unsigned bit;
    while (length--) {
        crc ^= *p++;
        for (bit = 0; bit < 8; ++bit) crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1u)));
    }
    return ~crc;
}

void settings_defaults(eeg_settings *s)
{ s->rate = 512; memset(s->gains, 8, sizeof(s->gains)); }

static int valid_settings(const eeg_settings *s)
{
    unsigned ch;
    if (s->rate != 512 && s->rate != 1024 && s->rate != 2048) return 0;
    for (ch = 0; ch < 32; ++ch)
        if (s->gains[ch] != 1 && s->gains[ch] != 2 && s->gains[ch] != 4 && s->gains[ch] != 8) return 0;
    return 1;
}

/* One independently checksummed 256-bit flash word per explicit SAVE. Never
 * rewrite an ECC word, even a torn/invalid record. Later valid records win.
 */
static int scan(eeg_settings *s, uint32_t *next, uint32_t *sequence)
{
    uint32_t offset, words[8];
    int found = 0;
    settings_defaults(s); *next = 0; *sequence = 0;
    for (offset = 0; offset < SETTINGS_BYTES; offset += 32) {
        unsigned i, blank = 1;
        if (!settings_read(offset, words)) { *next = offset + 32; continue; }
        for (i = 0; i < 8; ++i) if (words[i] != ~0u) blank = 0;
        if (blank) continue;
        *next = offset + 32;
        if (words[0] != SETTINGS_MAGIC || words[1] != 1 ||
            words[6] != crc32(words, 24) || words[7] != ~words[6] ||
            (words[3] != 512 && words[3] != 1024 && words[3] != 2048)) continue;
        s->rate = words[3];
        for (i = 0; i < 32; ++i) s->gains[i] = (uint8_t)(1u << ((words[4 + i / 16] >> (2 * (i % 16))) & 3));
        *sequence = words[2]; found = 1;
    }
    return found;
}

int settings_load(eeg_settings *s)
{ uint32_t next, sequence; return scan(s, &next, &sequence); }

int settings_save(const eeg_settings *s)
{
    eeg_settings old;
    uint32_t next, sequence, words[8] = {SETTINGS_MAGIC, 1, 0, 0, 0, 0, 0, 0}, check[8];
    unsigned i;
    int found;
    if (!valid_settings(s)) return 0;
    found = scan(&old, &next, &sequence);
    if (found && old.rate == s->rate && !memcmp(old.gains, s->gains, 32)) return 1;
    /* Keep existing settings if the journal fills. FACTORY explicitly recycles it. */
    if (next >= SETTINGS_BYTES) return -1;
    words[2] = sequence + 1; words[3] = s->rate;
    for (i = 0; i < 32; ++i) {
        unsigned code = s->gains[i] == 8 ? 3 : s->gains[i] == 4 ? 2 : s->gains[i] == 2 ? 1 : 0;
        words[4 + i / 16] |= code << (2 * (i % 16));
    }
    words[6] = crc32(words, 24); words[7] = ~words[6];
    return settings_program(next, words) && settings_read(next, check) && !memcmp(words, check, 32);
}

int settings_factory(void)
{
    eeg_settings defaults;
    settings_defaults(&defaults);
    return settings_erase() && settings_save(&defaults) == 1;
}

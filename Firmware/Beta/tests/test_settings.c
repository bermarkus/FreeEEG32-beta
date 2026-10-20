#include "settings.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint8_t memory[SETTINGS_BYTES], used[SETTINGS_BYTES / 32], bad[SETTINGS_BYTES / 32];
static unsigned writes, torn, erase_fail;
int settings_read(uint32_t offset, uint32_t record[8])
{ assert(offset < SETTINGS_BYTES && !(offset & 31)); if (bad[offset/32]) return 0; memcpy(record, memory+offset, 32); return 1; }
int settings_program(uint32_t offset, const uint32_t record[8])
{
    assert(offset < SETTINGS_BYTES && !(offset & 31) && !used[offset/32]);
    used[offset/32] = 1; ++writes;
    if (torn) { memcpy(memory+offset, record, 16); bad[offset/32] = 1; return 0; }
    memcpy(memory+offset, record, 32); return 1;
}
int settings_erase(void)
{ if (erase_fail) return 0; memset(memory, 255, sizeof(memory)); memset(used, 0, sizeof(used)); memset(bad, 0, sizeof(bad)); return 1; }
int main(void)
{
    eeg_settings s, loaded;
    unsigned i;
    settings_erase(); assert(!settings_load(&loaded) && loaded.rate == 512 && loaded.gains[31] == 8);
    settings_defaults(&s); s.rate = 2048; s.gains[31] = 2;
    assert(settings_save(&s) == 1 && writes == 1);
    assert(settings_save(&s) == 1 && writes == 1); /* Identical save causes no wear. */
    assert(settings_load(&loaded) && loaded.rate == 2048 && loaded.gains[31] == 2);
    /* A torn ECC word is skipped forever; the previous valid settings survive. */
    s.rate = 1024; torn = 1; assert(settings_save(&s) == 0); torn = 0;
    assert(settings_load(&loaded) && loaded.rate == 2048);
    assert(settings_save(&s) == 1 && used[2]);
    assert(settings_load(&loaded) && loaded.rate == 1024);
    memory[64+12] ^= 1; assert(settings_load(&loaded) && loaded.rate == 2048); /* CRC fallback. */
    s.rate = 256; assert(settings_save(&s) == 0);
    s.rate = 512; s.gains[0] = 3; assert(settings_save(&s) == 0); s.gains[0] = 8;
    for (i = 3; i < SETTINGS_BYTES/32; ++i) { s.rate = (i & 1) ? 512 : 1024; assert(settings_save(&s) == 1); }
    s.rate = 2048; assert(settings_save(&s) == -1); /* Never auto-erase last valid settings. */
    erase_fail = 1; assert(!settings_factory()); assert(settings_load(&loaded));
    erase_fail = 0; assert(settings_factory());
    assert(settings_load(&loaded) && loaded.rate == 512 && loaded.gains[31] == 8);
    puts("Settings simulation passed: defaults, all gains, explicit save/no-op, torn ECC/CRC recovery, bounds/full journal, factory reset.");
    return 0;
}

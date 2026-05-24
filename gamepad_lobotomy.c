#include "util.h"
#include <stdio.h>
#include <string.h>

__attribute__((constructor))
void gamepad_lobotomizer() {
    char comm[256] = {0};
    FILE *f = fopen("/proc/self/comm", "r");
    if (f) {
        fgets(comm, sizeof(comm), f);
        fclose(f);
    }
    
    if (strstr(comm, "metro") == NULL) {
        return; 
    }

    fprintf(stderr, "[srpfix] Preparing gamepad lobotomy...\n");

    uintptr_t target_addr = 0x00D43BB0;
    uint8_t patch_payload[] = { 0x31, 0xC0, 0xC3 }; // xor eax, eax | ret

    schedule_memory_patch_for_steam(target_addr, patch_payload, sizeof(patch_payload), 3, "gamepad lobotomy");
}

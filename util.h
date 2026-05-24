#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    uintptr_t target_addr;
    uint8_t *patch_bytes;
    size_t patch_len;
    int delay_seconds;
    char* reason;
} patch_config_t;

static void* _patch_thread_worker(void* arg) {
    patch_config_t* config = (patch_config_t*)arg;
    
    sleep(config->delay_seconds);

    size_t page_size = sysconf(_SC_PAGE_SIZE);
    uintptr_t page_start = config->target_addr & ~(page_size - 1);

    // Unlock, write, and re-lock
    if (mprotect((void*)page_start, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == 0) {
        uint8_t *dest = (uint8_t*)config->target_addr;
        for (size_t i = 0; i < config->patch_len; i++) {
            dest[i] = config->patch_bytes[i];
        }
        fprintf(stderr, "[srpfix] Applied %zu bytes at 0x%lX for %s.\n", config->patch_len, config->target_addr, config->reason);
        mprotect((void*)page_start, page_size, PROT_READ | PROT_EXEC);
    } else {
        fprintf(stderr, "[srpfix] srpfix: Failed to unprotect memory page for %s!\n", config->reason);
    }

    free(config->patch_bytes);
    free(config);
    return NULL;
}


static inline void schedule_memory_patch_for_steam(uintptr_t address, const uint8_t* bytes, size_t length, int delay_seconds, char* reason) {
    patch_config_t* config = (patch_config_t*)malloc(sizeof(patch_config_t));
    config->target_addr = address;
    config->patch_len = length;
    config->delay_seconds = delay_seconds;
    config->reason = reason;

    // Deep copy the bytes so the caller doesn't have to keep them in scope
    config->patch_bytes = (uint8_t*)malloc(length);
    memcpy(config->patch_bytes, bytes, length);

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, _patch_thread_worker, config);
    pthread_detach(thread_id);
}

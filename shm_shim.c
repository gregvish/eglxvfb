#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>


#define SHM_PATH "/tmp/shm"
#define PAGE_SIZE (4096)
#define PATH_SIZE (4096)


int shmget(key_t key, size_t size, int shmflg)
{
    char path[PATH_SIZE] = {0};
    int fd = 0;
    int flags = O_RDWR;
    bool private = false;
    struct stat st = {0};
    size_t cur_size = 0;

    flags |= (shmflg & IPC_CREAT) ? O_CREAT : 0;
    flags |= (shmflg & IPC_EXCL) ? O_EXCL : 0;

    if (key == IPC_PRIVATE) {
        key = rand();
        private = true;
    }

    mkdir(SHM_PATH, 0777);
    snprintf(path, sizeof(path), "%s/%d-%d", SHM_PATH, private, key);
    fd = open(path, flags, 0644);

    if (fd < 0) {
        return -1;
    }

    stat(path, &st);
    cur_size = st.st_size;

    if (cur_size == 0) {
        size = (1 + (size / PAGE_SIZE)) * PAGE_SIZE;
        if (ftruncate(fd, size) < 0) {
            return -1;
        }
    }

    return fd;
}


static bool get_path(int fd, char *path)
{
    char fd_path[PATH_SIZE] = {0};

    snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);

    if (readlink(fd_path, path, PATH_SIZE) < 1) {
        return false;
    }

    return true;
}


int shmctl(int shmid, int cmd, struct shmid_ds *buf)
{
    char path[PATH_SIZE] = {0};

    if (!get_path(shmid, path)) {
        return -1;
    }

    if (cmd == IPC_RMID) {
        unlink(path);
        // TODO close?

    } else {
        fprintf(stderr, "shm_shim unsupported shmctl cmd: %d\n", cmd);
        return -1;
    }

    return 0;
}


void *shmat(int shmid, const void *shmaddr, int shmflg)
{
    struct stat st = {0};
    char path[PATH_SIZE] = {0};
    size_t size = 0;
    int prot = 0;
    void *addr = NULL;

    if (!get_path(shmid, path)) {
        return NULL;
    }

    stat(path, &st);
    size = st.st_size;

    prot |= (shmflg & SHM_EXEC) ? PROT_EXEC : 0;
    prot |= (shmflg & SHM_RDONLY) ? 0 : PROT_WRITE;

    addr = mmap((void *)shmaddr, size, PROT_READ | prot, MAP_SHARED, shmid, 0);
    if (!addr) {
        return (void *)-1;
    }

    return addr;
}


int shmdt(const void *shmaddr)
{
    char cmd[1024] = {0};
    FILE *maps = NULL;
    uint64_t start_addr = (uint64_t)shmaddr;
    uint64_t end_addr = 0;

    snprintf(cmd, sizeof(cmd),
             "cat /proc/self/maps | grep -o '%016lx-[a-fA-F0-9]*'", start_addr);

    maps = popen(cmd, "r");
    if (!maps) {
        printf("failed to open proc self maps\n");
        return -1;
    }

    // TODO what if empty
    fscanf(maps, "%lx-%lx\n", &start_addr, &end_addr);
    fclose(maps);

    return munmap((void *)shmaddr, end_addr - start_addr);
}


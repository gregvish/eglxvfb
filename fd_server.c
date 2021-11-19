#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/fcntl.h>


typedef void* (serv_func_t)(void* fd);
extern serv_func_t xdamage_main;
extern serv_func_t xtest_main;


int create_abstract_uds(const char *path)
{
    struct sockaddr_un addr = {0};
    uint16_t addrlen = 0;

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd < 0) {
        fprintf(stderr, "socket fail\n");
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path + 1, path, sizeof(addr.sun_path) - 2);
    // Abstract namespace (first path byte is NULL, and addrlen must be shortened)
    addr.sun_path[0] = 0;
    addrlen = sizeof(addr) - (sizeof(addr.sun_path) - strlen(addr.sun_path + 1) - 1);

    if (bind(sfd, (struct sockaddr *)&addr, addrlen) == -1) {
        fprintf(stderr, "bind fail\n");
        return -1;
    }

    if (listen(sfd, 5) == -1) {
        fprintf(stderr, "listen fail\n");
        return -1;
    }

    return sfd;
}


bool write_fd_to_uds(int sock, int fd, void *data, size_t data_len)
{
    struct msghdr msg = {0};
    char buf[CMSG_SPACE(sizeof(fd))] = {0};
    struct iovec io = {.iov_base = data, .iov_len = data_len};
    struct cmsghdr *cmsg = NULL;

    memset(buf, '\0', sizeof(buf));
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd));

    memmove(CMSG_DATA(cmsg), &fd, sizeof(fd));
    msg.msg_controllen = CMSG_SPACE(sizeof(fd));

    if (sendmsg(sock, &msg, 0) < 0)
    {
        fprintf(stderr, "Failed to sendmsg\n");
        return false;
    }

    return true;
}


int run_client_thread(serv_func_t *func)
{
    pthread_t gl_thread = 0;
    int fds[2] = {0};

    if (socketpair(PF_LOCAL, SOCK_STREAM, 0, fds) == -1) {
        fprintf(stderr, "socketpair fail\n");
        return -1;
    }

    if (pthread_create(&gl_thread, NULL, func, (void *)(long)fds[0])) {
        fprintf(stderr, "pthread_create fail\n");
        return -1;
    }

    return fds[1];
}


int main(int argc, const char *argv[])
{
    int sock_fd = -1;
    int client_fd = -1;
    int shm_fd = -1;
    int xtest_fd = -1;
    int xdamage_fd = -1;

    if (argc < 2) {
        fprintf(stderr, "Usage ./mem_fd <mmap_file_path> <abstract_path>\n");
        return 1;
    }

    shm_fd = open(argv[1], O_RDONLY);
    if (shm_fd < 0) {
        fprintf(stderr, "Failed opening shared memory file\n");
        return 1;
    }

    sock_fd = create_abstract_uds(argv[2]);
    if (sock_fd < 0) {
        fprintf(stderr, "Failed creating UDS\n");
        return 1;
    }

    while (true) {
        client_fd = accept(sock_fd, NULL, NULL);
        if (client_fd < 0) {
            fprintf(stderr, "accept fail\n");
            return 1;
        }

        xtest_fd = run_client_thread(xtest_main);
        xdamage_fd = run_client_thread(xdamage_main);
        if (xtest_fd < 0 || xdamage_fd < 0) {
            fprintf(stderr, "failed to run xtest or xdamage threads\n");
            return 1;
        }

        if (!write_fd_to_uds(client_fd, shm_fd, "x", 1) ||
            !write_fd_to_uds(client_fd, xtest_fd, "x", 1) ||
            !write_fd_to_uds(client_fd, xdamage_fd, "x", 1)) {
            return 1;
        }

        close(client_fd);
        close(xtest_fd);
        close(xdamage_fd);
    }

    return 0;
}

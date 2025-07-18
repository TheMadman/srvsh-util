#include <srvsh.h>
#include <poll.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <stdlib.h>

typedef struct pollfd pollfd_t;

static int next_fd(const int current_fd, const int total_count)
{
	/*
	 * If the current_fd is the last client file descriptor,
	 * circle back around to the server file descriptor.
	 */
	return ((current_fd + 1 - SRV_FILENO) % total_count) + SRV_FILENO;
}

int main()
{
	const int clients = cli_count();
	if (clients < 0)
		return 1;

	// clients + server
	pollfd_t *fds = calloc(clients + 1, sizeof(*fds));
	if (!fds)
		return 1;

	const int total = srvcli_polls(fds, clients + 1);
	if (total < 0)
		return 1;

	const int forever = -1;
	while (poll(fds, total, forever) > 0) {
		for (const pollfd_t *current = fds; current < &fds[total]; current++) {
			if (current->revents & POLLIN) {
				char buf[1024] = { 0 };
				char controlbuf[1024] = { 0 };

				struct iovec data = {
					.iov_base = buf,
					.iov_len = sizeof(buf)
				};
				struct msghdr msg = {
					.msg_iov = &data,
					.msg_iovlen = 1,
					.msg_control = controlbuf,
					.msg_controllen = sizeof(controlbuf),
				};

				ssize_t result = recvmsg(current->fd, &msg, 0);
				if (result < 0)
					return 1;

				data.iov_len = (size_t)result;

				if (sendmsg(next_fd(current->fd, total), &msg, 0) < 0)
					return 1;
			} else if (current->revents & POLLHUP) {
				return 0;
			} else if (
				current->revents & POLLERR
				|| current->revents & POLLNVAL
			) {
				return 1;
			}
		}
	}
}

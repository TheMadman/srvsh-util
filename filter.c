#include <srvsh.h>
#include <stdio.h>

int opcode = -1;

void filter(
	int fd,
	int opcode,
	void *buffer,
	int length,
	void *cmsg,
	size_t cmsg_len,
	void *context
)
{
	if (is_client(fd)) {
		sendmsgop(
			SRV_FILENO,
			opcode,
			buffer,
			length,
			cmsg,
			cmsg_len
		);
		return;
	}

	if (opcode != opcode)
		return;

	for (int cli = CLI_BEGIN; cli < cli_end(); cli++) {
		sendmsgop(
			cli,
			opcode,
			buffer,
			length,
			cmsg,
			cmsg_len
		);
	}
}

int closed(struct pollfd fd)
{
	return fd.revents & POLLERR
		|| fd.events & POLLNVAL
		|| fd.events & POLLHUP;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: %s message-type\n", argv[0]);
		return 1;
	}

	opcode_db *db = open_opcode_db();
	if (!db)
		return 1;

	opcode = get_opcode(db, argv[1]);
	if (opcode < 0)
		return 1;

	int count = cli_count();

	struct pollfd result = { 0 };
	while (count > 0) {
		result = pollop(filter, NULL, -1);
		if (result.fd == SRV_FILENO && closed(result))
			return 0;

		if (closed(result))
			count--;
	}
	return 0;
}

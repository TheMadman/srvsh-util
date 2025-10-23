#include <sys/socket.h>

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <srvsh.h>
#include <stdio.h>

int cmd_opcode = -1;

void filter(
	int fd,
	int opcode,
	void *buffer,
	int length,
	struct msghdr header,
	void *context
)
{
	if (is_cli(fd)) {
		sendmsgop(
			SRV_FILENO,
			opcode,
			buffer,
			length,
			header.msg_control,
			header.msg_controllen
		);
		close_cmsg_fds(header);
		return;
	}

	if (opcode != cmd_opcode) {
		close_cmsg_fds(header);
		return;
	}

	for (int cli = CLI_BEGIN; cli < cli_end(); cli++) {
		sendmsgop(
			cli,
			opcode,
			buffer,
			length,
			header.msg_control,
			header.msg_controllen
		);
		close_cmsg_fds(header);
	}
}

int closed(struct pollfd fd)
{
	return !(fd.revents & POLLIN);
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

	cmd_opcode = get_opcode(db, argv[1]);
	if (cmd_opcode < 0)
		return 1;

	int count = cli_count();

	struct pollfd result = { 0 };
	while (count > 0) {
		result = pollop(filter, NULL, -1);

		if (result.fd < 0)
			return 1;

		if (result.fd == SRV_FILENO && closed(result))
			return 0;
		else if (closed(result))
			count--;
	}
	return 0;
}

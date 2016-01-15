#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_MESSAGE_SIZE (65535)
#define NETLINK_USER (31)

int get_reports(char *buffer);

int main(int argc, char **argv) {

	FILE *output = NULL; 
	char recv_buffer[MAX_MESSAGE_SIZE] = {0};

	if (2 != argc) {
		fprintf(stderr, "Usage: %s <output file>\n", argv[0]);
		return -1;
	}

	output = fopen(argv[1], "w");

	if (0 > get_reports(recv_buffer)) {	
		return -1;
	}

	fwrite(recv_buffer, 1, MAX_MESSAGE_SIZE, output);
	fclose(output);
	return 0;

}

int get_reports(char *buffer) {

	struct sockaddr_nl src_addr = {0};
	struct sockaddr_nl dst_addr = {0};
	struct nlmsghdr *nlh = NULL;
	struct iovec iov = {0};
	int sock_fd = 0;
	struct msghdr msg = {0};
	int size = 0;
	const char *message_to_kernel = "hush_pass!";

	sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
	if (0 > sock_fd) {
		fprintf(stderr, "Can't create socket! ERR:[%s]\n", strerror(errno));
		return -1;
	}

	// memset(&src_addr, 0, sizeof(struct sockaddr_nl));
	// memset(&dst_addr, 0, sizeof(struct sockaddr_nl));
	/* Set source */
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid();

	bind(sock_fd, (struct sockaddr *) &src_addr, sizeof(struct sockaddr_nl));

	/* Set destination kernel */
	dst_addr.nl_family = AF_NETLINK;
	dst_addr.nl_pid = 0;
	dst_addr.nl_groups = 0;

	/* Set message */
	nlh = (struct nlmsghdr *) malloc(NLMSG_SPACE(MAX_MESSAGE_SIZE));
	memset(nlh, 0 , NLMSG_SPACE(MAX_MESSAGE_SIZE));
	nlh->nlmsg_len = NLMSG_SPACE(MAX_MESSAGE_SIZE);
	nlh->nlmsg_pid = getpid();

	strncpy(NLMSG_DATA(nlh), message_to_kernel, 11);

	/* Construct message */
	iov.iov_base = (void *) nlh;
	iov.iov_len = nlh->nlmsg_len;
	msg.msg_name = (void *) &dst_addr;
	msg.msg_namelen = sizeof(struct msghdr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	/* Letting kernel know our PID */
	sendmsg(sock_fd, &msg, 0);

	memset(NLMSG_DATA(nlh), 0, NLMSG_PAYLOAD(nlh, size));

	recvmsg(sock_fd, &msg, 0);

	if (NLMSG_DONE != nlh->nlmsg_type) {
		fprintf(stderr, "Err: Problem reading from netlink\n");
		return -2;
	}

	if (NLMSG_PAYLOAD(nlh, size) > MAX_MESSAGE_SIZE) {
		fprintf(stderr, "Err: Message recieved from netlink larger than buffer\nbuffer size: %d netlink: %lu \n", MAX_MESSAGE_SIZE, nlh->nlmsg_len - sizeof(struct nlmsghdr));
		return -3;
	}

	memcpy(buffer, NLMSG_DATA(nlh), NLMSG_PAYLOAD(nlh, size));
	close(sock_fd);
	return 0;
}
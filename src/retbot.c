#include "retbot.h"

struct sock *netlink_socket = NULL;
#define NETLINK_USER (31)

unsigned long long RETBOT_up_time = 0;
EXPORT_SYMBOL( RETBOT_up_time );

static void __init RETBOT_load_module( void ) { 

	unsigned long **table = NULL;
	struct timeval t = {0};
	struct netlink_kernel_cfg n_config = {0};

	do_gettimeofday( &t );
	RETBOT_up_time = t.tv_usec;

 	proc_create( PROC_NAME, 0, NULL, &RETBOT_proc_info );

 	table = COLLECTOR_find_table();

 	if (NULL != table) {
 		COLLECTOR_hook_open( table );
 	}

 	/* Config netlink */
 	n_config.input = RETBOT_netlink_reciever;

 	netlink_socket = netlink_kernel_create(  &init_net, NETLINK_USER, &n_config );
 	
 	printk( "Loaded!\n" );
}

static void __exit RETBOT_unload_module( void ) {

	unsigned long **table = NULL;

	table = COLLECTOR_find_table();

	if (NULL != table){
		COLLECTOR_unhook_open( table );
	}

	remove_proc_entry( PROC_NAME, NULL );

	netlink_kernel_release( netlink_socket );

	printk( "Unloaded!\n" );
}

int RETBOT_open_proc( struct inode *inode, struct file *file ) {

	return single_open( file, RETBOT_print_logs, NULL );
}

int RETBOT_print_logs( struct seq_file *m, void *v ) {

	seq_printf( m, "uptime: %llu\nfiles opened: %llu\n", RETBOT_up_time, COLLECTOR_get_files_opened() );
	RECORDER_clean_buffer();
	return 0;
}

static void RETBOT_netlink_reciever( struct sk_buff *sk ) {

	struct nlmsghdr *nlh = NULL;
	int pid = 0; 
	struct sk_buff *skb_out = NULL;
	unsigned int msg_size  = 0;

	skb_out = nlmsg_new( RECORDER_get_buffer_size(), NULL );
	msg_size = RECORDER_get_buffer_size();

	nlh = (struct nlmsghdr *)sk->data;
	pid = nlh->nlmsg_pid;

	if (NULL == skb_out) {
		printk("Err: can't create netlink socket\n");
		return;
	}

	nlh = nlmsg_put( skb_out, 0, 0, NLMSG_DONE, msg_size, 0 );
	NETLINK_CB( skb_out ).dst_group = 0;
	memcpy( nlmsg_data(nlh), RECORDER_get_buffer(), RECORDER_get_buffer_size() );
	printk("%s", nlmsg_data(nlh));

	if (nlmsg_unicast( netlink_socket, skb_out, pid ) < 0) {
		printk("Err: can't send over netlink socket\n");
	}
	else {
		RECORDER_clean_buffer();
	}
}

module_init( RETBOT_load_module );
module_exit( RETBOT_unload_module );

MODULE_LICENSE( "GPL" );
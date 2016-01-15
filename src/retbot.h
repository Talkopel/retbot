#ifndef RETBOT_H
#define RETBOT_H

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <net/sock.h>

#include "collector.h"
#include "recorder.h"

#define CYCLE_BUFFER_SIZE 65355
#define LOG_MESSAGE_MAX_SIZE 2048

#define PROC_NAME "retbot_logs"

/* @brief: start module - create proc, start collecting data */
static void __init RETBOT_load_module( void );
/* @brief: unload resources */
static void __exit RETBOT_unload_module( void );

/* @brief: open proc file for reading */
int RETBOT_open_proc( struct inode *inode, struct file *file );

/* @brief: write data to proc */
int RETBOT_print_logs( struct seq_file *m, void *v );

/* @brief: connect function from netlink */
static void RETBOT_netlink_reciever( struct sk_buff *sk );

const struct file_operations RETBOT_proc_info = {
    .owner      = THIS_MODULE,
    .open       = RETBOT_open_proc,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

#endif
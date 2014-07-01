#ifndef __PKT_ENGINE_H__
#define __PKT_ENGINE_H__
/*---------------------------------------------------------------------*/
/* for data types */
#include <stdint.h>
/* for queue management */
#include "queue.h"
/* for io_module_funcs */
#include "io_module.h"
/* for pthreads */
#include <pthread.h>
/*---------------------------------------------------------------------*/
/**
 *  io_type: Right now, we only support IO_NETMAP.
 */
typedef enum io_type {
	IO_NETMAP, IO_DPDK, IO_PFRING, IO_PSIO, IO_LINUX, IO_FILE
} io_type;
/* the default is set to IO_NETMAP */
#define IO_DEFAULT		IO_NETMAP
/*---------------------------------------------------------------------*/
/**
 *
 * PACKET ENGINE INTERFACE
 *
 * This is a template for the packet engine. More comments will be added
 * in the future... as the interface is matured further.
 *
 */
/*---------------------------------------------------------------------*/
typedef struct engine {
	uint8_t run; 			/* the engine mode running/stopped */
	io_type iot;			/* type: currently only supports netmap */
	uint8_t *name;			/* the engine name will be used as an identifier */
	int8_t cpu;			/* the engine thread will be affinitized to this cpu */
	uint64_t byte_count;		/* total number of bytes seen by this engine */
	uint64_t pkt_count;		/* total number of packets seen by this engine */

	struct io_module_funcs iom;	/* io_funcs ptrs */
	void *private_context;		/* I/O-related context */
	pthread_t t;

	/* the linked list ptr that will chain together all the engines (for networkin_interface.c) */
	TAILQ_ENTRY(engine) if_entry; 

	/* the linked list ptr that will chain together all the engines (for pkt_engine.c) */
	TAILQ_ENTRY(engine) entry; 
} engine __attribute__((aligned(__WORDSIZE)));

typedef TAILQ_HEAD(elist, engine) elist;
/*---------------------------------------------------------------------*/
/**
 * Creates a new pkt_engine for packet sniffing
 *
 */
void
pktengine_new(const unsigned char *name, const unsigned char *type, 
		      const int8_t cpu);

/**
 * Deletes the pkt_engine
 *
 */
void
pktengine_delete(const unsigned char *name);

/**
 * Register an iface to the pkt engine
 *
 */
void
pktengine_link_iface(const unsigned char *name, 
		     const unsigned char *iface,
		     const int16_t batch_size);

/**
 * Unregister the iface from the pkt engine
 *
 */
void
pktengine_unlink_iface(const unsigned char *name,
		       const unsigned char *iface);

/**
 * Start the engine
 *
 */
void
pktengine_start(const unsigned char *name);

/**
 * Stop the engine
 */
void
pktengine_stop(const unsigned char *name);

/**
 * Print current traffic statistics
 */
void
pktengine_dump_stats(const unsigned char *name);
				  
/**
 * Initializes the engine module
 */
void
engine_init();
/*---------------------------------------------------------------------*/
#endif /* !__PKT_ENGINE_H__ */

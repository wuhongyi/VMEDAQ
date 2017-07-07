/*
 * include file for babies
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Custom values */
//#define DEBUG     1    // If define DEBUG, shows debug messages 

// Selection of buffer size, w/o HUGEBUFF, LARGEBUFF => 256kB buffer
#define HUGEBUFF    1    // 8MB Buffer
//#define LARGEBUFF    1    // 2MB Buffer

#define BABIESRC  "./babies.rc"  // parameterfile


// Receiver port of babild
#define ERRCVPORT   17601
#define ESCOMPORT   17651  // port will be ESCOMPORT + EFN


// for run status
#define STAT_RUN_IDLE     0
#define STAT_RUN_START    1
#define STAT_RUN_NSSTA    2
#define STAT_RUN_WAITSTOP 3


typedef void (*babies_func)(void);

// Show registered values to check
void babies_check(void);

// Register program name
void babies_name(char *name);


// Register User start function
void babies_start(babies_func start);
void babies_nssta(babies_func nssta);
void babies_stop(babies_func stop);
void babies_reload(babies_func reload);

// Register event loop, generate new thread
void babies_evtloop(babies_func tevtloop);

// Return run status, IDLE = 0, START = 1, NSSTA = 2
int babies_status(void);

// Register quit function (for SIGINT and ES_QUIT)
void babies_quit(babies_func quit);

// Initialize
int babies_init(int efn);

// Main loop
void babies_main(void);

// send data to babild
void babies_flush();

/* RIDF functions */
/** Get evtn
 *  @return evtn event number
 */
int babies_get_evtn(void);

/** Initialize event block
 */
void babies_init_event(void);

void babies_init_eventts(void);
 

/** Initialize segment block
 */
void babies_init_segment(int segid);

/** End of data block */
int babies_end_block();

/** check block size */
int babies_chk_block(int maxbuff);

/** last block should be sent or not */
int babies_last_block(void);

/** End of event block */
int babies_end_event(void);

/** End of event block with Time Stamp*/
int babies_end_eventts(void);

/** End of segment block */
int babies_end_segment(void);

/** initialize scaler block (clear type) */
void babies_init_scaler(int scrid);

/** initialize scaler block (non clear type) */
void babies_init_ncscaler(int scrid);

/** End of scaler (clear type) */
int babies_end_scaler(void);

/** End of non clear scaler */
int babies_end_ncscaler(void);

/** End of non clear scaler */
int babies_end_ncscaler32(void);

/** Copy data into segment, buff = pointer of char, size = char unit*/
int babies_segdata(char *buff, int size);

/** Copy data into scaler buff, buff = pointer of char, size = char unit*/
int babies_scrdata(char *buff, int size);

/** Return pointer of the data buffer and increment 1 short word pointer*/
char *babies_segpt16(void);

/** Return pointer of the data buffer and increment 1 int word pointer*/
char *babies_segpt32(void);

/** Return pointer of the data buffer and increment 1 48bit word pointer*/
char *babies_segpt48(void);

/** Return pointer of the data buffer and increment 1 64bit word pointer*/
char *babies_segpt64(void);


#ifdef __cplusplus
}
#endif

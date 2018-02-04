#ifndef MODBUS_TCP
#define MODBUS_TCP

#if !defined(MODBUS_THREAD_STACKSIZE)
#define MODBUS_THREAD_STACKSIZE           3000
#endif

#if !defined(MODBUS_THREAD_PRIORITY)
#define MODBUS_THREAD_PRIORITY             (LOWPRIO + 2)
#endif

extern THD_WORKING_AREA(modbus_thread_size, MODBUS_THREAD_STACKSIZE);
extern THD_FUNCTION(modbus_server, p);

#endif /* MODBUS_TCP */

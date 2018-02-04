#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>
#include <lwip/sockets.h>
#include "modbus_thread.h"
#include "modbus-tcp.h"
#include "user_input.h"
#include "modbus-private.h"
#include "modbus-tcp-private.h"

#ifndef MSG_NOSIGNAL
# define MSG_NOSIGNAL 0
#endif

THD_WORKING_AREA(modbus_thread_size, MODBUS_THREAD_STACKSIZE);

static void libmodbus() {
     int s = -1;
     modbus_t *ctx;
     modbus_mapping_t *mb_mapping;

     ctx = modbus_new_tcp("192.168.1.10", 1502);
     /* modbus_set_debug(ctx, TRUE); */

     mb_mapping = modbus_mapping_new(0, 0, 50, 0);
     memset(mb_mapping->tab_registers, 40, 25 * sizeof(uint16_t));
     if (mb_mapping == NULL) {
       modbus_free(ctx);
       chThdExit(1);
     }

     for (;;) {
       s = modbus_tcp_listen(ctx, 1);

       if(modbus_tcp_accept(ctx, &s) == -1) {
         close(s);
         continue;
       }
       for(;;) {
          uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
          int rc;
          systime_t now;
          rc = modbus_receive(ctx, query);
          mb_mapping->tab_registers[0] = chVTGetSystemTime()/1000;
          if (rc > 0) {
            /* rc is the query size */
            modbus_reply(ctx, query, rc, mb_mapping);
          } else if (rc == -1) {
            /* Connection closed by the client or error */
            break;
          }
       }
     }

     if (s != -1) {
       close(s);
     }
     modbus_mapping_free(mb_mapping);
     modbus_close(ctx);
     modbus_free(ctx);
}

static void testSocket() {
  int socket_fd,accept_fd,yes;
     volatile int portCustom;
     int addr_size,sent_data;
     char data_buffer[80];
     struct sockaddr_in sa,ra,isa;

     /* Creates an TCP socket (SOCK_STREAM) with Internet Protocol Family (PF_INET).
      * Protocol family and Address family related. For example PF_INET Protocol Family and AF_INET family are coupled.
     */

     socket_fd = socket(PF_INET, SOCK_STREAM, 0);

     if ( socket_fd < 0 )
     {
       chThdExit(0);
     }

     yes = 1;
     portCustom = 1502;
     if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,
                    (char *) &yes, sizeof(yes)) == -1) {
         close(socket_fd);
         chThdExit(0);
     }

     memset(&sa, 0, sizeof(struct sockaddr_in));
     sa.sin_family = AF_INET;
     sa.sin_addr.s_addr = INADDR_ANY;
 //    inet_addr("192.168.1.10");
     sa.sin_port = htons(portCustom);


     /* Bind the TCP socket to the port SENDER_PORT_NUM and to the current
     * machines IP address (Its defined by SENDER_IP_ADDR).
     * Once bind is successful for UDP sockets application can operate
     * on the socket descriptor for sending or receiving data.
     */
     if (bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
     {
       close(socket_fd);
       chThdExit(1);
     }

     listen(socket_fd,1);
     addr_size = sizeof(isa);
     accept_fd = accept(socket_fd, (struct sockaddr*)&isa,&addr_size);
     if(accept_fd < 0)
     {
       close(socket_fd);
       chThdExit(2);
     }
     strcpy(data_buffer,"Hello World robert\n");
     sent_data = send(accept_fd, data_buffer,sizeof("Hello World robert"),0);

     if(sent_data < 0 )
     {
       close(socket_fd);
       chThdExit(3);
     }

     close(socket_fd);
}
THD_FUNCTION(modbus_server, p) {
  libmodbus();
//  testSocket();
}

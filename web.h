
#ifndef __WEB_H__
#define __WEB_H__

// void post(char * code);
void post(char * payload, ip_addr_t ip, int port, const char * method, const char * path);

#endif

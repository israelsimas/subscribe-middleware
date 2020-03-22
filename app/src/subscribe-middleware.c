/**************************************************************************
 *
 * subscribe.c
 *
 *    Application publish 
 * 
 * Copyright 2019
 *
 **************************************************************************/

#include <middleware.h>
#include <subscribe.h>
#include <utils.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define THIS_FILE	"subscribe-middleware.c"

middleware_conn conn = NULL;

int on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg) {
	printf("Topic: %s\n", msg->topic);
  printf("Payload %s\n", (const char *)msg->payload);
  printf("Payloadlen: %d\n\n", msg->payloadlen);
	return 0;
}

void *startMiddlewareCallback(void *pArg) {

  int status;
  
  // Topic generic (receive status/network, status/database etc)
  status = middleware_subscribe_callback(on_message, "status/#", MIDDLEWARE_HOST, MIDDLEWARE_PORT, NULL, NULL);  

  return NULL;
}

int main() {

  int status;
  pthread_t procThrId;

  conn = middleware_open(PUBLISH_ID, MIDDLEWARE_HOST, MIDDLEWARE_PORT, NULL);
  if (!conn) {
    log_error("Invalid connection middleware\n");
    return ERROR;
  }

  status = (int) pthread_create(&procThrId, NULL, startMiddlewareCallback, NULL );
  if (status) {
    log_error("Error to create thread");
  } else {
    pthread_detach(procThrId);
  }  

  log("Loop application ...");

  while (true) {
    sleep(1);
  }

  return SUCCESS;
}

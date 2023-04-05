#include "dds/dds.h"
#include "dds/ddsrt/priorityqueue.h"
#include "HelloWorldData.h"
#include <stdio.h>
#include <stdlib.h>

extern pthread_mutex_t mutex;

void add_pq(priorityQueue pq){
  HelloWorldData_Msg *msg1 = (HelloWorldData_Msg *)malloc(sizeof(HelloWorldData_Msg));//, msg2, msg3, msg4;
  elementStruct *e1 = (elementStruct *)malloc(sizeof(elementStruct));//, e2, e3, e4;
  HelloWorldData_Msg *msg2 = (HelloWorldData_Msg *)malloc(sizeof(HelloWorldData_Msg));//, msg2, msg3, msg4;
  elementStruct *e2 = (elementStruct *)malloc(sizeof(elementStruct));//, e2, e3, e4;
  HelloWorldData_Msg *msg3 = (HelloWorldData_Msg *)malloc(sizeof(HelloWorldData_Msg));//, msg2, msg3, msg4;
  elementStruct *e3 = (elementStruct *)malloc(sizeof(elementStruct));//, e2, e3, e4;
  HelloWorldData_Msg *msg4 = (HelloWorldData_Msg *)malloc(sizeof(HelloWorldData_Msg));//, msg2, msg3, msg4;
  elementStruct *e4 = (elementStruct *)malloc(sizeof(elementStruct));//, e2, e3, e4;

  e1->priority = 27;
  e1->startTime = time(NULL);
  e2->priority = 28;
  e2->startTime = time(NULL);
  e3->priority = 26;
  e3->startTime = time(NULL);
  e4->priority = 30;
  e4->startTime = time(NULL);
  /* Create a message to write. */
  msg1->userID = 1;
  msg1->message = "Hello World1";
  e1->msg = msg1;
  printf("ADDER BEGIN TO RUN\n");
  insert(pq, e1);
  msg2->userID = 2;
  msg2->message = "Hello World2";
  e2->msg = msg2;
  insert(pq, e2);
  msg3->userID = 3;
  msg3->message = "Hello World3";
  e3->msg = msg3;
  insert(pq, e3);
  // msg4->userID = 4;
  // msg4->message = "Hello World4";
  // e4->msg = msg4;
  // insert(pq, e4);
}

int main (int argc, char ** argv)
{
  dds_entity_t participant;
  dds_entity_t topic;
  dds_entity_t writer;
  dds_return_t rc;
  uint32_t status = 0;
  (void)argc;
  (void)argv;

  /* Create a Participant. */
  participant = dds_create_participant (111, NULL, NULL);
  if (participant < 0)
    DDS_FATAL("dds_create_participant: %s\n", dds_strretcode(-participant));

  /* Create a Topic. */
  topic = dds_create_topic (
    participant, &HelloWorldData_Msg_desc, "HelloWorldData_Msg", NULL, NULL);
  if (topic < 0)
    DDS_FATAL("dds_create_topic: %s\n", dds_strretcode(-topic));

  /* Create a Writer. */
  writer = dds_create_writer (participant, topic, NULL, NULL);
  if (writer < 0)
    DDS_FATAL("dds_create_writer: %s\n", dds_strretcode(-writer));

  printf("=== [Publisher]  Waiting for a reader to be discovered ...\n");
  fflush (stdout);

  rc = dds_set_status_mask(writer, DDS_PUBLICATION_MATCHED_STATUS);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_set_status_mask: %s\n", dds_strretcode(-rc));

  while(!(status & DDS_PUBLICATION_MATCHED_STATUS))
  {
    rc = dds_get_status_changes (writer, &status);
    if (rc != DDS_RETCODE_OK)
      DDS_FATAL("dds_get_status_changes: %s\n", dds_strretcode(-rc));

    /* Polling sleep. */
    dds_sleepfor (DDS_MSECS (20));
  }

  priorityQueue pq = initialize(5);

  pthread_t writerTh, scanTh, adderTh;
  struct writerThParam wtp = {writer, pq};
  pthread_create(&adderTh, NULL, (void *)add_pq, (void *)pq);
  //sleep(1);
  pthread_create(&scanTh, NULL, (void *)scanQueue, (void *)pq);
  pthread_create(&writerTh, NULL, (void *)write_pq, (void *)&wtp);

  pthread_join(writerTh, NULL);
  pthread_join(adderTh, NULL);

  /* Deleting the participant will delete all its children recursively as well. */
  rc = dds_delete (participant);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_delete: %s\n", dds_strretcode(-rc));
  return EXIT_SUCCESS;
}
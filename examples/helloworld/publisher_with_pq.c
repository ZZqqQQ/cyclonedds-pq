// #include "dds/dds.h"
// #include "HelloWorldData.h"
// #include "dds/ddsc/dds_priorityqueue.h"
// #include <stdio.h>
// #include <stdlib.h>

// int main (int argc, char ** argv)
// {
//   dds_entity_t participant;
//   dds_entity_t topic;
//   dds_entity_t writer;
//   dds_return_t rc;
//   HelloWorldData_Msg msg1, msg2, msg3, msg4;
//   elementStruct e1, e2, e3, e4;
//   uint32_t status = 0;
//   (void)argc;
//   (void)argv;

//   /* Create a Participant. */
//   participant = dds_create_participant (DDS_DOMAIN_DEFAULT, NULL, NULL);
//   if (participant < 0)
//     DDS_FATAL("dds_create_participant: %s\n", dds_strretcode(-participant));

//   /* Create a Topic. */
//   topic = dds_create_topic (
//     participant, &HelloWorldData_Msg_desc, "HelloWorldData_Msg", NULL, NULL);
//   if (topic < 0)
//     DDS_FATAL("dds_create_topic: %s\n", dds_strretcode(-topic));

//   /* Create a Writer. */
//   writer = dds_create_writer (participant, topic, NULL, NULL);
//   if (writer < 0)
//     DDS_FATAL("dds_create_writer: %s\n", dds_strretcode(-writer));

//   printf("=== [Publisher]  Waiting for a reader to be discovered ...\n");
//   fflush (stdout);

//   rc = dds_set_status_mask(writer, DDS_PUBLICATION_MATCHED_STATUS);
//   if (rc != DDS_RETCODE_OK)
//     DDS_FATAL("dds_set_status_mask: %s\n", dds_strretcode(-rc));

//   while(!(status & DDS_PUBLICATION_MATCHED_STATUS))
//   {
//     rc = dds_get_status_changes (writer, &status);
//     if (rc != DDS_RETCODE_OK)
//       DDS_FATAL("dds_get_status_changes: %s\n", dds_strretcode(-rc));

//     /* Polling sleep. */
//     dds_sleepfor (DDS_MSECS (20));
//   }

//   priorityQueue pq = initialize(5);
  
//   e1.priority = 1;
//   e1.startTime = time(NULL);
//   e2.priority = 2;
//   e2.startTime = time(NULL);
//   e3.priority = 3;
//   e3.startTime = time(NULL);
//   e4.priority = 4;
//   e4.startTime = time(NULL);
//   /* Create a message to write. */
//   msg1.userID = 1;
//   msg1.message = "Hello World1";
//   e1.msg = &msg1;
//   insert(pq, &e1);
//   msg1.userID = 2;
//   msg1.message = "Hello World2";
//   e2.msg = &msg2;
//   insert(pq, &e2);
//   msg1.userID = 3;
//   msg1.message = "Hello World3";
//   e3.msg = &msg3;
//   insert(pq, &e3);
//   msg1.userID = 4;
//   msg1.message = "Hello World4";
//   e4.msg = &msg4;
//   insert(pq, &e4);

//   // printf ("=== [Publisher]  Writing : ");
//   // printf ("Message (%"PRId32", %s)\n", msg.userID, msg.message);
//   // fflush (stdout);

//   write_pq(writer, pq);
//   // rc = dds_write (writer, &msg);
//   // if (rc != DDS_RETCODE_OK)
//   //   DDS_FATAL("dds_write: %s\n", dds_strretcode(-rc));

//   /* Deleting the participant will delete all its children recursively as well. */
//   rc = dds_delete (participant);
//   if (rc != DDS_RETCODE_OK)
//     DDS_FATAL("dds_delete: %s\n", dds_strretcode(-rc));
//   destroy(pq);
//   return EXIT_SUCCESS;
// }

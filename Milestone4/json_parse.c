#include "json_parse.h"
#include "jsmn.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <speedCommand_queue.h>
#include <pidControl.h>


char * trim_space(char *str) { // remove null chars from the array
    char *end;
    /* skip leading whitespace */
    while (isspace(*str)) {
        str = str + 1;
    }
    /* remove trailing whitespace */
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end = end - 1;
    }
    /* write null character */
    *(end+1) = '\0';
    return str;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

int parse(char *message)
{
    int i;
    int r;
    int len = strlen(message);
    int checksum;

    //message for speed command
    char temporary[30];
    speed_msg new_cmd;


    jsmn_parser p;
    jsmntok_t t[500]; /* We expect no more than 150 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, message, len, t, 500);

    if(r < 0)
    {
//        printf("ERROR: Failed to parse MQTT JSON payload\n");
        return -1;
    }

    if (r < 0)
    {
//      printf("Failed to parse JSON: %d\n", r);
      return 1;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT)
    {
      printf("Object expected\n");
      return 1;
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++)
    {
      if (jsoneq(message, &t[i], "status") == 0)
      {
          memset(temporary, 0, 30);
          strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
          temporary[t[i + 1].end - t[i + 1].start] = '\0';
          if (strcmp(trim_space(temporary), "running") == 0) {
              new_cmd.status = running;
          }
          i++;
      }
      else if (jsoneq(message, &t[i], "msg_no") == 0)
      {
          memset(temporary, 0, 30);
          strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
          temporary[t[i + 1].end - t[i + 1].start] = '\0';
          new_cmd.msg_no = atoi(temporary);
          //        sprintf("- sequenceNumber: %.*s\n", t[i + 1].end - t[i + 1].start, message + t[i + 1].start);
          i++;
      }
      else if (jsoneq(message, &t[i], "motor1_speed") == 0)
      {
          memset(temporary, 0, 30);
          strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
          temporary[t[i + 1].end - t[i + 1].start] = '\0';
          new_cmd.motor1_speed = atoi(temporary);
          i++;
      }
      else if (jsoneq(message, &t[i], "motor1_direction") == 0)
      {
          memset(temporary, 0, 30);
          strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
          temporary[t[i + 1].end - t[i + 1].start] = '\0';

          if (strcmp(trim_space(temporary), "DRIVE_FORWARD") == 0) {
              new_cmd.motor1_direction = DRIVE_FORWARD;
          }
          else if (strcmp(trim_space(temporary), "DRIVE_BACKWARD") == 0) {
              new_cmd.motor1_direction = DRIVE_BACKWARD;
          }
          i++;
      }
      else if (jsoneq(message, &t[i], "motor2_speed") == 0)
      {
          memset(temporary, 0, 30);
          strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
          temporary[t[i + 1].end - t[i + 1].start] = '\0';
          new_cmd.motor2_speed = atoi(temporary);
          i++;
      }
      else if (jsoneq(message, &t[i], "motor2_direction") == 0)
      {
          memset(temporary, 0, 30);
          strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
          temporary[t[i + 1].end - t[i + 1].start] = '\0';
          if (strcmp(trim_space(temporary), "DRIVE_FORWARD") == 0) {
              new_cmd.motor2_direction = DRIVE_FORWARD;
          }
          else if (strcmp(trim_space(temporary), "DRIVE_BACKWARD") == 0) {
              new_cmd.motor2_direction = DRIVE_BACKWARD;
          }
          i++;
      }
      else if (jsoneq(message, &t[i], "motor3_speed") == 0)
      {
          memset(temporary, 0, 30);
          strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
          temporary[t[i + 1].end - t[i + 1].start] = '\0';
          new_cmd.motor3_speed = atoi(temporary);
          i++;
      }
      else if (jsoneq(message, &t[i], "motor3_direction") == 0)
      {
          memset(temporary, 0, 30);
          strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
          temporary[t[i + 1].end - t[i + 1].start] = '\0';
          if (strcmp(trim_space(temporary), "DRIVE_FORWARD") == 0) {
              new_cmd.motor3_direction = DRIVE_FORWARD;
          }
          else if (strcmp(trim_space(temporary), "DRIVE_BACKWARD") == 0) {
              new_cmd.motor3_direction = DRIVE_BACKWARD;
          }
          i++;
      }
      else if (jsoneq(message, &t[i], "sequenceNumber") == 0) { // statistics topics
          //printf("- sequenceNumber: %.*s\n", t[i + 1].end - t[i + 1].start, message + t[i + 1].start);
          i++;
      }
      else if (jsoneq(message, &t[i], "checksum") == 0) {
          //printf("- checksum: %.*s\n", t[i + 1].end - t[i + 1].start, message + t[i + 1].start);
          memset(temporary, 0, 30);
          strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
          temporary[t[i + 1].end - t[i + 1].start] = '\0';
          checksum = atoi(temporary);
          if(checksum != t[i + 3].end - t[i + 3].start+1) {
              printf("Checksum does not match\n");\
              return 0;
          }
          i++;
      }
      else if (jsoneq(message, &t[i], "payload") == 0)
      {
          //printf("- payload: %.*s\n", t[i + 1].end - t[i + 1].start, message + t[i + 1].start);
          i++;
      }
      else if (jsoneq(message, &t[i], "boardNumber") == 0)
      {
          //printf("- boardNumber: %.*s\n", t[i + 1].end - t[i + 1].start, message + t[i + 1].start);
          i++;
      }
//      else if (jsoneq(message, &t[i], "KP1") == 0) {
//            memset(temporary, 0, 30);
//            strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
//            temporary[t[i + 1].end - t[i + 1].start] = '\0';
//            kp_m1 = atof(temporary);
//            i++;
//      }
//      else if (jsoneq(message, &t[i], "KP2") == 0) {
//            memset(temporary, 0, 30);
//            strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
//            temporary[t[i + 1].end - t[i + 1].start] = '\0';
//            kp_m2 = atof(temporary);
//            i++;
//      }
//      else if (jsoneq(message, &t[i], "KP3") == 0) {
//            memset(temporary, 0, 30);
//            strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
//            temporary[t[i + 1].end - t[i + 1].start] = '\0';
//            kp_m3 = atof(temporary);
//            i++;
//      }
//      else if (jsoneq(message, &t[i], "KI1") == 0) {
//            memset(temporary, 0, 30);
//            strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
//            temporary[t[i + 1].end - t[i + 1].start] = '\0';
//            ki_m1 = atof(temporary);
//            i++;
//      }
//      else if (jsoneq(message, &t[i], "KI2") == 0) {
//            memset(temporary, 0, 30);
//            strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
//            temporary[t[i + 1].end - t[i + 1].start] = '\0';
//            ki_m2 = atof(temporary);
//            i++;
//      }
//      else if (jsoneq(message, &t[i], "KI3") == 0) {
//            memset(temporary, 0, 30);
//            strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
//            temporary[t[i + 1].end - t[i + 1].start] = '\0';
//            ki_m3 = atof(temporary);
//            i++;
//      }
      else
      {
//        printf("Unexpected key: %.*s\n", t[i].end - t[i].start,message + t[i].start);
      }
    }

    if (&new_cmd != NULL){
        writeMsgToSpeedQueue(&new_cmd);
    }

    return 1; //success case
}

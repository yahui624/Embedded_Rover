#include "json_parse.h"
#include "jsmn.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
      if (jsoneq(message, &t[i], "sequenceNumber") == 0)
      {
        //printf("- sequenceNumber: %.*s\n", t[i + 1].end - t[i + 1].start, message + t[i + 1].start);
        i++;
      }
      else if (jsoneq(message, &t[i], "checksum") == 0)
      {
        //printf("- checksum: %.*s\n", t[i + 1].end - t[i + 1].start, message + t[i + 1].start);

        char temporary[30];
        strncpy(temporary, message + t[i + 1].start, t[i + 1].end - t[i + 1].start);
        temporary[t[i + 1].end - t[i + 1].start] = '\0';
        checksum = atoi(temporary);

        if(checksum != t[i + 3].end - t[i + 3].start+1)
        {
            printf("Checksum does not match\n");
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
      else
      {
        //printf("Unexpected key: %.*s\n", t[i].end - t[i].start,message + t[i].start);
      }
    }

    return 1; //success case
}

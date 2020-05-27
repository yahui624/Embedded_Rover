/*
 * jsmn.c
 *
 *  Created on: Apr 27, 2020
 *      Author: hungnguyen
 */

#include "jsmn.h"

#include "mqtt_client/uart_term.h"

static jsmn_parser parser;

static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0)
    {
        return TRUE;
    }
    return FALSE;
}

int json_parser_process(JSON_Parse_Data *data, const char *json)
{
    jsmn_init(&parser);

    int jsmn_return = ZERO;

    jsmntok_t tokens[MAX_TOKENS];

    jsmn_return =
        jsmn_parse(&parser, json, strlen(json), tokens, sizeof(tokens) / sizeof(tokens[0]));

    if (jsmn_return < 0)
    {
        code(JSON_PARSER_PARSE_FAIL);
        return FALSE;
    }

    // Assume top-level element is an object
    if (jsmn_return < 1 || tokens[0].type != JSMN_OBJECT)
    {
        code(JSON_PARSER_OBJECT_EXPECTED);
        return FALSE;
    }

    int i;
    for (i = 1; i < jsmn_return; i++)
    {
        if (jsoneq(json, &tokens[i], CHECKSUM_STRING))
        {
            int  length = tokens[i + 1].end - tokens[i + 1].start;
            char string_buffer[length + 1];
            memcpy(string_buffer, json + tokens[i + 1].start, length);
            memset((void *)(string_buffer + length), '\0', 1);

            data->checksum = (uint32_t)strtoul(string_buffer, NULL, HEXADECIMAL);

            i++;
        }
        else if (jsoneq(json, &tokens[i], BOARD_ID_STRING))
        {
            int  length = tokens[i + 1].end - tokens[i + 1].start;
            char string_buffer[length + 1];
            memcpy(string_buffer, json + tokens[i + 1].start, length);
            memset((void *)(string_buffer + length), '\0', 1);
            data->board_id = (uint8_t)strtoul(string_buffer, NULL, DECIMAL);

            i++;
        }

        else if (jsoneq(json, &tokens[i], SEQUENCE_STRING))
        {
            int  length = tokens[i + 1].end - tokens[i + 1].start;
            char string_buffer[length + 1];
            memcpy(string_buffer, json + tokens[i + 1].start, length);
            memset((void *)(string_buffer + length), '\0', 1);

            data->sequence = (uint32_t)strtoul(string_buffer, NULL, DECIMAL);

            i++;
        }
        else if (jsoneq(json, &tokens[i], PATHABLE_STRING))
        {
            int  length = tokens[i + 1].end - tokens[i + 1].start;
            char string_buffer[length + 1];
            memcpy(string_buffer, json + tokens[i + 1].start, length);
            memset((void *)(string_buffer + length), '\0', 1);

            data->pathable = (uint8_t)strtoul(string_buffer, NULL, DECIMAL);

            i++;
        }
        else if (jsoneq(json, &tokens[i], COLOR_STRING))
        {
            int  length = tokens[i + 1].end - tokens[i + 1].start;
            char string_buffer[length + 1];
            memcpy(string_buffer, json + tokens[i + 1].start, length);
            memset((void *)(string_buffer + length), '\0', 1);

            data->color = (uint8_t)strtoul(string_buffer, NULL, DECIMAL);
            i++;
        }
        else if (jsoneq(json, &tokens[i], POSITION_STRING))
        {
            int  length = tokens[i + 1].end - tokens[i + 1].start;
            char string_buffer[length + 1];
            memcpy(string_buffer, json + tokens[i + 1].start, length);
            memset((void *)(string_buffer + length), '\0', 1);

            data->position = (uint8_t)strtoul(string_buffer, NULL, DECIMAL);

            i++;
        }
        else
        {
            code(JSON_PARSER_PARSE_EXTRA_DATA);
            i++;
        }
    }

    return TRUE;
}

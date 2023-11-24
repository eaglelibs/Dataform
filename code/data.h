#ifndef el_data_h
#define el_data_h

#include <stdlib.h>
#include <string.h>

/* Public types */

typedef struct el_data el_data;

/* Internal types */

typedef struct
{
char escape;
char real;
}
elz_dsv_escape_code;

typedef struct
{
char delimiter;
char escape_character;
elz_dsv_escape_code* escape_code;
int escape_code_count;
int include_invalid_escape;
}
elz_dsv_config;

typedef struct
{
char* current_field;
int field_offset;
int escaped;
int quoted;
}
elz_dsv_parser;

typedef struct
{
char** field;
int field_count;
}
elz_dsv_data;

struct el_data
{
elz_dsv_config config;
elz_dsv_parser parser;
elz_dsv_data data;
};

/* Public functions */

int el_data_initialise(el_data* dp);
int el_data_dsv_config_set_delimiter(el_data* dp, char delimiter);
int el_data_dsv_config_set_escape_character(el_data* dp, char escape);
int el_data_dsv_config_escape_code(el_data* dp, char escape, char real);
int el_data_dsv_config_include_invalid_escape_sequence(el_data* dp);
int el_data_dsv_config_exclude_invalid_escape_sequence(el_data* dp);
int el_data_dsv_parse_line(el_data* dp, char* line);
char* el_data_dsv_get_field(el_data* dp, int field);
int el_data_dsv_count_fields(el_data* dp);
int el_data_cleanup(el_data* dp);

int elz_dsv_parser_process_char(el_data* dp, char c);
int elz_dsv_parser_process_nonescaped_char(el_data* dp, char c);
int elz_dsv_parser_process_escaped_char(el_data* dp, char c);
int elz_dsv_parser_is_escape_char(el_data* dp, char c);
int elz_dsv_parser_is_delimiter(el_data* dp, char c);
int elz_dsv_parser_process_delimiter(el_data* dp);
int elz_dsv_parser_create_new_field(el_data* dp);
int elz_dsv_parser_add_field_char(elz_dsv_parser* parser, char c);
int elz_dsv_data_add_field(el_data* dp);
int elz_reset(el_data* dp);
int elz_dsv_is_valid_delimiter(char d);
int elz_dsv_config_escape_code_find(elz_dsv_config* config, char c);
int elz_dsv_config_cleanup(elz_dsv_config* config);
int elz_dsv_parser_cleanup(elz_dsv_parser* parser);
int elz_dsv_data_cleanup(elz_dsv_data* data);
int elz_dsv_parser_reset(elz_dsv_parser* parser);
int elz_dsv_config_reset(elz_dsv_config* config);
int elz_dsv_data_reset(elz_dsv_data* data);
int elz_hlp_is_valid_char(char c);

#endif

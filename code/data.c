#include "data.h"

int el_data_initialise(el_data* dp)
{
if(!dp) return 0;
return elz_reset(dp);
}
int el_data_dsv_config_set_delimiter(el_data* dp, char delimiter)
{
if(!dp) return 0;
if(!elz_dsv_is_valid_delimiter(delimiter)) return 0;
dp->config.delimiter=delimiter;
return 1;
}
int el_data_dsv_config_set_escape_character(el_data* dp, char escape)
{
if(!dp) return 0;
if(escape<33) return 0;
if(!elz_dsv_is_valid_delimiter(escape)) return 0;
if(dp->config.delimiter==escape) return 0;
dp->config.escape_character=escape;
return 1;
}
int el_data_dsv_config_escape_code(el_data* dp, char escape, char real)
{
if(!dp) return 0;
int id=elz_dsv_config_escape_code_find(&dp->config, escape);
if(id>-1)
{
dp->config.escape_code[id].real=real;
return 1;
}
int ecc=dp->config.escape_code_count+1;
elz_dsv_escape_code* ec=realloc(dp->config.escape_code, sizeof(elz_dsv_escape_code)*ecc);
if(!ec) return 0;
elz_dsv_escape_code new;
new.escape=escape;
new.real=real;
dp->config.escape_code=ec;
dp->config.escape_code[dp->config.escape_code_count]=new;
dp->config.escape_code_count=ecc;
return 1;
}
int el_data_dsv_config_include_invalid_escape_sequence(el_data* dp)
{
if(!dp) return 0;
dp->config.include_invalid_escape=1;
return 1;
}
int el_data_dsv_config_exclude_invalid_escape_sequence(el_data* dp)
{
if(!dp) return 0;
dp->config.include_invalid_escape=0;
return 1;
}
int el_data_dsv_parse_line(el_data* dp, char* line)
{
if(!dp) return 0;
if((!line)||(!*line)) return 0;
if(dp->config.delimiter==0) return 0;
if(!elz_dsv_parser_create_new_field(dp)) return 0;
for(int x=0; x<=strlen(line); x++)
{
elz_dsv_parser_process_char(dp, line[x]);
}
elz_dsv_parser_cleanup(&dp->parser);
return 1;
}
char* el_data_dsv_get_field(el_data* dp, int field)
{
if(!dp) return NULL;
if(field<0) return NULL;
if(field>=dp->data.field_count) return NULL;
return dp->data.field[field];
}
int el_data_dsv_count_fields(el_data* dp)
{
if(!dp) return 0;
return dp->data.field_count;
}

int el_data_cleanup(el_data* dp)
{
if(!dp) return 0;
if(!elz_dsv_config_cleanup(&dp->config)) return 0;
if(!elz_dsv_parser_cleanup(&dp->parser)) return 0;
if(!elz_dsv_data_cleanup(&dp->data)) return 0;
return elz_reset(dp);
}

int elz_dsv_parser_process_char(el_data* dp, char c)
{
if(!dp) return 0;
if(!elz_hlp_is_valid_char(c)) return 0;
if(elz_dsv_parser_is_delimiter(dp, c)) return elz_dsv_parser_process_delimiter(dp);
if(dp->parser.escaped) return elz_dsv_parser_process_escaped_char(dp, c);
return elz_dsv_parser_process_nonescaped_char(dp, c);
}
int elz_dsv_parser_process_nonescaped_char(el_data* dp, char c)
{
if(!dp) return 0;
if(elz_dsv_parser_is_escape_char(dp, c))
{
dp->parser.escaped=1;
return 1;
}
if(c==34)
{
dp->parser.quoted=!dp->parser.quoted;
return 1;
}
return elz_dsv_parser_add_field_char(&dp->parser, c);
}
int elz_dsv_parser_process_escaped_char(el_data* dp, char c)
{
if(!dp) return 0;
dp->parser.escaped=0;
int id=elz_dsv_config_escape_code_find(&dp->config, c);
if(id>-1) return elz_dsv_parser_add_field_char(&dp->parser, dp->config.escape_code[id].real);
if(!dp->config.include_invalid_escape) return 1;
elz_dsv_parser_add_field_char(&dp->parser, dp->config.escape_character);
elz_dsv_parser_add_field_char(&dp->parser, c);
return 1;
}
int elz_dsv_parser_is_escape_char(el_data* dp, char c)
{
if(!dp) return 0;
if(dp->config.escape_code_count<=0) return 0;
if(dp->config.escape_character==c) return 1;
return 0;
}
int elz_dsv_parser_is_delimiter(el_data* dp, char c)
{
if(!dp) return 0;
if(c==0) return 1;
if(dp->parser.quoted) return 0;
if(c==dp->config.delimiter) return 1;
return 0;
}
int elz_dsv_parser_process_delimiter(el_data* dp)
{
if(!dp) return 0;
if(!dp->parser.current_field) return 0;
if((dp->parser.escaped)&&(dp->config.include_invalid_escape))
{
dp->parser.escaped=0;
elz_dsv_parser_add_field_char(&dp->parser, dp->config.escape_character);
}
if(!elz_dsv_data_add_field(dp)) return 0;
if(!elz_dsv_parser_create_new_field(dp)) return 0;
return 1;
}
int elz_dsv_parser_create_new_field(el_data* dp)
{
if(!dp) return 0;
if(!dp->parser.current_field) dp->parser.current_field=malloc(1024);
if(!dp->parser.current_field) return 0;
strcpy(dp->parser.current_field, "");
dp->parser.field_offset=0;
return 1;
}
int elz_dsv_parser_add_field_char(elz_dsv_parser* parser, char c)
{
if(!parser) return 0;
parser->current_field[parser->field_offset]=c;
parser->field_offset++;
parser->current_field[parser->field_offset]=0;
return 1;
}
int elz_dsv_data_add_field(el_data* dp)
{
if(!dp) return 0;
if(!*dp->parser.current_field) return 0;
char* newfield=NULL;
if(dp->parser.current_field[0]!=0)
{
newfield=malloc(strlen(dp->parser.current_field)+1);
if(!newfield) return 0;
strcpy(newfield, dp->parser.current_field);
}
int newdatasize=dp->data.field_count+1;
char** newdata=realloc(dp->data.field, sizeof(char*)*newdatasize);
if(!newdata)
{
if(newfield) free(newfield);
return 0;
}
newdata[dp->data.field_count]=newfield;
dp->data.field=newdata;
dp->data.field_count=newdatasize;
return 1;
}

int elz_dsv_is_valid_delimiter(char d)
{
if(d==9) return 1;
if(d<32) return 0;
if(d>=127) return 0;
if((d==34)||(d==39)) return 0;
if((d>=48)&&(d<=57)) return 0;
if((d>=65)&&(d<=90)) return 0;
if((d>=96)&&(d<=122)) return 0;
return 1;
}
int elz_dsv_config_escape_code_find(elz_dsv_config* config, char c)
{
if(!config) return -1;
for(int x=0; x<config->escape_code_count; x++)
{
if(config->escape_code[x].escape==c) return x;
}
return -1;
}

int elz_reset(el_data* dp)
{
if(!dp) return 1;
elz_dsv_parser_reset(&dp->parser);
elz_dsv_config_reset(&dp->config);
elz_dsv_data_reset(&dp->data);
return 1;
}
int elz_dsv_config_cleanup(elz_dsv_config* config)
{
if(!config) return 1;
if(config->escape_code) free(config->escape_code);
return elz_dsv_config_reset(config);;
}
int elz_dsv_parser_cleanup(elz_dsv_parser* parser)
{
if(!parser) return 1;
if(parser->current_field) free(parser->current_field);
return elz_dsv_parser_reset(parser);
}
int elz_dsv_data_cleanup(elz_dsv_data* data)
{
if(!data) return 1;
for(int x=0; x<data->field_count; x++)
{
free(data->field[x]);
}
if(data->field) free(data->field);
return elz_dsv_data_reset(data);
}
int elz_dsv_parser_reset(elz_dsv_parser* parser)
{
if(!parser) return 1;
parser->current_field=NULL;
parser->field_offset=0;
parser->escaped=0;
parser->quoted=0;
return 1;
}
int elz_dsv_config_reset(elz_dsv_config* config)
{
if(!config) return 1;
config->delimiter=44;
config->escape_character=92;
config->escape_code=NULL;
config->escape_code_count=0;
config->include_invalid_escape=1;
return 1;
}
int elz_dsv_data_reset(elz_dsv_data* data)
{
if(!data) return 1;
data->field=NULL;
data->field_count=0;
return 1;
}
int elz_hlp_is_valid_char(char c)
{
if(c==9) return 1;
if(c<32) return 0;
if(c>=127) return 0;
return 1;
}

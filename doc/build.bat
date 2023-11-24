@echo off

mkdir html\api

pandoc --from=mediawiki --to=gfm --output=../readme.md ../readme.mw >>result.log 2>&1

pandoc --from=mediawiki --to=html --output=html/index.html --template=template.html mw/index.mw >>result.log 2>&1
pandoc --from=mediawiki --to=html --output=html/api/el_data_cleanup.html --template=template.html mw/api/el_data_cleanup.mw >>result.log 2>&1
pandoc --from=mediawiki --to=html --output=html/api/el_data_initialise.html --template=template.html mw/api/el_data_initialise.mw >>result.log 2>&1
pandoc --from=mediawiki --to=html --output=html/api/el_data_dsv_config_escape_code.html --template=template.html mw/api/el_data_dsv_config_escape_code.mw >>result.log 2>&1
pandoc --from=mediawiki --to=html --output=html/api/el_data_dsv_config_exclude_invalid_escape_sequence.html --template=template.html mw/api/el_data_dsv_config_exclude_invalid_escape_sequence.mw >>result.log 2>&1
pandoc --from=mediawiki --to=html --output=html/api/el_data_dsv_config_include_invalid_escape_sequence.html --template=template.html mw/api/el_data_dsv_config_include_invalid_escape_sequence.mw >>result.log 2>&1
pandoc --from=mediawiki --to=html --output=html/api/el_data_dsv_config_set_delimiter.html --template=template.html mw/api/el_data_dsv_config_set_delimiter.mw >>result.log 2>&1
pandoc --from=mediawiki --to=html --output=html/api/el_data_dsv_config_set_escape_character.html --template=template.html mw/api/el_data_dsv_config_set_escape_character.mw >>result.log 2>&1
pandoc --from=mediawiki --to=html --output=html/api/el_data_dsv_count_fields.html --template=template.html mw/api/el_data_dsv_count_fields.mw >>result.log 2>&1
pandoc --from=mediawiki --to=html --output=html/api/el_data_dsv_get_field.html --template=template.html mw/api/el_data_dsv_get_field.mw >>result.log 2>&1
pandoc --from=mediawiki --to=html --output=html/api/el_data_dsv_parse_line.html --template=template.html mw/api/el_data_dsv_parse_line.mw >>result.log 2>&1

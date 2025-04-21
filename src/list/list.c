#include "list.h"

int load_template_and_replace(const char*  filename, 
			      const char*  placeholder, 
			      const char*  replacement,
			            char** result_ptr) {
    int flag_error = ERR_OK;

    FILE* file = fopen(filename, "r");
    if(  !file)
	flag_error = ERR_FILE_NOT_FOUND;
    else {
	fseek( file, 0, SEEK_END);
	long len = ftell(file);
	rewind(file);

	char* buffer = malloc(len + 1);
	if(  !buffer)
	    flag_error = ERR_ALLOCATION;
	else {
	    fread(buffer, 1, len, file);
	    buffer[len] = '\0';
	    fclose(file);

	    char* pos = strstr(buffer, placeholder);
	    if(  !pos)
		flag_error = ERR_SEARCH;
	    else {
		size_t new_len = len + strlen(replacement) - strlen(placeholder);
		char* local_result = malloc(new_len + 1);
		if(  !local_result)
		    flag_error = ERR_ALLOCATION;
		else {
		    size_t 	 prefix_len = pos - buffer;
		    strncpy(local_result, buffer, prefix_len);
		    local_result[prefix_len] = '\0';

		    strcat(local_result, replacement);
		    strcat(local_result, pos + strlen(placeholder));

		    *result_ptr = local_result;
		    local_result = NULL;
		}
	    }
	    free(buffer);
	}
    }

    return flag_error;
}

int parse_string(const char* input, 
		 char* 	     output,
		 size_t      output_size,
		 char*	     type_parse) {
    int flag_error = ERR_OK;
	
    type_parse[MAX_TYPE_PARSE_LEN - 1] = '\0';
    int type = atoi(type_parse);

    char* temp_str = strdup(input);
    if(  !temp_str)
	flag_error = ERR_ALLOCATION;
    else {
    	const char *delimiters = ", ";
    	char* token = strtok(temp_str, delimiters);

    	output[0]      = '\0';
    	int offset     = 0;
    	int word_count = 0;
	
    	while(token != NULL && word_count < MAX_WORDS) {
	    if     (type == 1)
	    	offset += snprintf(output      + offset, 
			           output_size - offset, 
			    "%d. %s<br>", word_count + 1, token);
	    else if(type == 2)
	    	offset += snprintf(output      + offset, 
			    	   output_size - offset, 
			    "• %s<br>", token);
	
	    word_count++;
	    token = strtok(NULL, delimiters);
        }

	free(temp_str);
    }

    return flag_error;
}



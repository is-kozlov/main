#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100

char* del_space(char* s) {    
    if(s[0] == '\0') return s; 
    size_t buffer_size = BUFFER_SIZE;  
    char* buf = (char*) malloc ((size_t)buffer_size*sizeof(char));
    if(buf == NULL) return NULL;    
    size_t len = 1;
    buf[0] = s[0];  
  
    for(size_t i = 1; s[i] != '\0'; i++) {
        if((s[i] == ' ') && (buf[len-1] == ' ')) continue;
        
        buf[len++] = s[i];

        if(len == buffer_size) {
            buffer_size *= 2;
            buf = (char*) realloc (buf, (size_t)buffer_size * sizeof(char));
            if(buf == NULL) return NULL;
        }
    }
    
    buf = (char*) realloc (buf, (size_t)++len * sizeof(char));
    if(buf == NULL) return NULL;
    buf[len-1] = '\0';
    
    s = (char*) memcpy(s, buf, (size_t)len * sizeof(char));
    free(buf);
    return s;
}

//------------------------------------------------------------------------------

char** arr_del_space(char** arr, size_t N) {
    for(size_t i = 0; i < N; i++) {
        arr[i] = del_space(arr[i]);
        if(arr[i] == NULL) return NULL;
    }
    return arr;
}

//------------------------------------------------------------------------------

char** read_array(char** arr, size_t *ptr_N) {
    size_t len = 0;
    size_t N = 1;
    char tmp;
    size_t buffer_size = BUFFER_SIZE;
    arr[0] = (char*) malloc ((size_t)buffer_size * sizeof(char));
    if(arr[0] == NULL) return NULL; 

    while((tmp = getchar()) != EOF) {      
        arr[N-1][len++] = (char) tmp;
      
        if(tmp == '\n') {
            arr[N-1][len-1] = '\0';
            arr[N-1] = (char*) realloc (arr[N-1], (size_t)len * sizeof(char));
            if(arr[N-1] == NULL) return NULL;            
            buffer_size = BUFFER_SIZE;
            arr = (char**) realloc (arr, (size_t)++N * sizeof(char*));
            if(arr == NULL) return NULL;
            arr[N-1] = (char*) malloc ((size_t)buffer_size * sizeof(char));
            if(arr[N-1] == NULL) return NULL;
            len = 0;
        }
        
        if(len == buffer_size) {
            buffer_size *= 2;
            arr[N-1] = (char*) realloc (arr[N-1], (size_t)buffer_size * sizeof(char));
            if(arr[N-1] == NULL) return NULL;
        }

    }

    arr[N-1] = (char*) realloc (arr[N-1], (size_t)++len * sizeof(char));
    if(arr[N-1] == NULL) return NULL;
    arr[N-1][len-1] = (char) '\0';
    *ptr_N = N; 
    return arr; 
}

//------------------------------------------------------------------------------

void print_array(char** arr, size_t N) {

    for(size_t i = 0; i < N; i++) printf("%s\n", arr[i]);
    return;   
}

//------------------------------------------------------------------------------

void clean_array(char** arr, size_t N) {
    for(size_t i = 0; i < N; i++) free(arr[i]);
    free(arr);
    return;
}

//------------------------------------------------------------------------------

int main() {
    size_t N = 0;  
    char** arr = NULL; 
       
    arr = (char**) malloc ((size_t)++N*sizeof(char*));
    if(arr == NULL) {
        printf("[error]");
        return 0;
    }
    
    arr = read_array(arr, &N);
    if(arr == NULL) {
        printf("[error]");
        return 0;
    }

    arr = arr_del_space(arr, N);
    if(arr == NULL) {
        printf("[error]");
        return 0;
    }
    print_array(arr, N);
    clean_array(arr, N);    
    return 0;
}

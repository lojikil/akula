#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define nil NULL
#define nul '\0'

/* Headers would be better as a dict
 * (either linear or trie or whatever).
 * Could just implement FNV1A again and
 * hash headers...
 */

typedef struct httphead {
    char *name;
    char *value;
    int refc;
} HTTPHeader;

typedef struct cookie {
    char secure;
    char httponly;
    char *name;
    char *value;
    char *expires;
    char *path;
    char *domain;
    char *comment;
    int refc;
} HTTPCookie;

typedef struct httpreq {
    char *method;
    char *uri;
    char *query;
    char *body;
    HTTPHeader **headers;
    HTTPCookie **cookieJar;
    int refc;
} HTTPRequest;

typedef struct httpresp {
    char *version;
    char *status;
    char *message;
    char *body;
    HTTPHeader **headers;
    HTTPCookie **cookieJar;
    int refc;
} HTTPResponse;

/* not really sure how much I like the below types...
 * they make sense, sure, but they're kinda unweildy
 * to use...
 */

typedef enum {
    WRAPPED, UNWRAPPED;
} Wrapping;

typedef struct {
    Wrapping wrap;
    union {
        char **raw_cookies;
        HTTPCookie **cookies;
    } wrapped;
    int refc;
} WrappedCookie;

typedef struct {
    Wrapping wrap;
    union {
        char **raw_headers;
        HTTPCookie **cookies;
    } wrapped;
    int refc;
} WrappedHeader;

const char defaultUserAgent[] = "Akula/bot 0.0alpha";

/* Don't know if we really want this:

HTTPResponse *get(HTTPRequest *);
HTTPResponse *post(HTTPRequest *);
HTTPResponse *head(HTTPRequest *);

 because probably we want users to just be able to
 pass in strings and have a request type generated
 for them...

Of course, problem 2 occurs: we may have requests wherein we already have
a set of headers/cookies we'd like to use, but we need to grab the char 
values... Yuck.
 */

HTTPResponse *get(char *URI, char *query, WrappedHeader *headers, WrappedCookie *cookies);
HTTPResponse *put(char *URI, char *query, WrappedHeader *headers, WrappedCookie *cookies);
HTTPResponse *post(char *URI, char *query, char *body, WrappedHeader *headers, WrappedCookie *cookies);
HTTPResponse *head(char *URI, char *query, WrappedHeader *headers, WrappedCookie *cookies);
HTTPResponse *options(char *URI, char *query, WrappedHeader *headers, WrappedCookie *cookies);
HTTPResponse *delete(char *URI, char *query, WrappedHeader *headers, WrappedCookie *cookies);
HTTPResponse *issue_request(char *method, char *URI, char *query, char *body, WrappedHeader *headers, WrappedCookie *cookies);
HTTPResponse *issue_typed_request(HTTPRequest *request);
int increment_req(HTTPRequest *);
int decrement_req(HTTPRequest *);
int increment_res(HTTPResponse *);
int decrement_res(HTTPResponse *);
int increment_header(HTTPHeader *);
int decrement_header(HTTPHeader *);
int increment_cookie(HTTPCookie *);
int decrement_cookie(HTTPCookie *);
void directory_buster(char *host, char **db_paths);

int
main(int ac, char **al, char **el) {

    if(ac != 3) {
        usage();
        return 1;
    }
    
    printf("");
    printf("        __         .__          \n");
    printf("_____  |  | ____ __|  | _____   \n");
    printf("\\__  \\ |  |/ /  |  \\  | \\__  \\  \n");
    printf(" / __ \\|    <|  |  /  |__/ __ \\_\n");
    printf("(____  /__|_ \\____/|____(____  /\n");
    printf("     \\/     \\/               \\/ \n");
    printf("  MIT/X licensed HTTP Fuzzer\n");
    printf("  copyright 2015 Stefan Edwards/lojikil\n");
    directory_buster(al[1], &al[2]);
    return 0;
}

void
directory_buster(char *host, char **db_paths) {
    FILE *cur_fh = nil;
    char *cur_url = nil, *tmp = nil;
    int idx = 0;
   
    cur_url = (char *)calloc(sizeof(char), 2048);

    while(db_paths[idx] != nil) {
        cur_fh = fopen(db_paths[idx], "r");

        if(!cur_fh) {
            printf("[!] could not open database path: %s\n", db_paths[idx]);
            continue; 
        }

        while(1) {
            // use tmp, so as to keep a ref to cur_url for free(3)
            tmp = fgets(cur_url, 2048, cur_fh);
            if(!tmp || feof(cur_fh)) {
                break;
            }
        }

        fclose(cur_fh);
        idx++;
    }

    free(cur_url);
}

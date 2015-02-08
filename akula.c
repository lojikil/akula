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

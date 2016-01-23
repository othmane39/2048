#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <unistd.h>

#include "score.h"

static int make_request(char* host, int port, char* pathToAPI, char* parameters, char** result);
/* This function makes a HTTP request.
   host is the host to try to connect. (ex : localhost)
   port is the port to connect on. (ex : 80)
   pathToAPI is the file path on the remote server to the api file (ex : /scoreServer/score.php)
   parameters is the parameters given to the api ; they are separated with '&'. Refer to API's doc to see available commands (ex : command=getHighScores)
   result is a pointer to a string where to write the result page
   The function return 0 if it succeds -1 else
*/

static void parse_highScore(char* result, int* score, char** username);
/* This function parses the result of a request to separate the score and the username of the hightest score.
   result is the raw text to separate
   score is the variable where we want to write the score
   username is the variable where we want tot write the username
*/

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

static  int make_request(char* host, int port, char* pathToAPI, char* parameters, char** result)
{
  CURL *curl;
  CURLcode res;

  int succes = 0;

  curl = curl_easy_init();
  if(curl) {
    struct string s;
    init_string(&s);

    char* request = malloc(sizeof(char) * 255);
    sprintf(request, "%s:%d%s?%s", host, port, pathToAPI, parameters);
    curl_easy_setopt(curl, CURLOPT_URL, request);
    free(request);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
      succes = -1;
    else
      strcpy(*result, s.ptr);

    free(s.ptr);
    curl_easy_cleanup(curl);
  }

  return succes;
}

static void parse_highScore(char* result, int* score, char** username)
{
  *score = atoi(strtok(result, ":"));
  *username = strcpy(*username, strtok(NULL, ":"));
}

bool is_highScore(int scoreToCheck)
{
  // We get the result of the api first
  char* result = malloc(sizeof(char) * 255);
  char* parameters = malloc(sizeof(char) * 255);
  sprintf(parameters, "command=isHighScore&score=%d", scoreToCheck);
  if(!make_request(HOST, PORT, PATH_TO_API, parameters, &result)) // if the request works
    {
      free(parameters);

      // We compare the result
      if(strcmp(result, "TRUE") == 0)
	{
	  free(result);
	  return true;
	}
    }
  free(parameters);
  free(result);

  return false;
}
 
int get_highScore(int* highScore, char** username)
{
  char* result = malloc(sizeof(char) * 255);
  if(!make_request(HOST, PORT, PATH_TO_API, "command=getHighScores", &result)) // if the request works
    {
      parse_highScore(result, highScore, username); // we parse the result an copy it to the result variables
      free(result);
      return 0;
    }
  else
    return -1;
}

int submit_score(int scoreToSubmit, char* username)
{
  char* result = malloc(sizeof(char) * 255);
  char* parameters = malloc(sizeof(char) * 255);
  sprintf(parameters, "command=submit&score=%d&username=%s", scoreToSubmit, username);
  if(!make_request(HOST, PORT, PATH_TO_API, parameters, &result)) // if the request works
    {
      // We compare the result
      if(strcmp(result, "SCORE_RECEIVED") == 0)
	{
	  free(result);
	  return 0;
	}
    }
  free(parameters);
  free(result);

  return -1;
}

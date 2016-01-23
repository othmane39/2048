#ifndef _SCORE_H_
#define _SCORE_H_

#include <stdbool.h>

/* Edit those parameters to connect to score server */
#define HOST "ota2048.alwaysdata.net"
#define PORT 80
#define PATH_TO_API "/score.php"

bool is_highScore(int scoreToCheck);
/* This function checks if a score it's a highscore
   scoreToCheck is the score to check
   result is true if the parameter is a highscore, return false in the other case
*/

int get_highScore(int* highScore, char** username);
/* This function downloads the hightest score from the server
   highscore is a pointer to a variable which will contain the highscore
   username is a pointer to a variable which will contain the username of the hightest score
   The function returns 0 if it succeds -1 else
*/

int submit_score(int scoreToSubmit, char* username);
/* This function submits a score, if it's a hightscore, it will automatically replace the old hightest score
   scoreToSubmit is the score to submit
   username is the username of the player
   The function returns 0 if it succeds -1 else
*/

#endif

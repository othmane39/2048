<?php
	/* This API allows 3 commands:

	1. http://host/score.php?command=isHighScore&score={scoreToCheck}
	Prints TRUE if {scoreToCheck} is a highscore. 
	Prints FALSE if not.

	2. http://host/score.php?command=submit&score={newScore}&username={username}
	Prints SCORE_RECEIVED if {newScore} is received. This also add it to highscores if it's a record and saves it's player.

	3. http://host/score.php?command=getHighScores
	Prints the actual highscores separated with ';' charter.
	*/
	
	if(isset($_GET['command']))
	{		
		if($_GET['command'] == "submit" && isset($_GET['score']) && $_GET['score'] != NULL && isset($_GET['username']) && $_GET['username'] != NULL)
		{
			$scoreToAdd = $_GET['score'];
			$username = $_GET['username'];

			if(is_highscore($scoreToAdd))
			{
				set_high_scores($scoreToAdd, $username);
			}
			print("SCORE_RECEIVED");
		}
		elseif($_GET['command'] == "getHighScores")
		{
			print(get_high_scores_raw());
		}
		elseif($_GET['command'] == "isHighScore" && isset($_GET['score']) && $_GET['score'] != NULL)
		{
			if(is_highscore($_GET['score']))
				print('TRUE');
			else
				print('FALSE');
		}
		else
		{
			print("ERROR");
		}	
	}
?>
<?php
	function is_highscore($scoreToCheck)
	{
		$highScores = get_high_scores();
		foreach($highScores as $key => $value)
		{
			if($scoreToCheck> $value[0])
					 return true;
		}

		return false;
	}
	
	function get_high_scores_raw()
	{
		$fileToRead = fopen("highscores.txt", "r") or die("ERROR_FILE_OPEN");
		$highScoresRaw = fread($fileToRead, filesize("highscores.txt"));
		fclose($fileToRead);

		return $highScoresRaw;
	}
	function get_high_scores()
	{	
		$tmp = explode(";", get_high_scores_raw());
		foreach ($tmp as $key => $value)
		{
			$tmp[$key] = explode(":", $value);
		}
		return $tmp;
	}

	function set_high_scores($scoreToAdd, $username)
	{
		$highScores = get_high_scores();
		
		$highScoresInverted = array_reverse($highScores);
		//print_r($highScoresInverted);
		foreach($highScoresInverted as $key => $value)
		{
			if($scoreToAdd > $value[0] && $key < count($highScoresInverted) - 1)
			{
				$highScoresInverted[$key][0] = $highScoresInverted[$key + 1][0];
				$highScoresInverted[$key][1] = $highScoresInverted[$key + 1][1];
				if($scoreToAdd < $highScoresInverted[$key + 1][0])
				{
					$highScoresInverted[$key][0] = $scoreToAdd;
					$highScoresInverted[$key][1] = $username;
				}
			}
			elseif($scoreToAdd > $value[0] && $key == count($highScoresInverted) - 1)
			{
				$highScoresInverted[$key][0] = $scoreToAdd;
				$highScoresInverted[$key][1] = $username;
			}
		}
		$highScores = array_reverse($highScoresInverted);
		$formatedScores = '';
		foreach ($highScores as $key => $value)
		{
			$formatedScores .= $value[0];
			$formatedScores .= ":";
			$formatedScores .= $value[1];
			if($key < count($highScores) - 1)
				$formatedScores .= ';';
		}

		$fileToWrite = fopen("highscores.txt", "w") or die("ERROR_FILE_OPEN");
		$highScoresRaw = fwrite($fileToWrite, $formatedScores);
		fclose($fileToWrite);
	}
?>
  int pfd[2];
   pipe(pfd);
int child1Exit;
int child2Exit;
int secondNeeded;


auto pid1 = fork()

if(pid1 == 0)
{

	if(tokens[2] == ‘>’)
	{
		int filed = open(“file”);
		dup2(filed,STDOUT_FILENO);

	}
	else if(tokens[2] == ‘<’)
	{
		int filed = open(“file”);
		dup2(filed,STDIN_FILENO);

	}
	else if(tokens[2] == ‘|’)
	{
		close(pfd[0]);
		dup2(pfd[1],STDOUT_FILENO);
		close(pfd[1]);

	}

	execlp(tokens[0],tokens[0], tokens[1],NULL);


	exit(1);
	

}


waitpid(pid1,&child1Exit);

if(tokens.length() > 2)
{
		if(child1Exit == 0 && tokens[2] ==’&&’)

{
	secondNeeded = 1;
}
else if(child1Exit != 0 && tokens[2] == ‘||’)
{
	secondNeeded = 1;
}
else if(tokens[2] == ‘;’ || tokens[2] == ‘|’)
{
	secondNeeded = 1;
}
else
{
		secondNeeded = 0;
}



	if(secondNeeded)
	{
		auto pid2 = fork();

		if(pid2 == 0)
		{

			 if(tokens[2] == ‘|’)
			{
				close(pfd[1]);
				dup2(pfd[0],STDIN_FILENO);
				close(pfd[0]);

			}
			
			execlp(tokens[3],tokens[3],tokens[4],NULL);
			exit(1);

			
		}

		waitpid(pid2,&child2Exit);
	}
	








	return 0;

}

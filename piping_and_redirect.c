/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_and_redirect.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:48:50 by ckonneck          #+#    #+#             */
/*   Updated: 2024/10/08 12:07:54 by ckonneck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	handle_pipe(t_cmd *cmd_list, int number_of_pipes, char **envp)
{
	int pipefd[2];
	int i;
	int pid;
	int saved_stdin;
	int saved_stdout;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (number_of_pipes == 0)
	{
		pipe_function(cmd_list, envp, saved_stdin, saved_stdout);
		return;
	}
	i = 0;
	if (number_of_pipes > 0)
		pipe(pipefd);
	pid = fork();
	if (pid == 0)
		child_function(pipefd, cmd_list, envp);
	else
    {
		parent_function(pipefd, cmd_list, envp, number_of_pipes);
		waitpid(pid, NULL, 0);
    }
	pipe_end_function(saved_stdin, saved_stdout);
}


void	child_function(int *pipefd, t_cmd *cmd_list, char **envp)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	execute_path(cmd_list, envp);
	exit(0);
}

void	parent_function(int *pipefd, t_cmd *cmd_list, char **envp, int number_of_pipes)
{
	close(pipefd[1]); 
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
	printf("WE GOING DOWN BOIS\n");
    handle_pipe(cmd_list->next, number_of_pipes - 1, envp);
}
void	pipe_function(t_cmd *cmd_list, char **envp, int saved_stdin, int saved_stdout)
{
	handle_redirect_or_execute(cmd_list, envp);
	dup2(saved_stdin, STDIN_FILENO);   
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
void	pipe_end_function(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
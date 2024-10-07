/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 14:27:51 by ckonneck          #+#    #+#             */
/*   Updated: 2024/10/07 11:22:09 by ckonneck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "parsing.h"
// int g_sig = 0;

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_firstcmd	*command_table;
	int			i;
	int			found;
	// g_sig = 0;
	command_table = init_command_table();
	input = NULL;
	setup_signal_handlers();
	while (1)
	{
		input = prompt();
		argv = ft_split(input, ' ');
		argc = 0;
		i = 0;
		found = 0;
		int pipenumber = 0;
		while (argv[argc])
			argc++;
		if (argc > 0)
		{
			while (command_table[i].name != NULL)
			{
				if (ft_strncmp(argv[0], command_table[i].name,
						ft_strlen(command_table[i].name)) == 0)
				{
					command_table[i].func(argv, envp);
					found = 1;
					break ;
				}
				i++;
			}
			if (ft_strncmp(argv[0], "exit", ft_strlen("exit")) == 0)
				exit_function(command_table, argv, input);
			int k = 0;
			while (argv[k])
			{
				if (ft_strncmp(argv[k], "|", ft_strlen("|")) == 0)
					pipenumber++;
				k++;
			}
			if (pipenumber > 0)
				handle_pipe(argv, k, envp);
			
			if (!found)
				handle_redirect_or_execute(argv, envp);
		}
		free_call(argv, input);
	}
}

void	free_call(char **argv, char *input)
{
	int	i;

	free(input);
	i = 0;
	while (argv[i] != NULL)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	exit_function(t_firstcmd *command_table, char **argv, char *input)
{
	printf("GOODBYE NYA\n");
	free(command_table);
	free_call(argv, input);
	exit(0);
}

void	handle_redirect_or_execute(char **argv, char **envp)
{
	if (argv[1] && ft_strncmp(argv[1], "<<", ft_strlen("<<")) == 0)
		heredoc(argv, envp);
	else if (argv[1] && ft_strncmp(argv[1], "<", ft_strlen("<")) == 0)
		input_redirect(argv, envp);
	else if (argv[1] && ft_strncmp(argv[1], ">>", ft_strlen(">>")) == 0)
		output_append(argv, envp);
	else if (argv[1] && ft_strncmp(argv[1], ">", ft_strlen(">")) == 0)
		output_redirect(argv, envp);
	else
		execute_path(argv, envp);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjackows <cjackows@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 13:11:49 by cjackows          #+#    #+#             */
/*   Updated: 2023/06/16 17:02:51 by cjackows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pc_start_minishell(t_env *env);
static void	pc_free_commands_tab(t_command *commands);
static void	pc_tester(t_env *env, int ac, char **av);

int	main(int ac, char **av, char **env_var)
{
	t_env	env;

	// (void)ac;
	// (void)av;
	pc_copy_env_variables(&env, env_var);
	pc_tester(&env, ac, av);
	pc_mod_term_atributes_echoctl_off();
	pc_signals_interactive();
	pc_start_minishell(&env);
	pc_clear_env(&env);
	return (0);
}

/** @brief Starts minishell within infinite loop. */
static void	pc_start_minishell(t_env *env)
{
	char		*input;
	t_command	*commands;

	while (1)
	{
		pc_signals_interactive();
		input = readline(BLUE SHELL_PROMPT DARKBLUE SHELL_SIGN NC);
		if (input == NULL)
			break ;
		if (!input[0])
			continue ;
		add_history(input);
		commands = pc_parse_raw_input(env, &input);
		if (!commands || commands[0].command == 0)
			continue ;
		env->last_result = pc_exec_commands(env, commands);
		pc_free_commands_tab(commands);
		free(input);
	}
}

static void	pc_free_commands_tab(t_command *commands)
{
	int	i;

	i = 0;
	while (commands[i].command)
	{
		if (commands[i].command)
			free(commands[i].command);
		if (commands[i].flags)
			free(commands[i].flags);
		if (commands[i].arguments)
			pc_clear_2d_tab(commands[i].arguments);
		i++;
	}
	free(commands);
}

/** @brief  Performs a stylish exit from the program.
 * @param msg Error message to be displayed on failure.
 * @param failure Failure code indicating the type of exit. */
void	pc_quit(t_env *env, char *msg, int failure)
{
	pc_clear_env(env);
	if (failure == -1)
	{
		printf("%s%s%s\n", ERROR, msg, NC);
		exit(EXIT_FAILURE);
	}
	else if (!failure)
	{
		exit(EXIT_SUCCESS);
	}
	else
	{
		printf("%s%s%s\n", ERROR, msg, NC);
		exit(failure);
	}
}

static void	pc_tester(t_env *env, int ac, char **av)
{
	int		exit_status;

	if (ac >= 3 && !ft_strncmp(av[1], "-c", 3))
	{
		t_command	*commands;
		char *com = ft_strdup(av[2]);
		commands = pc_parse_raw_input(env, &com);
		if (!commands)
			exit(2);
		exit_status = pc_exec_commands(env, commands);
		free(com);
		exit(exit_status);
	}
}
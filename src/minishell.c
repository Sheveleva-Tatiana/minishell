/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshera <sshera@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 12:14:20 by bcarlee           #+#    #+#             */
/*   Updated: 2022/01/29 13:04:31 by sshera           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env *init_env(t_data *data, char **env)
{
    int     i;
    char    *j;
    t_env   *tmp;

    data->env = malloc(sizeof(t_env));
    tmp = data->env;
    i = 0;
    while (env[i])
    {
       j = ft_strchr(env[i], '=');
       tmp->key = ft_substr(env[i], 0, ft_strichr(env[i], '=') + 1);
       tmp->value = ft_substr((j + 1), 0, ft_strlen(j) - 1);
       tmp->next = malloc(sizeof(t_env));
       tmp = tmp->next;
       i++;
    }
    tmp = NULL;
    data->std_in = dup(0);
    data->std_out = dup(1);
    return (data->env);
}

int	main(int i, char **argv, char **env)
{
	char	*line;
	t_data	data;
    t_env   *envp;

	if (i > 1)
		return (print_error("No such file or directory\n", 2));
	envp = init_env(&data, env);
    data.exit_code = 0;
	while (1)
	{
        dup2(data.std_in, 0);
        dup2(data.std_out, 1);
        init_signal_h(&data);
		line = readline("\033[1;31mminishell->\033[0m ");
        if (line[0])
            add_history(line);
        i = preparsing(line, 0, 0, 0);
        if (i > 0)
        {
            print_error("", i);
            exit(1);
        }
		data.cmd->cmd = parser(line, &data);
		ft_execve(data);
	}
	return (0);
}

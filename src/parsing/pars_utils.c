#include "../../includes/minishell.h"

int get_redir_count(char *str, char c, t_redirect *rd) {
    rd->str = ft_strdup(str);
    if (!str)
        return (1);
    rd->i = 0;
    while (rd->str[rd->i] == c)
        rd->i++;
    if (!(rd->str[rd->i]) || rd->i > 2)
    {
        free(rd->str);
        return (1);
    }
    rd->p = rd->i;
    rd->s = NULL;
    while (rd->str[rd->i] == ' ')
        rd->i++;
    rd->tmp = NULL;
    return (0);
}

int set_dollar(t_redirect *rd, char c)
{
    char    *tmp;
    char    *tmp2;

    if (c == '$')
    {
        tmp2 = replace_value(&rd->str[rd->i]);
        if (!tmp2)
            return (-1);
        tmp = ft_substr(rd->str, 0, rd->i);
        if (!tmp)
            return (-1);
        free(rd->str);
        rd->str = ft_strdup(tmp);
        free(tmp);
        if (tmp2[0] == '\"')
            rd->str = ft_strjoin(rd->str, &tmp2[1]);
        else
            rd->str = ft_strjoin(rd->str, tmp2);
        free(tmp2);
    }
    return (0);
}

void    redirect_hand(char *str, char c, int *i, int *r)
{
    t_redirect rd;

    if (get_redir_count(str, c, &rd))
        return ;
    while (rd.str[rd.i] && rd.str[rd.i] != ' ')
    {
        rd.j = rd.i;
        while (rd.str[rd.i] && !set_dollar(&rd, rd.str[rd.i]) && \
        rd.str[rd.i] != '\'' && rd.str[rd.i] != '\"' && rd.str[rd.i] != ' ')
            rd.i++;
        if (!rd.str[rd.i] || rd.str[rd.i] == ' ')
        {
            rd.tmp = ft_substr(rd.str, rd.j, rd.i = rd.j);
            if (!rd.tmp)
                return ;
            rd.s = ft_strjoin(rd.s, rd.tmp);
            free(rd.tmp);
            break ;
        }
        else if (rd.str[rd.i] == '\'' || rd.str[rd.i] == '\"')
            get_str_in_quotes(&rd, rd.str[rd.i]);
    }
    printf("%s", rd.tmp);
}

void    skip_cmd(t_pars *pars)
{
    if (pars->cmd[pars->i] == ' ')
        implement_index(&pars->i, &pars->k, pars->cmd, ' ');
    else if (pars->cmd[pars->i] == '<' || pars->cmd[pars->i] == '>')
        redirect_hand(&pars->cmd[pars->i], pars->cmd[pars->i], &pars->i, &pars->r);
}
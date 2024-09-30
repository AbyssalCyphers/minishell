#include "../../include/minishell.h"

void	print_token(t_token *token)
{
	t_token	*tmp;

	if (token == NULL)
	{
		printf("Token list is empty\n");
		return;
	}
	tmp = token;
	while (tmp->next != token)
	{
		printf("Type : %d, [%s]\n", tmp->type, tmp->str);
		tmp = tmp->next;
	}
	printf("Type : %d, [%s]\n",  tmp->type, tmp->str);
}

void print_command_structure(t_cmd *cmd)
{
    t_cmd *current = cmd;
    int cmd_count = 1;

    while (current != NULL)
    {
        printf("Command %d:\n", cmd_count);
        printf("  Skip: %s\n", current->skip_cmd ? "true" : "false");
        printf("  Infile: %d\n", current->infile);
        printf("  Outfile: %d\n", current->outfile);
        printf("  Command and arguments: ");
        
        if (current->cmd_param)
        {
            for (int i = 0; current->cmd_param[i] != NULL; i++)
            {
                printf("%s ", current->cmd_param[i]);
            }
        }
        printf("\n");

        current = current->next;
        if (current == cmd)
            break;  // We've looped back to the start in the circular list
        cmd_count++;
    }
}

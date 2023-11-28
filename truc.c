/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   truc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:41:04 by rluiz             #+#    #+#             */
/*   Updated: 2023/11/28 18:44:50 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct s_list
{
    int data;
    int op;
    struct s_list *next;
} t_list;

t_list *new_node(int data, int op)
{
    t_list *node = malloc(sizeof(t_list));
    if (node)
    {
        node->data = data;
        node->op = op;
        node->next = NULL;
    }
    return node;
}

void free_list(t_list *list)
{
    while (list)
    {
        t_list *temp = list;
        list = list->next;
        free(temp);
    }
}

int parse(char *str, t_list **list)
{
    int i = 0, num = 0;
    int num_flag = 0;
    while (str[i])
    {
        if (isdigit(str[i]))
        {
            num = num * 10 + (str[i] - '0');
            num_flag = 1;
        }
        else if (isspace(str[i]))
        {
            if (num_flag)
            {
                *list = new_node(num, 0);
                list = &(*list)->next;
                num = 0;
                num_flag = 0;
            }
        }
        else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
        {
            if (num_flag)
            {
                *list = new_node(num, 0);
                list = &(*list)->next;
                num = 0;
                num_flag = 0;
            }
            int op = (str[i] == '+') ? 1 : (str[i] == '-') ? 2 : (str[i] == '*') ? 3 : 4;
            *list = new_node(0, op);
            list = &(*list)->next;
        }
        else
        {
            return 1; // Invalid character found
        }
        i++;
    }
    if (num_flag)
    {
        *list = new_node(num, 0);
    }
    return 0;
}

int calculate(t_list **list)
{
    t_list *current = *list, *prev = NULL;
    int result = 0;
    while (current)
    {
        if (current->op == 0)
        {
            if (!prev)
                result = current->data;
            else
            {
                switch (prev->op)
                {
                case 1:
                    result += current->data;
                    break;
                case 2:
                    result -= current->data;
                    break;
                case 3:
                    result *= current->data;
                    break;
                case 4:
                    if (current->data == 0)
                    {
                        printf("Error: Division by zero\n");
                        return 1;
                    }
                    result /= current->data;
                    break;
                }
            }
        }
        prev = current;
        current = current->next;
    }
    printf("Result: %d\n", result);
    return 0;
}

int main(void)
{
    char str[101];
    while (1)
    {
        printf("Enter expression (or 'exit' to quit): ");
        if (!fgets(str, 100, stdin))
        {
            printf("Input error\n");
            continue;
        }
        if (strncmp(str, "exit", 4) == 0)
            break;

        t_list *list = NULL;
        if (parse(str, &list))
        {
            printf("Input error\n");
            free_list(list);
            continue;
        }
        if (calculate(&list))
        {
            free_list(list);
            continue;
        }
        free_list(list);
    }
    return 0;
}

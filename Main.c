/*
 * Main.c
 *
 * Simple syntax C checker for pairs of round, figure,
 * square brackets, double and single quotes, comments
 * and truth of writing in control characters
 *
 *  Created to: 12 February
 *     Author: SashaMois
 */

#include <stdio.h>

#define MAXLINE 1000000

void syntax_checker (char program[]);

int main()
{
    int c; /* variable for save future input */
    int i; /* count */
    char program[MAXLINE]; /* array for save input program */

    /* clean array */
    for (i = 0; i < MAXLINE; ++i)
        program[i] = 0;

    for (i = 0; (c = getchar()) != EOF; ++i)
        program[i] = c;

    syntax_checker(program);

    return 0;
}

void syntax_checker(char program[])
{
    enum checkers_entry {
        IN_COMMENTS, 
        OUT_COMMENTS, 
        IN_DOUBLE_QUOTES = 0,
        OUT_DOUBLE_QUOTES, 
        IN_SINGLE_QUOTES = 0, 
        OUT_SINGLE_QUOTES,
        IN_CONTROL_CHARACTER = 0,
        OUT_CONTROL_CHARACTER,
    };
    enum borders_or_starts {
        SYMBOLS_IN_CONTROL_CHARS = 1,
    };

    /* variables for discover point in the code */
    int i; /* count */
    int i_control_character = 0;
    int status_for_comments = OUT_COMMENTS;
    int status_for_double_quotes = OUT_DOUBLE_QUOTES;
    int status_for_single_quotes = OUT_SINGLE_QUOTES;
    int status_for_control_character = OUT_CONTROL_CHARACTER;

    /* variables for syntax check */
    int round_brackets_r = 0;
    int round_brackets_l = 0;

    for (i = 0; program[i] != '\0'; ++i) {
        /* check entry in control character */
        if (status_for_double_quotes == IN_DOUBLE_QUOTES 
            || status_for_single_quotes == IN_SINGLE_QUOTES) {
            if (status_for_control_character == OUT_CONTROL_CHARACTER
                && program[i] == '\\')
                status_for_control_character = IN_CONTROL_CHARACTER;

            else if (i_control_character == SYMBOLS_IN_CONTROL_CHARS) {
                i_control_character = 0;
                status_for_control_character = OUT_CONTROL_CHARACTER;
            }
            
            else if (status_for_control_character == IN_CONTROL_CHARACTER)
                ++i_control_character;
        }

        /* check entry in comments */
        if (status_for_double_quotes && status_for_single_quotes) {
            if (status_for_comments == OUT_COMMENTS && program[i] == '/'
                && program[i + 1] == '*')
                status_for_comments = IN_COMMENTS;
            else if (status_for_comments == IN_COMMENTS && program[i] == '*'
                && program[i + 1] == '/')
                status_for_comments = OUT_COMMENTS;
        }

        /* check entry in double_quotes */
        if (status_for_comments && status_for_single_quotes) {
            if (status_for_double_quotes == OUT_DOUBLE_QUOTES && program[i] == '\"')
                status_for_double_quotes = IN_DOUBLE_QUOTES;
            else if (status_for_double_quotes == IN_DOUBLE_QUOTES && program[i] == '\"'
                && status_for_control_character != IN_CONTROL_CHARACTER)
                status_for_double_quotes = OUT_DOUBLE_QUOTES;
        }
        
        /* check entry in single_quotes */
        if (status_for_comments && status_for_double_quotes) {
            if (status_for_single_quotes == OUT_SINGLE_QUOTES && program[i] == '\'')
                status_for_single_quotes = IN_SINGLE_QUOTES;
            else if (status_for_single_quotes == IN_SINGLE_QUOTES && program[i] == '\''
                && status_for_control_character != IN_CONTROL_CHARACTER)
                status_for_single_quotes = OUT_SINGLE_QUOTES;
        }

        if (status_for_comments && status_for_double_quotes && status_for_single_quotes) {
            /* check pair round brackets */
            if (program[i] == '(')
                ++round_brackets_l;
            else if (program[i] == ')') {
                ++round_brackets_r;

                if (round_brackets_l < round_brackets_r) {
                    printf("Quantity of round right brackets more that quantity of"
                           " round left brackets.\n");
                    return;
                }
            }
        }
    }
    
    /* finish check */
    if (round_brackets_l > round_brackets_r) {
        printf("Quantity of round left brackets more that quantity of round right brackets.\n");
        return;
    }

}
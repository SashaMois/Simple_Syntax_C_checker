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
#include <string.h>

#define MAXLINE 1000000

void syntax_checker (char program[]);
int check_entry_in_array_of_control_chars(char char_for_check);

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

    /* variables for syntax check (count of anything) */
    /* round brackets */
    int round_brackets_r = 0;
    int round_brackets_l = 0;
    /* figure brackets */
    int figure_brackets_r = 0;
    int figure_brackets_l = 0;
    /* square brackets */
    int square_brackets_r = 0;
    int square_brackets_l = 0;
    /* double quotes */
    int double_quote_open = 0;
    int double_quote_close = 0;
    /* single quotes */
    int single_quote_open = 0;
    int single_quote_close = 0;
    /* comments */
    int comment_open = 0;
    int comment_close = 0;

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

            else if (status_for_control_character == IN_CONTROL_CHARACTER) {
                if (check_entry_in_array_of_control_chars(program[i]))
                    ++i_control_character;
                else {
                    printf("No such control character exists!");
                    return;
                }
            }
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
            if (status_for_double_quotes == OUT_DOUBLE_QUOTES && program[i] == '\"') {
                status_for_double_quotes = IN_DOUBLE_QUOTES;
                ++double_quote_open;
            }
            else if (status_for_double_quotes == IN_DOUBLE_QUOTES && program[i] == '\"'
                && status_for_control_character != IN_CONTROL_CHARACTER) {
                status_for_double_quotes = OUT_DOUBLE_QUOTES;
                ++double_quote_close;
            }
        }
        
        /* check entry in single_quotes */
        if (status_for_comments && status_for_double_quotes) {
            if (status_for_single_quotes == OUT_SINGLE_QUOTES && program[i] == '\'') {
                status_for_single_quotes = IN_SINGLE_QUOTES;
                ++single_quote_open;
            }
            else if (status_for_single_quotes == IN_SINGLE_QUOTES && program[i] == '\''
                && status_for_control_character != IN_CONTROL_CHARACTER) {
                status_for_single_quotes = OUT_SINGLE_QUOTES;
                ++single_quote_close;
            }
        }

        /* start check truth of writing all brackets*/
        if (status_for_comments && status_for_double_quotes && status_for_single_quotes) {
            /* check pair of round brackets */
            if (program[i] == '(')
                ++round_brackets_l;
            else if (program[i] == ')') {
                ++round_brackets_r;

                if (round_brackets_l < round_brackets_r) {
                    printf("Quantity of round right brackets more that quantity of"
                           " round left brackets!\n");
                    return;
                }
            }

            /* check pair of figure brackets */
            if (program[i] == '{')
                ++figure_brackets_l;
            else if (program[i] == '}') {
                ++figure_brackets_r;

                if (figure_brackets_l < figure_brackets_r) {
                    printf("Quantity of figure right brackets more that quantity of"
                           " figure left brackets!\n");
                    return;
                }
            }

            /* check pair of square brackets */
            if (program[i] == '[')
                ++square_brackets_l;
            else if (program[i] == ']') {
                ++square_brackets_r;
                
                if (square_brackets_l < square_brackets_r) {
                    printf("Quantity of square right brackets more that quantity of"
                           " square left brackets!\n");
                    return;
                }
            }
        }

        /* start check truth of writing comments */
        if (status_for_double_quotes && status_for_single_quotes) {
            if (program[i] == '/' && program[i + 1] == '*'
                && status_for_comments == IN_COMMENTS)
                ++comment_open;
            else if (program[i] == '*' && program[i + 1] == '/'
                     && status_for_comments == OUT_COMMENTS) {
                ++comment_close;

                if (comment_open < comment_close) {
                    printf("Quantity of close symbols comment more that quantity of"
                           " open symbols comment!\n");
                    return;
                }
            }
        }

        }
    
    /* finish check */
    if (round_brackets_l > round_brackets_r) {
        printf("Quantity of round left brackets more that quantity of round right brackets!\n");
        return;
    }

    if (figure_brackets_l > figure_brackets_r) {
        printf("Quantity of figure left brackets more that quantity of figure right brackets!\n");
        return;
    }

    if (square_brackets_l > square_brackets_r) {
        printf("Quantity of square left brackets more that quantity of square right brackets!\n");
        return;
    }

    if (double_quote_open > double_quote_close) {
        printf("You forgot to close the double quotes!\n");
        return;
    }

    if (single_quote_open > single_quote_close) {
        printf("You forgot to close the single quotes!\n");
        return;
    }
    
    if (comment_open > comment_close) {
        printf("Quantity of open symbols comment more that quantity of"
               " close symbols comment!\n");
        return;
    }

}

int check_entry_in_array_of_control_chars(char char_for_check)
{
    enum {
        NO,
        YES,
    };

    char control_characters[] = "abfnrtv\\?\'\"";

    for (int i = 0; i < strlen(control_characters); ++i)
        if (char_for_check == control_characters[i])
            return YES;
    return NO;
}
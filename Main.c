#include <stdio.h>
#include <string.h>

/*
 * Main.c
 *
 * Simple syntax C checker for pairs of round, figure,
 * square brackets, double and single quotes, comments
 * and truth of writing in control characters
 *
 */

#define MAXLINE 1000000

void syntax_checker (char program[]);
int is_entry_in_control_chars_list(char symbol);

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
        SYMBOLS_IN_SINGLE_Q = 1,
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
    /* count of characters in single quotes */
    int counter_chars_in_single_q = 0;
    int max_chars_in_single_q = SYMBOLS_IN_SINGLE_Q;

    int line_number = 1; /* number of current line in program */

    /* memos for lines with any opening language design */
    int round_bracket_memor = 0; /* memorization for line with open round bracket */
    int figure_bracket_memor = 0; /* memorization for line with open figure bracket */
    int square_bracket_memor = 0; /* memorization for line with open square bracket */
    int double_quote_memor = 0;  /* memorization for line with open double quote */ 
    int single_quote_memor = 0; /* memorization for line with open single quote */ 
    int comment_memor = 0; /* memorization for line with open comment */ 

    for (i = 0; program[i] != '\0'; ++i) {
        /* check entry in control character */
        if (status_for_double_quotes == IN_DOUBLE_QUOTES 
            || status_for_single_quotes == IN_SINGLE_QUOTES) {

            if (i_control_character == SYMBOLS_IN_CONTROL_CHARS) {
                i_control_character = 0;
                status_for_control_character = OUT_CONTROL_CHARACTER;
            }
            
            else if (status_for_control_character == IN_CONTROL_CHARACTER) {
                if (is_entry_in_control_chars_list(program[i]))
                    ++i_control_character;
                else {
                    printf("No such control character exists in %d line!\n", line_number);
                    return;
                }
            }

            if (status_for_control_character == OUT_CONTROL_CHARACTER
                && program[i] == '\\') {
                status_for_control_character = IN_CONTROL_CHARACTER;

                ++max_chars_in_single_q;
                ++counter_chars_in_single_q;
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

                if (!double_quote_memor)
                    double_quote_memor = line_number;
            }
            else if (status_for_double_quotes == IN_DOUBLE_QUOTES && program[i] == '\"'
                && status_for_control_character != IN_CONTROL_CHARACTER) {
                status_for_double_quotes = OUT_DOUBLE_QUOTES;
                ++double_quote_close;

                if (double_quote_open == double_quote_close)
                    double_quote_memor = 0;
            }   
        }
        
        /* check entry in single_quotes */
        if (status_for_comments && status_for_double_quotes) {
            if (status_for_single_quotes == OUT_SINGLE_QUOTES && program[i] == '\'') {
                status_for_single_quotes = IN_SINGLE_QUOTES;
                ++single_quote_open;

                if (!single_quote_memor)
                    single_quote_memor = line_number;
            }
            else if (status_for_single_quotes == IN_SINGLE_QUOTES && program[i] == '\''
                && status_for_control_character != IN_CONTROL_CHARACTER) {
                status_for_single_quotes = OUT_SINGLE_QUOTES;
                ++single_quote_close;

                if (single_quote_open == single_quote_close)
                    single_quote_memor = 0;
            }
            else if (status_for_single_quotes == IN_SINGLE_QUOTES) {
                if (program[i] != '\\')
                    ++counter_chars_in_single_q;

                if (counter_chars_in_single_q == max_chars_in_single_q && program[i + 1] != '\'') {
                    printf("Single quotation marks exceed allowed in %d line!\n", line_number);
                    return;
                }
                else if (counter_chars_in_single_q == max_chars_in_single_q) {
                    max_chars_in_single_q = SYMBOLS_IN_SINGLE_Q;
                    counter_chars_in_single_q = 0; 
                }
            }
            
        }

        /* start check truth of writing all brackets*/
        if (status_for_comments && status_for_double_quotes && status_for_single_quotes) {
            /* check pair of round brackets */
            if (program[i] == '(') {
                ++round_brackets_l;
                if (!round_bracket_memor)
                    round_bracket_memor = line_number;
            }
            else if (program[i] == ')') {
                ++round_brackets_r;

                if (round_brackets_l < round_brackets_r) {
                    printf("Quantity of round right brackets more that quantity of"
                           " round left brackets in %d line!\n", line_number);
                    return;
                }

                if (round_brackets_l == round_brackets_r)
                    round_bracket_memor = 0;
            }

            /* check pair of figure brackets */
            if (program[i] == '{') {
                ++figure_brackets_l;
                if (!figure_bracket_memor)
                    figure_bracket_memor = line_number;
            }
            else if (program[i] == '}') {
                ++figure_brackets_r;

                if (figure_brackets_l < figure_brackets_r) {
                    printf("Quantity of figure right brackets more that quantity of"
                           " figure left brackets in %d line!\n", line_number);
                    return;
                }

                if (figure_brackets_l == figure_brackets_r)
                    figure_bracket_memor = 0;
            }

            /* check pair of square brackets */
            if (program[i] == '[') {
                ++square_brackets_l;
                if (!square_bracket_memor)
                    square_bracket_memor = line_number;
            }
            else if (program[i] == ']') {
                ++square_brackets_r;
                
                if (square_brackets_l < square_brackets_r) {
                    printf("Quantity of square right brackets more that quantity of"
                           " square left brackets in %d line!\n", line_number);
                    return;
                }

                if (square_brackets_l == square_brackets_r)
                    square_bracket_memor = 0;
            }
        }

        /* start check truth of writing comments */
        if (status_for_double_quotes && status_for_single_quotes) {
            if (program[i] == '/' && program[i + 1] == '*'
                && status_for_comments == IN_COMMENTS) {
                ++comment_open;
                if (!comment_memor)
                    comment_memor = line_number;
            }
            else if (program[i] == '*' && program[i + 1] == '/'
                     && status_for_comments == OUT_COMMENTS) {
                ++comment_close;

                if (comment_open < comment_close) {
                    printf("Quantity of close symbols comment more that quantity of"
                           " open symbols comment in %d line!\n", line_number);
                    return;
                }

                if (comment_open == comment_close)
                    comment_memor = 0;
            }
        }

        if (program[i] == '\n')
            ++line_number;

    }
    
    /* finish check */
    int i_truth = 0; /* variable for count of true statements */

    enum {
        MAX_COUNT_OF_TRUTH_STATEMENTS = 6,
    };

    if (round_brackets_l > round_brackets_r) {
        printf("Quantity of round left brackets more that quantity of"
               " round right brackets in %d line!\n", round_bracket_memor);
        return;
    }
    ++i_truth;

    if (figure_brackets_l > figure_brackets_r) {
        printf("Quantity of figure left brackets more that quantity of"
               " figure right brackets in %d line!\n", figure_bracket_memor);
        return;
    }
    ++i_truth;

    if (square_brackets_l > square_brackets_r) {
        printf("Quantity of square left brackets more that quantity of"
               " square right brackets in %d line!\n", square_bracket_memor);
        return;
    }
    ++i_truth;

    if (double_quote_open > double_quote_close) {
        printf("You forgot to close the double quotes in %d line!\n", double_quote_memor);
        return;
    }
    ++i_truth;

    if (single_quote_open > single_quote_close) {
        printf("You forgot to close the single quotes in %d line!\n", single_quote_memor);
        return;
    }
    ++i_truth;
    
    if (comment_open > comment_close) {
        printf("Quantity of open symbols comment more that quantity of"
               " close symbols comment in %d line!\n", comment_memor);
        return;
    }
    ++i_truth;

    printf("All right! Let's start the program!\n");

}

int is_entry_in_control_chars_list(char symbol)
{
    char control_characters[] = "abfnrtv\\?'\"0"; /* list of control characters in C */
    int i;

    enum {
        NO,
        YES,
    };

    for (i = 0; i < strlen(control_characters); ++i)
        if (control_characters[i] == symbol)
            return YES;
    return NO;
}
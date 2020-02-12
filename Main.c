/*
 * Main.c
 *
 * Simple syntax checker for C programs.
 *
 * It checks unpaired round, square, figure brackets
 * and single and double quotes, control characters,
 * comments.
 *
 *  Created on: 1 February 2020 y.
 *      Author: sashamois
 */

#include <stdio.h>

 /****************************************/
		  /* constants */
#define MAXLINE 100000
#define GOOD 0
#define ERROR 1
#define ERROR_OF_SINGLE_QUOTE 2
#define YES 1
#define NO 0
#define START_LINE 1 /* number of first line in any C program */
#define OUT_COMMENT 1
#define IN_COMMENT 0
#define OUT_DOUBLE_QUOTES 1
#define IN_DOUBLE_QUOTES 0
#define OUT_SINGLE_QUOTES 1
#define IN_SINGLE_QUOTES 0
#define ALL_RIGHT 7 /* this program has been 7 syntax checkers */
#define MAX_SYMBOLS_IN_SINGLE_QUOTES 3
		 /**************/
/****************************************/

/* function for calls all check functions */
void syntax_checker(char program_for_check[]);

/***************************************************************************/
				  /* check functions */
int check_pair_round_brackets(char program_for_check[]);
int check_pair_figure_brackets(char program_for_check[]);
int check_pair_square_brackets(char program_for_check[]);
int check_pair_double_quotes(char program_for_check[]);
int check_pair_single_quotes(char program_for_check[]);
int check_pair_comments(char program_for_check[]);
int check_control_characters(char program_for_check[]);
int is_cmp_with_control_character(int symbol_for_cmp);
/*******************/
/***************************************************************************/

main()
{
	char program[MAXLINE]; /* input program */
	int c; /* saver for input symbols */
	int i; /* count */

	/* gets inputed program */
	for (i = 0; (c = getchar()) != EOF; ++i)
		program[i] = c;
	program[i] = '\0';

	syntax_checker(program);

	return 0;
}

void syntax_checker(char program[])
{
	int right = 0; /* count for good checkers */
	int function_status = 0; /* status of function work */

	if ((function_status = check_pair_comments(program)) == ERROR) {
		printf("\n\nStatus:\n*ERROR*\tcheck pair of comments\n\n");
		printf("Checker - 'check_pair_comments'\n");
	}
	else if (function_status == ERROR_OF_SINGLE_QUOTE)
		return;
	else
		++right;

	if ((function_status = check_pair_round_brackets(program)) == ERROR) {
		printf("\n\nStatus:\n*ERROR*\tcheck pair of round brackets\n\n");
		printf("Checker - 'check_pair_round_brackets'\n");
	}
	else if (function_status == ERROR_OF_SINGLE_QUOTE)
		return;
	else
		++right;

	if ((function_status = check_pair_figure_brackets(program)) == ERROR) {
		printf("\n\nStatus:\n*ERROR*\tcheck pair of figure brackets\n\n");
		printf("Checker - 'check_pair_figure_brackets'");
	}
	else if (function_status == ERROR_OF_SINGLE_QUOTE)
		return;
	else
		++right;

	if ((function_status = check_pair_square_brackets(program)) == ERROR) {
		printf("\n\nStatus:\n*ERROR*\tcheck pair of square brackets\n\n");
		printf("Checker - 'check_pair_square_brackets'\n");
	}
	else if (function_status == ERROR_OF_SINGLE_QUOTE)
		return;
	else
		++right;

	if ((function_status = check_pair_double_quotes(program)) == ERROR) {
		printf("\n\nStatus:\n*ERROR*\tcheck pair of double quotes\n\n");
		printf("Checker - 'check_pair_double_quotes'\n");
	}
	else if (function_status == ERROR_OF_SINGLE_QUOTE)
		return;
	else
		++right;

	if ((function_status = check_pair_single_quotes(program)) == ERROR) {
		printf("\n\nStatus:\n*ERROR*\tcheck pair of single quotes\n\n");
		printf("Checker = 'check_pair_single_quotes'\n");
	}
	else if (function_status == ERROR_OF_SINGLE_QUOTE)
		return;
	else
		++right;

	if (check_control_characters(program)) {
		printf("\n\nStatus:\n*ERROR*\tcheck truth of control characters\n\n");
		printf("Checker = 'check_control_characters'\n");
	}
	++right;

	if (right == ALL_RIGHT) /* all checkers is good */
		printf("\n\nStatus:\n*All right*");
}

int check_pair_round_brackets(char program[])
{
	int i; /* count */
	int count_of_symbols_in_single_quotes = 0; /* count of symbols when single quotes will stop (3 or 4) */
	int max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES; /* border for a higher-level variable */
	int number_of_line = START_LINE; /* number of line where code have written */
	int check_bracket_l = 0; /* check count of left brackets */
	int check_bracket_r = 0; /* check count of right brackets */
	int status_for_comments = OUT_COMMENT; /* is there not round bracket in comment */
	int status_for_double_quotes = OUT_DOUBLE_QUOTES; /* is there not round bracket in double quotes */
	int status_for_single_quotes = OUT_SINGLE_QUOTES; /* is there not round bracket in single quotes */

	/* starts verify */
	for (i = 0; program[i] != '\0'; ++i) {
		if (status_for_double_quotes) {
			/* is it in comment? */
			if (program[i] == '/' && program[i + 1] == '*')
				status_for_comments = IN_COMMENT;
			else if (program[i] == '*' && program[i + 1] == '/') {
				++i;
				status_for_comments = OUT_COMMENT;
			}
		}

		if (status_for_comments) {
			/* is it in double quotes? */
			if (program[i] == '\"' && status_for_double_quotes && program[i - 1] != '\\')
				status_for_double_quotes = IN_DOUBLE_QUOTES;
			else if (program[i] == '\"' && status_for_double_quotes == IN_DOUBLE_QUOTES && program[i - 1] != '\\')
				status_for_double_quotes = OUT_DOUBLE_QUOTES;
		}

		if (status_for_comments && status_for_double_quotes) {
			/* is it in single quotes? */
			if (program[i] == '\'' && status_for_single_quotes && program[i - 1] != '\\')
				status_for_single_quotes = IN_SINGLE_QUOTES;
			else if (program[i] == '\'' && status_for_single_quotes == IN_SINGLE_QUOTES && program[i + 1] != '\\') {
				max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES;
				count_of_symbols_in_single_quotes = 0;
				status_for_single_quotes = OUT_SINGLE_QUOTES;
			}
			else if (status_for_single_quotes == IN_SINGLE_QUOTES && program[i] == '\\' && program[i - 1] != '\\')
				++max_count_of_symbols_in_single_quotes;
			else if (status_for_single_quotes == IN_SINGLE_QUOTES &&
				count_of_symbols_in_single_quotes == max_count_of_symbols_in_single_quotes) {
				printf("\nCheck close single quote in %d line.\n", number_of_line);
				printf("\n\nStatus:\n*ERROR*\t close single quote is absent.\n");
				printf("Checker - 'check_pair_round_brackets'\n");
				return ERROR_OF_SINGLE_QUOTE;
			}
			if (status_for_single_quotes == IN_SINGLE_QUOTES)
				++count_of_symbols_in_single_quotes;
		}

		if (status_for_comments && status_for_double_quotes && status_for_single_quotes) {
			/* verifies compliance of round brackets */
			if (program[i] == '(')
				++check_bracket_l;
			else if (program[i] == ')') {
				++check_bracket_r;

				if (check_bracket_r > check_bracket_l) { /* count of right brackets always equally or less *
														  * than count of left brackets                    */
					printf("\nThere is no compliance of round brackets in %d line.\n", number_of_line);
					return ERROR;
				}
			}

			if (program[i] == '\n')
				++number_of_line;
		}
	}

	if (check_bracket_l == check_bracket_r)
		return GOOD;
	else
		return ERROR;

}

int check_pair_figure_brackets(char program[])
{
	int i; /* count */
	int count_of_symbols_in_single_quotes = 0; /* count of symbols when single quotes will stop (3 or 4) */
	int max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES; /* border for a higher-level variable */
	int number_of_line = START_LINE; /* number of line where code have written */
	int check_bracket_l = 0; /* check count of left brackets */
	int check_bracket_r = 0; /* check count of right brackets */
	int status_for_comments = OUT_COMMENT; /* is there not figure bracket in comment */
	int status_for_double_quotes = OUT_DOUBLE_QUOTES; /* is there not figure bracket in double quotes */
	int status_for_single_quotes = OUT_SINGLE_QUOTES; /* is there not figure bracket in single quotes */

	/* starts verify */
	for (i = 0; program[i] != '\0'; ++i) {
		if (status_for_double_quotes) {
			/* is it in comment? */
			if (program[i] == '/' && program[i + 1] == '*')
				status_for_comments = IN_COMMENT;
			else if (program[i] == '*' && program[i + 1] == '/') {
				++i;
				status_for_comments = OUT_COMMENT;
			}
		}

		if (status_for_comments) {
			/* is it in double quotes? */
			if (program[i] == '\"' && status_for_double_quotes && program[i - 1] != '\\')
				status_for_double_quotes = IN_DOUBLE_QUOTES;
			else if (program[i] == '\"' && status_for_double_quotes == IN_DOUBLE_QUOTES && program[i - 1] != '\\')
				status_for_double_quotes = OUT_DOUBLE_QUOTES;
		}

		if (status_for_comments && status_for_double_quotes) {
			/* is it in single quotes? */
			if (program[i] == '\'' && status_for_single_quotes && program[i - 1] != '\\')
				status_for_single_quotes = IN_SINGLE_QUOTES;
			else if (program[i] == '\'' && status_for_single_quotes == IN_SINGLE_QUOTES && program[i + 1] != '\'') {
				max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES;
				count_of_symbols_in_single_quotes = 0;
				status_for_single_quotes = OUT_SINGLE_QUOTES;
			}
			else if (status_for_single_quotes == IN_SINGLE_QUOTES && program[i] == '\\' && program[i - 1] != '\\')
				++max_count_of_symbols_in_single_quotes;
			else if (status_for_single_quotes == IN_SINGLE_QUOTES &&
				count_of_symbols_in_single_quotes == max_count_of_symbols_in_single_quotes) {
				printf("\nCheck close single quote in %d line.\n", number_of_line);
				printf("\n\nStatus:\n*ERROR*\t close single quote is absent.\n");
				printf("Checker - 'check_pair_figure_brackets'\n");
				return ERROR_OF_SINGLE_QUOTE;
			}
			if (status_for_single_quotes == IN_SINGLE_QUOTES)
				++count_of_symbols_in_single_quotes;
		}

		if (status_for_comments && status_for_double_quotes && status_for_single_quotes) {
			/* verifies compliance of figure brackets */
			if (program[i] == '{')
				++check_bracket_l;
			else if (program[i] == '}') {
				++check_bracket_r;

				if (check_bracket_r > check_bracket_l) { /* count of right brackets always equally or less *
															  * than count of left brackets                    */
					printf("\nThere is no compliance of figure brackets in %d line.\n", number_of_line);
					return ERROR;
				}
			}

			if (program[i] == '\n')
				++number_of_line;
		}
	}

	if (check_bracket_l == check_bracket_r)
		return GOOD;
	else
		return ERROR;
}

int check_pair_square_brackets(char program[])
{
	int i; /* count */
	int count_of_symbols_in_single_quotes = 0; /* count of symbols when single quotes will stop (3 or 4) */
	int max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES; /* border for a higher-level variable */
	int number_of_line = START_LINE; /* number of line where code have written */
	int check_bracket_l = 0; /* check count of left brackets */
	int check_bracket_r = 0; /* check count of right brackets */
	int status_for_comments = OUT_COMMENT; /* is there not square bracket in comment */
	int status_for_double_quotes = OUT_DOUBLE_QUOTES; /* is there not square bracket in double quotes */
	int status_for_single_quotes = OUT_SINGLE_QUOTES; /* is there not square bracket in single quotes */

	/* starts verify */
	for (i = 0; program[i] != '\0'; ++i) {
		if (status_for_double_quotes) {
			/* is it in comment? */
			if (program[i] == '/' && program[i + 1] == '*')
				status_for_comments = IN_COMMENT;
			else if (program[i] == '*' && program[i + 1] == '/') {
				++i;
				status_for_comments = OUT_COMMENT;
			}
		}

		if (status_for_comments) {
			/* is it in double quotes? */
			if (program[i] == '\"' && status_for_double_quotes && program[i - 1] != '\\')
				status_for_double_quotes = IN_DOUBLE_QUOTES;
			else if (program[i] == '\"' && status_for_double_quotes == IN_DOUBLE_QUOTES && program[i - 1] != '\\')
				status_for_double_quotes = OUT_DOUBLE_QUOTES;
		}

		if (status_for_comments && status_for_double_quotes) {
			/* is it in single quotes? */
			if (program[i] == '\'' && status_for_single_quotes && program[i - 1] != '\\')
				status_for_single_quotes = IN_SINGLE_QUOTES;
			else if (program[i] == '\'' && status_for_single_quotes == IN_SINGLE_QUOTES && program[i + 1] != '\'') {
				max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES;
				count_of_symbols_in_single_quotes = 0;
				status_for_single_quotes = OUT_SINGLE_QUOTES;
			}
			else if (status_for_single_quotes == IN_SINGLE_QUOTES && program[i] == '\\' && program[i - 1] != '\\')
				++max_count_of_symbols_in_single_quotes;
			else if (status_for_single_quotes == IN_SINGLE_QUOTES &&
				count_of_symbols_in_single_quotes == max_count_of_symbols_in_single_quotes) {
				printf("\nCheck close single quote in %d line.\n", number_of_line);
				printf("\n\nStatus:\n*ERROR*\t close single quote is absent.\n");
				printf("Checker - 'check_pair_square_brackets'");
				return ERROR_OF_SINGLE_QUOTE;
			}
			if (status_for_single_quotes == IN_SINGLE_QUOTES)
				++count_of_symbols_in_single_quotes;
		}

		if (status_for_comments && status_for_double_quotes && status_for_single_quotes) {
			/* verifies compliance of square brackets */
			if (program[i] == '[')
				++check_bracket_l;
			else if (program[i] == ']') {
				++check_bracket_r;

				if (check_bracket_r > check_bracket_l) { /* count of right brackets always equally or less *
															  * than count of left brackets                    */
					printf("\nThere is no compliance of square brackets in %d line.\n", number_of_line);
					return ERROR;
				}
			}

			if (program[i] == '\n')
				++number_of_line;
		}
	}

	if (check_bracket_l == check_bracket_r)
		return GOOD;
	else
		return ERROR;
}

int check_pair_double_quotes(char program[])
{
	int i; /* count */
	int count_of_symbols_in_single_quotes = 0; /* count of symbols when single quotes will stop (3 or 4) */
	int max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES; /* border for a higher-level variable */
	int number_of_line = START_LINE; /* number of line where code have written */
	int check_double_quote_l = 0; /* check count of left double quotes */
	int check_double_quote_r = 0; /* check count of right double quotes */
	int status_for_double_quotes = OUT_DOUBLE_QUOTES; /* it is for count of double right and left quotes */
	int status_for_comments = OUT_COMMENT; /* is there not double quotes in comment */
	int status_for_single_quotes = OUT_SINGLE_QUOTES; /* is there not double quotes in single quotes */

	/* starts verify */
	for (i = 0; program[i] != '\0'; ++i) {
		/* is it in comment? */
		if (program[i] == '/' && program[i + 1] == '*')
			status_for_comments = IN_COMMENT;
		else if (program[i] == '*' && program[i + 1] == '/') {
			++i;
			status_for_comments = OUT_COMMENT;
		}

		if (status_for_comments) {
			/* is it in single quotes? */
			if (program[i] == '\'' && status_for_single_quotes && program[i - 1] != '\\')
				status_for_single_quotes = IN_SINGLE_QUOTES;
			else if (program[i] == '\'' && status_for_single_quotes == IN_SINGLE_QUOTES && program[i + 1] != '\'') {
				max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES;
				count_of_symbols_in_single_quotes = 0;
				status_for_single_quotes = OUT_SINGLE_QUOTES;
			}
			else if (status_for_single_quotes == IN_SINGLE_QUOTES && program[i] == '\\' && program[i - 1] != '\\')
				++max_count_of_symbols_in_single_quotes;
			else if (status_for_single_quotes == IN_SINGLE_QUOTES &&
				count_of_symbols_in_single_quotes == max_count_of_symbols_in_single_quotes) {
				printf("\nCheck close single quote\n");
				printf("\n\nStatus:\n*ERROR*\t close single quote is absent.\n");
				printf("Checker - 'check_pair_double_quotes'\n");
				return ERROR_OF_SINGLE_QUOTE;
			}
			if (status_for_single_quotes == IN_SINGLE_QUOTES)
				++count_of_symbols_in_single_quotes;
		}

		if (status_for_comments && status_for_single_quotes) {
			/* verifies compliance of double quotes */
			if (program[i] == '\"' && status_for_double_quotes && program[i - 1] != '\\') {
				++check_double_quote_l;
				status_for_double_quotes = IN_DOUBLE_QUOTES;
			}
			else if (program[i] == '\"' && status_for_double_quotes == IN_DOUBLE_QUOTES && program[i - 1] != '\\') {
				++check_double_quote_r;
				status_for_double_quotes = OUT_DOUBLE_QUOTES;
			}
			if (program[i] == '\n')
				++number_of_line;
		}
	}

	if (check_double_quote_l == check_double_quote_r)
		return GOOD;
	else
		return ERROR;
}

int check_pair_single_quotes(char program[])
{
	int i; /* count */
	int count_of_symbols_in_single_quotes = 0; /* count of symbols when single quotes will stop (3 or 4) */
	int max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES; /* border for a higher-level variable */
	int number_of_line = START_LINE; /* number of line where code have written */
	int check_single_quote_l = 0; /* check count of left single quotes */
	int check_single_quote_r = 0; /* check count of right single quotes */
	int status_for_double_quotes = OUT_DOUBLE_QUOTES; /* is there single quote in double quotes */
	int status_for_comments = OUT_COMMENT; /* is there single quotes in comment */
	int status_for_single_quotes = OUT_SINGLE_QUOTES; /* it is for count of single right and left quotes */

	/* starts verify */
	for (i = 0; program[i] != '\0'; ++i) {
		/* is it in comment? */
		if (status_for_double_quotes) {
			if (program[i] == '/' && program[i + 1] == '*')
				status_for_comments = IN_COMMENT;
			else if (program[i] == '*' && program[i + 1] == '/') {
				++i;
				status_for_comments = OUT_COMMENT;
			}
		}

		if (status_for_comments) {
			/* is it in double quotes? */
			if (program[i] == '\"' && status_for_double_quotes && program[i - 1] != '\\')
				status_for_double_quotes = IN_DOUBLE_QUOTES;
			else if (program[i] == '\"' && status_for_double_quotes == IN_DOUBLE_QUOTES && program[i - 1] != '\\')
				status_for_double_quotes = OUT_DOUBLE_QUOTES;
		}

		if (status_for_comments && status_for_double_quotes) {
			/* verifies compliance of single quotes */
			if (program[i] == '\'' && status_for_single_quotes && program[i - 1] != '\\') {
				++check_single_quote_l;
				++count_of_symbols_in_single_quotes;
				status_for_single_quotes = IN_SINGLE_QUOTES;
			}
			else if (program[i] == '\'' && status_for_single_quotes == IN_SINGLE_QUOTES && program[i + 1] != '\'') {
				++check_single_quote_r;
				max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES;
				count_of_symbols_in_single_quotes = 0;
				status_for_single_quotes = OUT_SINGLE_QUOTES;
			}
			else if (status_for_single_quotes == IN_SINGLE_QUOTES && program[i] == '\\' && program[i - 1] != '\\')
				++max_count_of_symbols_in_single_quotes;
			else if (status_for_single_quotes == IN_SINGLE_QUOTES &&
				count_of_symbols_in_single_quotes == max_count_of_symbols_in_single_quotes) {
				printf("\nCheck close single quote\n in %d line.", number_of_line);
				printf("\n\nStatus:\n*ERROR*\t close single quote is absent.\n");
				printf("Checker - 'check_pair_single_quotes'\n");
				return ERROR_OF_SINGLE_QUOTE;
			}
			if (status_for_single_quotes == IN_SINGLE_QUOTES)
				++count_of_symbols_in_single_quotes;

			if (program[i] == '\n')
				++number_of_line;
		}
	}

	if (check_single_quote_l == check_single_quote_r)
		return GOOD;
	else
		return ERROR;
}

int check_pair_comments(char program[])
{
	int i; /* count */
	int count_of_symbols_in_single_quotes = 0; /* count of symbols when single quotes will stop (3 or 4) */
	int max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES; /* border for a higher-level variable */
	int number_of_line = START_LINE; /* number of line where code have written */
	int check_comment_l = 0; /* check count of left comments */
	int check_comment_r = 0; /* check count of right comments */
	int status_for_double_quotes = OUT_DOUBLE_QUOTES; /* is there not comment in double quotes */
	int status_for_single_quotes = OUT_SINGLE_QUOTES; /* is there not comment in single quotes */

	/* starts verify */
	for (i = 0; program[i] != '\0'; ++i) {
		/* is it in double quotes? */
		if (program[i] == '\"' && status_for_double_quotes && program[i - 1] != '\\')
			status_for_double_quotes = IN_DOUBLE_QUOTES;
		else if (program[i] == '\"' && status_for_double_quotes == IN_DOUBLE_QUOTES && program[i - 1] != '\\')
			status_for_double_quotes = OUT_DOUBLE_QUOTES;

		if (status_for_double_quotes) {
			/* is it in single quotes? */
			if (program[i] == '\'' && status_for_single_quotes && program[i - 1] != '\\')
				status_for_single_quotes = IN_SINGLE_QUOTES;
			else if (program[i] == '\'' && status_for_single_quotes == IN_SINGLE_QUOTES && program[i + 1] != '\'') {
				max_count_of_symbols_in_single_quotes = MAX_SYMBOLS_IN_SINGLE_QUOTES;
				count_of_symbols_in_single_quotes = 0;
				status_for_single_quotes = OUT_SINGLE_QUOTES;
			}
			else if (status_for_single_quotes == IN_SINGLE_QUOTES && program[i] == '\\' && program[i - 1] != '\\')
				++max_count_of_symbols_in_single_quotes;
			else if (status_for_single_quotes == IN_SINGLE_QUOTES &&
				count_of_symbols_in_single_quotes == max_count_of_symbols_in_single_quotes) {
				printf("\nCheck close single quote in %d line.\n", number_of_line);
				printf("\n\nStatus:\n*ERROR*\t close single quote is absent.\n");
				printf("Checker - 'check_pair_comments'\n");
				return ERROR_OF_SINGLE_QUOTE;
			}
			if (status_for_single_quotes == IN_SINGLE_QUOTES)
				++count_of_symbols_in_single_quotes;
		}

		if (status_for_double_quotes && status_for_single_quotes) {
			/* verifies compliance of comments */
			if (program[i] == '/' && program[i + 1] == '*')
				++check_comment_l;
			else if (program[i] == '*' && program[i + 1] == '/') {
				++i;
				++check_comment_r;

				if (check_comment_r > check_comment_l) { /* count of right comments always equally or less *
														  *         than count of left comments            */
					printf("\nThere is no compliance of comments in %d line.\n", number_of_line);
					return ERROR;
				}
			}

			if (program[i] == '\n')
				++number_of_line;
		}
	}

	if (check_comment_l == check_comment_r)
		return GOOD;
	else
		return ERROR;
}

int check_control_characters(char program[])
{
	int i; /* count */
	int number_of_line = START_LINE; /* number of line where code have written */

	/* starts verify */
	for (i = 0; program[i] != '\0'; ++i) {
		if (program[i] == '\n')
			++number_of_line;
		else if (program[i] == '\\' && !is_cmp_with_control_character(program[i + 1])) {
			printf("\nUnknown character in %d line.\n", number_of_line);
			return ERROR;
		}
	}

	return GOOD;
}


int is_cmp_with_control_character(int symbol)
{
	char control_characters[] = "abfnrtv\\\?\'\"oh0"; /*    array with all       *
													  * control characters in C */
	for (int i = 0; control_characters[i] != '\0'; ++i)
		if (control_characters[i] == symbol)
			return YES;
	return NO;
}
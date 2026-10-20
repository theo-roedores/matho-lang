#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define T_NUMBER 0
#define T_OPERATOR_ADD '+'
#define T_OPERATOR_SUB '-'
#define T_OPERATOR_DIV '/'
#define T_OPERATOR_MUL '*'
#define T_CLOSE_PAREN ')'
#define T_OPEN_PAREN '('
#define T_OPERATOR_POWER '^'
#define T_END 5

#define i64 long long int
#define i32 int
#define i8 char

#define u64 unsigned long long int
#define u32 unsigned int
#define u8 unsigned char

#define f64 double

#define b8 bool

#define HIGH_PRIORITY 3
#define MID_PRIORITY 2
#define LOW_PRIORITY 1

f64 pwr(f64 n, f64 power) {
	if (power <= 1) return n;
	return n * pwr(n, power-1);
}

b8 is_space(char c) { return c == ' '; }
b8 is_end_of_line(char c) { return c == '\0'; }

u8 token_get_priority(u8 token_type) {
	switch (token_type) {
		case T_OPERATOR_POWER:
			return HIGH_PRIORITY;
		case T_OPERATOR_DIV:
		case T_OPERATOR_MUL:
			return MID_PRIORITY;
		case T_OPERATOR_SUB:
		case T_OPERATOR_ADD:
			return LOW_PRIORITY;
		default:
			return 0;
	}
}

b8 token_is_operator(u8 token_type) {
	switch (token_type) {
		case T_OPERATOR_SUB:
		case T_OPERATOR_ADD:
		case T_OPERATOR_MUL:
		case T_OPERATOR_DIV:
		case T_OPERATOR_POWER:
			return true;
		default:
			return false;
	}
}

u8 char_get_token_type(char c) {
	switch (c) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
			return T_NUMBER;
		case '+':
			return T_OPERATOR_ADD;
		case '-':
			return T_OPERATOR_SUB;
		case '*':
			return T_OPERATOR_MUL;
		case '/':
			return T_OPERATOR_DIV;
		case '^':
			return T_OPERATOR_POWER;
		case '(':
			return T_OPEN_PAREN;
		case ')':
			return T_CLOSE_PAREN;
		default:
			return T_END;
	}
}

u64 str_get_size(const char *str) {
	u64 index = 0;
	while (is_end_of_line(str[index]) == false) {
		index++;
	}
	return index+1;
}

char* str_clear_spaces(const char *str) {
	u64 index = 0;
	while (is_end_of_line(str[index]) == false) {
		index++;
	}

	char *new = malloc(index+1);
	index = 0;
	u64 new_index = 0;
	while (is_end_of_line(str[index]) == false) {
		if (is_space(str[index]) == false) {
			new[new_index] = str[index];
			new_index++;
		}
		index++;
	}

	new[new_index] = '\0';

	return new;
}

typedef struct abstract_syntax_tree_node {
	struct abstract_syntax_tree_node* left;
	struct abstract_syntax_tree_node* right;
	u8 type;
	f64 number;
} abstract_syntax_tree;

typedef struct token_list_node {
	struct token_list_node* next;
	struct token_list_node* prev;
	u8 type;
	f64 number;
} token_list;

/*
 * Cria um novo node de token, leva como parametro o node passado.
 */
struct token_list_node* token_list_new(token_list* parent) {
	struct token_list_node* new_node = malloc(sizeof(struct token_list_node));
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->type = T_END;
	new_node->number = 0;

	if (parent != NULL) {
		new_node->prev = parent;
		parent->next = new_node;
	}

	return new_node;
}

/*
 * Limpa a lista de tokens
 */
void token_list_clear(token_list *list) {
	if (list != NULL) {
		if (list->next != NULL) {
			token_list_clear(list->next);
		}
		free(list);
	}
}

/*
 * Cria um brench novo para a arvore de sintax abstrata.
 */
struct abstract_syntax_tree_node* ast_new() {
	struct abstract_syntax_tree_node* new_node = malloc(sizeof(struct abstract_syntax_tree_node));
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->type = T_END;
	new_node->number = 0;
	return new_node;
}

/*
 * Limpa a árvore de sintax abstrata.
 */
void ast_clear(abstract_syntax_tree* tree) {
	if (tree == NULL) {
		return;
	}

	ast_clear(tree->left);
	ast_clear(tree->right);
	free(tree);
}

/*
 * Transforma os caracteres passados pelo parametro input
 * em uma lista de predeterminados tokens.
 */
token_list* lexer(const char *input) {
	char *buf = str_clear_spaces(input);

	token_list* tokens = token_list_new(NULL);
	token_list* current = tokens;
		
	u64 buf_len = str_get_size(buf);	
	u64 buf_index = 0;

	u8 last_token_type = T_END;

	char number[32] = {};
	u64 number_index = 0;
	
	while (buf_index < buf_len) {
		u8 token_type = char_get_token_type(buf[buf_index]);

		if (number_index > 21) {
			printf("[lexer error] NUMBER IS TOO LARGE!\n");
			free(buf);
			token_list_clear(tokens);
			return NULL;
		}

		if (token_type != last_token_type && last_token_type == T_NUMBER) {
			number[number_index] = '\0';
			number_index = 0;
			current->type = T_NUMBER;
			current->number = strtod(number, NULL);
			current = token_list_new(current);
		}

		if (token_type == T_NUMBER) {
			number[number_index] = buf[buf_index];
			number_index++;
		} else if (token_is_operator(token_type)) {
			if (token_is_operator(last_token_type) && last_token_type == token_type) {
				printf("[lexer error] OPERATORS SHOULD BE PLACED NEXT TO NUMBERS.");
				free(buf);
				token_list_clear(tokens);
				return NULL;
			}
			current->type = token_type;
			current = token_list_new(current);
		} else if (token_type == T_OPEN_PAREN || token_type == T_CLOSE_PAREN) {
			current->type = token_type;
			current = token_list_new(current);
		}

		buf_index += 1;
		last_token_type = token_type;
	}

	free(buf);
	return tokens;
}

static abstract_syntax_tree* parse_expression(token_list **iterator, int priority);

/*
 * Identifica os numeros e calcula as expressoes dentro de parenteses.
 */
static abstract_syntax_tree* parse_primary(token_list **iterator) {
	token_list* current = *iterator;
	
	if (current == NULL) {
		printf("[parser error] EXPECTED END OF EXPRESSION.");
		return NULL;
	}

	if (current->type == T_OPEN_PAREN) {
		*iterator = current->next;
		abstract_syntax_tree* inner = parse_expression(iterator, LOW_PRIORITY);
		if (inner == NULL) return NULL;

		if (*iterator == NULL || (*iterator)->type != T_CLOSE_PAREN) {
			printf("[parser error] EXPECTED ')'.\n");
			ast_clear(inner);
			return NULL;
		}
		*iterator = (*iterator)->next;
		return inner;
	}

	if (current->type == T_NUMBER) {
		abstract_syntax_tree *new_node = ast_new();
		new_node->type = T_NUMBER;
		new_node->number = current->number;
		*iterator = current->next;
		return new_node;
	}

	printf("[parser error] EXPECTED A NUMBER OR '('.\n");
	return NULL;
}

/*
 * Identifica e calcula os operadores e chama a função para identificar os numeros
 * e os parenteses.
 */
static abstract_syntax_tree* parse_expression(token_list **iterator, int priority) {
	abstract_syntax_tree* left = parse_primary(iterator);
	
	if (left == NULL) {
		return NULL;
	}

	while (*iterator != NULL
	       && token_is_operator((*iterator)->type)
	       && token_get_priority((*iterator)->type) >= priority
	) {
		u8 op = (*iterator)->type;
		int op_priority = token_get_priority(op);

		*iterator = (*iterator)->next;

		abstract_syntax_tree* right = parse_expression(iterator, op_priority + 1);
		if (right == NULL) {
			ast_clear(left);
			return NULL;
		}

		abstract_syntax_tree* node = ast_new();
		node->left = left;
		node->right = right;
		node->type = op;
		left = node;
	}

	return left;
}

/*
 * Chama o parse_expression para com a priority mais baixa para que
 * todos os token sejam ordenados na arvore de sintax abstrata.
 */
abstract_syntax_tree* parser(token_list* tokens) {
	token_list* iterator = tokens;
	abstract_syntax_tree* ast = parse_expression(&iterator, LOW_PRIORITY);
	if (ast != NULL && iterator != NULL && iterator->type != T_END) {
		printf("[parser error] UNEXPECTED TOKEN.\n");
		ast_clear(ast);
		return NULL;
	}
	return ast;
}

/*
 * Realiza todas as operações da arvore de sintax abstrata.
 */
f64 evaluate(abstract_syntax_tree* ast) {
	f64 number_first = 0;
	f64 number_second = 0;

	if (ast->left != NULL) {
		number_first = evaluate(ast->left);
	}

	if (ast->right != NULL) {
		number_second = evaluate(ast->right);
	}

	switch (ast->type) {
		case T_OPERATOR_ADD:
			return number_first + number_second;
		case T_OPERATOR_SUB:
			return number_first - number_second;
		case T_OPERATOR_DIV:
			return number_first == 0 || number_second == 0 ? 0 : number_first / number_second;
		case T_OPERATOR_MUL:
			return number_first * number_second;
		case T_NUMBER:
			return ast->number;
		case T_OPERATOR_POWER:
			return pwr(number_first, number_second);
	}

	return 0;
}

f64 eval_str(const char *expression) {
	token_list* tokens = lexer(expression);
	if (tokens == NULL) {
		return 0;
	}

	abstract_syntax_tree* ast = parser(tokens);
	if (ast == NULL) {
		token_list_clear(tokens);
		return 0;
	}

	f64 result = evaluate(ast);

	ast_clear(ast);
	token_list_clear(tokens);

	return result;
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		u64 size = 0;
		for (u64 i = 1; i < argc; i++) {
			size += str_get_size(argv[i])-1;
		}
	
		char* expression = (char*) malloc(size+1);

		u64 offset = 0;
		for (u64 i = 1; i < argc; i++) {
			size = str_get_size(argv[i])-1;
			memcpy(expression+offset, argv[i], size);
			offset += size;
		}
		
		f64 result = eval_str(expression);
		printf("Expression: '%s'\n", expression);
		printf("Result: '%f'\n", result);
		
		free(expression);
	} else {
		printf("Input not detected, please use as follow: ./matho <expression>\n");
		printf("\tExample: ./matho 1 + 2 + 3 + 4 or ./matho \"1 + 2 + 3\"\n");
		printf("\tSupported operations:\n");
		printf("\t\tAddition: '+'\n");
		printf("\t\tSubtraction: '-'\n");
		printf("\t\tDivision: '/'\n");
		printf("\t\tMultiplication: '*'\n");
		printf("\t\tPower: '^'\n");
		printf("\t\tAssosiation: '()'\n\n");

		printf("Loading sample expression for showcase\n");
		const char* sample_expression = "(1^99 + (2 - 58) / (5*5*2 / 3^3)) + (5 * 2 - 99 + 41)^2";
		
		f64 result = eval_str(sample_expression);
		printf("\tExpression: '%s'\n", sample_expression);
		printf("\tResult: '%f'\n", result);
	}
}

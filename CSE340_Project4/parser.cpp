#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include "compiler.h"
#include "ir_debug.h"
#include "lexer.h"


using namespace std;

LexicalAnalyzer lexer;

void parse_program();
void parse_var_decl();
struct StatementNode* parse_body();
void parse_id_list();
struct StatementNode * parse_stmt_list();
void parse_stmt();
bool check_for_next_statement();
void parse_assign_stmt();
struct StatementNode* parse_body();
void parse_assign_stmt();
void parse_primary();
void parse_expr();
bool is_operator(int oper);
void parse_id();
void parse_num();
void parse_print_stmt();
void parse_while_stmt();
void parse_condition();
void parse_if_statement();
void parse_switch_stmt();
void parse_case_list();
void parse_default_case();
void parse_for_stmt();
void parse_case_list();
void parse_case();


void syntax_error()
{
	cout << "Syntax Error\n";
	exit(1);
}



const string ERROR_MESSAGE = "Syntax Error";

bool check_for_next_statement() 
{
	Token t = lexer.GetToken();

	switch (t.token_type)
	{
	case ID:
	case PRINT:
	case WHILE:
	case IF:
	case SWITCH:
	case FOR:
		lexer.UngetToken(t);
		return true;
	default:
		lexer.UngetToken(t);
		return false;
	}	
}


bool is_operator(int oper) 
{
	switch (oper)
	{
	case PLUS:
	case MINUS:
	case MULT:
	case DIV:
		return true;
	default:
		return false;
	}
}


void parse_expr() 
{
	Token t1, t2, t3, t4;
	//parse_primary();
	t1 = lexer.GetToken(); // holds first operand
	t2 = lexer.GetToken(); // holds operator
	if (!is_operator(t2.token_type)) { syntax_error(); }

	// parse_primary();
	t3 = lexer.GetToken(); // holds second operand
}


void parse_default_case()
{
	Token t = lexer.GetToken();

	if (t.token_type != DEFAULT) { syntax_error(); }

	t = lexer.GetToken();

	if (t.token_type != COLON) { syntax_error(); }
	parse_body();
	
}

void parse_switch_stmt() 
{
	Token t1, t2, t3;

	t1 = lexer.GetToken();
	if (t1.token_type != SWITCH) { syntax_error(); }

	//t2 = parse_id(); 
	t2 = lexer.GetToken(); // holds the id
	t3 = lexer.GetToken();

	if (t3.token_type != LBRACE) { syntax_error(); }
	parse_case_list();

	t3 = lexer.GetToken();
	if (t3.token_type == RBRACE) { return; }
	else if (t3.token_type == DEFAULT)
	{
		lexer.UngetToken(t3);
		parse_default_case();
		t3 = lexer.GetToken();
		if (t3.token_type != RBRACE) { syntax_error(); }
	}
	else { syntax_error(); }

}

void parse_for_stmt() 
{
	Token t = lexer.GetToken();
	if (t.token_type != FOR) { syntax_error(); }

	t = lexer.GetToken();
	if (t.token_type != LPAREN) { syntax_error(); }

	parse_assign_stmt();
	parse_condition();
	
	t = lexer.GetToken();
	if (t.token_type != SEMICOLON) { syntax_error(); }

	parse_assign_stmt();

	t = lexer.GetToken();
	if (t.token_type != RPAREN) { syntax_error(); }

	parse_body();
}

void parse_case_list()
{
	parse_case();

	Token t = lexer.GetToken();
	if (t.token_type == CASE) 
	{
		lexer.UngetToken(t);
		parse_case_list();
	}
	else { lexer.UngetToken(t); }
}

void parse_case() 
{
	Token t;

	t = lexer.GetToken();
	if (t.token_type != CASE) { syntax_error(); }
	
	t = lexer.GetToken(); // create node here
	if (t.token_type != NUM) { syntax_error(); }

	t = lexer.GetToken();
	if (t.token_type != COLON) { syntax_error(); }

	parse_body();
}	


/*
void parse_id() 
{
	
}


void parse_num() 
{

}


void parse_primary()
{
	Token t = lexer.GetToken();

	switch (t.token_type)
	{
	case ID:
		parse_id();
		break;
	case NUM: 
		parse_num();
		break;
	default:
		syntax_error();
		break;
	}
}

*/


void parse_assign_stmt() 
{
	Token t1, t2, t3, t4, t5;
	
	// parse the id here
	t1 = lexer.GetToken(); // lhs id
	t2 = lexer.GetToken(); // should be equal sign
	
	if (t2.token_type != EQUAL) { syntax_error(); }

	t3 = lexer.GetToken(); // first rhs operand
	if (t3.token_type == ID || t3.token_type == NUM)
	{
		t4 = lexer.GetToken();
		if (is_operator(t4.token_type))
		{
			lexer.UngetToken(t4);
			lexer.UngetToken(t3);
			parse_expr();
		}
		else if (t4.token_type == SEMICOLON) 
		{
			// create node
			return;
		}
		{
			//have the whole statement here t = t2      t1 is equal sign, t3 is semicolon
			//lexer.UngetToken(t3);
			//lexer.UngetToken(t2);
			//parse_primary();


		
		}
	}
	t5 = lexer.GetToken();
	if (t5.token_type != SEMICOLON) { syntax_error(); }


}

void parse_id_list() 
{
	Token t = lexer.GetToken();
	if (t.token_type == ID)
	{
		//lexer.UngetToken(t);  // here is where the id can be used
		//parse_id();
		t = lexer.GetToken();
		if (t.token_type == COMMA)
		{
			parse_id_list();
		}
		else
		{
			lexer.UngetToken(t);

		}
	}
	else { syntax_error(); }

}


void parse_print_stmt() 
{
	Token t1, t2, t3;

	t1 = lexer.GetToken(); // holds print keyword
	if (t1.token_type != PRINT) { syntax_error(); }

	t2 = lexer.GetToken();  // hold the name of the var (id) to be printed 
	if (t2.token_type != ID) { syntax_error(); }

	t3 = lexer.GetToken();
	if (t3.token_type != SEMICOLON) { syntax_error(); }

}

void parse_while_stmt() 
{
	Token t1 = lexer.GetToken();
	if (t1.token_type != WHILE) { syntax_error(); }

	parse_condition();
	parse_body();

}


void parse_if_statement() 
{
	Token t1 = lexer.GetToken();

	if (t1.token_type != IF) { syntax_error(); }
	parse_condition();
	parse_body();

}

void parse_condition() 
{
	Token t1, t2, t3;
	t1 = lexer.GetToken(); // holds the id, primary lhs of cond
	t2 = lexer.GetToken(); // holds the relation operator
	t3 = lexer.GetToken(); // holds the id, primary rhs of cond

}



void parse_stmt()
{
	Token t1, t2;
	t1 = lexer.GetToken();
	
	switch(t1.token_type)
	{
	case PRINT:
		lexer.UngetToken(t1);
		parse_print_stmt();
		break;
	case WHILE:
		lexer.UngetToken(t1);
		parse_while_stmt();
		break;
	case IF:
		lexer.UngetToken(t1);
		parse_if_statement();
		break;
	case FOR:
		lexer.UngetToken(t1);
		parse_for_stmt();
		break;
	case ID:
		t2 = lexer.GetToken();
		if (t2.token_type == EQUAL)
		{
			lexer.UngetToken(t2);
			lexer.UngetToken(t1);
			parse_assign_stmt();
		}
		break;
	default:
		syntax_error();
	}
	
}

struct StatementNode* parse_stmt_list() 
{
	parse_stmt();
	while (check_for_next_statement()) 
	{	
		parse_stmt();
	}


	return NULL;
}

void parse_var_decl() 
{
	parse_id_list();
	Token t = lexer.GetToken();
	if (t.token_type != SEMICOLON) { syntax_error(); }
}

void parse_program() 
{
	parse_var_decl();
	parse_body();
}

struct StatementNode* parse_body()
{
	struct StatementNode* stl = nullptr;
	Token t = lexer.GetToken(); // get the { 
	if (t.token_type == LBRACE)
	{
		stl = parse_stmt_list();  // parse the statement list
	}
	t = lexer.GetToken(); // get the RBRACE
	if (t.token_type == RBRACE)
		return stl;
	else { syntax_error(); return NULL; }
}


struct StatementNode* parse_generate_intermediate_representation()
{
	parse_program();
	//parse_id_list();  // first line of declaration 
	return parse_body();  // parse the body

}

#include <string.h>

#include "tokenizer.h"

// HELPER FUNCTIONS
static bool isTokenEnder(char c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v' || c == '\f' 
			|| c == '(' || c == ')' || c == ';' || c == EOF;
}

static bool isSymbolCharacter(char c) {
	int charInt = c;
	return c == '+' || c == '-' || c == '.' || c == '*' || c == '/' || c == '<' || c == '=' 
			|| c == '>' || c == '!' || c == '?' || c == ':' || c == '$' || c == '%' || c == '_' 
			|| c == '&' || c == '~' || c == '^' || (charInt >= '0' && charInt <= '9') 
			|| (charInt >= 'A' && charInt <= 'Z') || (charInt >= 'a' && charInt <= 'z');
}

static bool isDigit(char c) {
	return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' 
			|| c == '7' || c == '8' || c == '9';
}

static char *charToStr(char c) {
	char *temp = talloc(sizeof(char) * 2);
	temp[0] = c;
	temp[1] = '\0';
	return temp;
}

static char *catStrChar(char *string, char c) {
	char *temp = charToStr(c);
	char *newStr = talloc(strlen(string) + 2*sizeof(char));
	newStr = strcpy(newStr, string);
	newStr = strcat(newStr, temp);
	return newStr;
}

// PARSE NUMBERS
static Value *parseNumber(FILE *stream, char nextChar) {
	Value *val = makeNull();
	double tempNum = nextChar - '0';
    val->type = INT_TYPE;

    nextChar = fgetc(stream);
    while (isDigit(nextChar)) {
        tempNum *= 10;
        tempNum += (nextChar - '0');
        nextChar = fgetc(stream);
    }

    if (nextChar == '.') {
        val->type = DOUBLE_TYPE;
        nextChar = fgetc(stream);

    	double dec = 1;
        while (isDigit(nextChar)) {
            dec /= 10;
            tempNum += (nextChar - '0')*dec;
            nextChar = fgetc(stream);
        }

        val->d = tempNum;
    } else{
        val->i = tempNum;
    } if (!isTokenEnder(nextChar)) {
        printf("Error, not a number\n");
        texit(1);
    }
    ungetc(nextChar, stream);

	return val;
}

// PARSE BOOLEANS
static Value *parseBoolean(FILE *stream) {
	Value *val = makeNull();
	val->type = BOOL_TYPE;

	char charRead = fgetc(stream);
	if (charRead == 't') {
		val->s = "#t";
	} else if (charRead == 'f') {
		val->s = "#f";
	} else {
		printf("Error, #%c not a bool type", charRead);
		texit(1);
	}
		
	charRead = fgetc(stream);
	if (!isTokenEnder(charRead)) {
		printf("Error, %s%c not a bool type", val->s, charRead);
		texit(1);
	}
	ungetc(charRead, stream);

	return val;
}

// PARSE SYMBOLS
static Value *parseSymbol(FILE *stream, char charRead) {
	Value *val = makeNull();
	val->type = SYMBOL_TYPE;
	char *charString = charToStr(charRead);
	charRead = fgetc(stream);
	while (isSymbolCharacter(charRead)) {
		charString = catStrChar(charString, charRead);
		charRead = fgetc(stream);
	}

	if (!isTokenEnder(charRead)) {
		printf("Error, %c is not a valid symbol character\n", charRead);
		texit(1);
    }
	ungetc(charRead, stream);
	val->s = charString;

	return val;
}

// PARSE STRINGS
static Value *parseString(FILE *stream, char charRead) {
	Value *val = makeNull();
	val->type = STR_TYPE;
	char *charString = charToStr(charRead);
	charRead = fgetc(stream);
	while(charRead != '\"') {
		if (charRead == EOF) {
			printf("Error, unterminated string");
			texit(1);
		} else if (charRead == '\\') {
			charRead = fgetc(stream);
			switch (charRead) {
				case 'n':
					charRead = '\n';
					break;
				case 't':
					charRead = '\t';
					break;
				case '\'':
				case '"':
				case '\\':
					break;
				default: // only so many valid escape sequences
					printf("Invalid escape sequence: \\%c", charRead);
					texit(1);
			}
		}
		charString = catStrChar(charString, charRead);
		charRead = fgetc(stream);
	}

	val->s = ++charString; // eliminate starting "
	return val;
}

/*
 * MAIN TOKENIZE FUNCTION 
 * Reads in code and creates a linked list of tokens based on their type.
 */
Value *tokenize(FILE *stream) {	
	char charRead;
	Value *list = makeNull();

	charRead = fgetc(stream);
	while (charRead != EOF) {
		if (charRead == '(') { // Open Type
			Value *val = makeNull();
			val->type = OPEN_TYPE;
			val->s = "(";
			list = cons(val, list);	
		} else if (charRead == ')') { // Close Type
			Value *val = makeNull();
			val->type = CLOSE_TYPE;
			val->s = ")";
			list = cons(val, list);	
		} else if (charRead == '#') { // Boolean Type
			Value *val = parseBoolean(stream);		
			list = cons(val, list);	
		} else if (charRead == '+' || charRead == '-') { // +/- Special Case
			Value *val;
			char sign = charRead;

			charRead = fgetc(stream);
			if (isTokenEnder(charRead)) {
				val = makeNull();
				val->type = SYMBOL_TYPE;
				val->s = charToStr(sign);
				ungetc(charRead, stream);
			} else {
				if (charRead == '.') {
					charRead = fgetc(stream);
					if (isTokenEnder(charRead)) {
                		printf("Error, %c. is not a valid token", sign);
                		texit(1);
            		}
        	    	ungetc(charRead, stream);
    	        	ungetc('.', stream);
	            	charRead = '0';
				}
				val = parseNumber(stream, charRead);
				if (sign == '-' && val->type == INT_TYPE) {
					val->i *= -1;
				} else if (sign == '-') {
					val->d *= -1;
				}
			}
			
			list = cons(val, list);
		} else if (charRead == '.') { // . Special Case
			charRead = fgetc(stream);
			if (isTokenEnder(charRead)) {
				printf("Error, . is not a valid token");
				texit(1);
			}
			ungetc(charRead, stream);
			ungetc('.', stream);
			Value *val = parseNumber(stream, '0');
			list = cons(val, list);
		} else if (isDigit(charRead)) { // Number Types
			Value *val = parseNumber(stream, charRead);
			list = cons(val, list);
		} else if (isSymbolCharacter(charRead) || charRead == '\'') { // Symbol Type
			Value *val = parseSymbol(stream, charRead);
			list = cons(val, list);
		} else if (charRead == '\"') { // String Type
			Value *val = parseString(stream, charRead);
			list = cons(val, list);
		} else if (charRead == ';') { // Comments
			charRead = fgetc(stream);
			while (charRead != '\n' && charRead != EOF) {
				charRead = fgetc(stream);
			}
			ungetc(charRead, stream);
		} else if (isTokenEnder(charRead)) {
			// No action needed, whitespace
		} else {
			printf("Error, %c is not a valid character to start a token\n", charRead);
			texit(1);
		}
		charRead = fgetc(stream);
	}

	return reverse(list);
}

/*
 * Takes in a linked list of tokens as input and displays them with one token
 * per line.
 */
void displayTokens(Value *list) {
	Value *token;
	while (!isNull(list)) {
		token = car(list);
		switch (token->type) {
			case INT_TYPE:
				printf("%d:integer\n", token->i);
				break;
			case DOUBLE_TYPE:
				printf("%lf:float\n", token->d);
				break;
			case STR_TYPE:
				printf("%s:string\n", token->s);
				break;
            case OPEN_TYPE:
                printf("%s:open\n", token->s);
                break;
            case CLOSE_TYPE:
                printf("%s:close\n", token->s);
                break;
            case BOOL_TYPE:
                printf("%s:boolean\n", token->s);
                break;
            case SYMBOL_TYPE:
                printf("%s:symbol\n", token->s);
                break;
            default:
				printf("Error: invalid token type\n");
				return;
                break;
		}
		list = cdr(list);
	}
}


#pragma once
#include <iostream>
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstddef>
#include <malloc.h>
#include <vector>
#include <ostream>

#define ALL_NUMERIC_CHARS  '0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9'
#define ALL_LOWER_LETTERS  'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':case'v':case'w':case'x':case'y':case'z'
#define ALL_UPPER_LETTERS  'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z'



struct file_content
{

    const char *const file_name;
    const char *const  f_content;
    const size_t f_size;
    file_content (
            const char * const file_name,
            const char * const f_content,
            const size_t f_size):
        file_name(file_name),
        f_content(f_content),
        f_size(f_size)
    {
    }

};

file_content get_file_content(const char * file_name)
{

    int fd=open(file_name,O_RDONLY);

    if(fd<0)throw "File does not exists";

    struct stat st;
    fstat(fd,&st);

    const size_t file_size=st.st_size;
    char * content= static_cast<char *>(malloc(file_size));
    read(fd,content,file_size);

    close(fd);

    return {file_name,content,file_size};
}
namespace tokenizer {
    enum class TokenType 
    {
        STRING_LITERAL,
        INT_LITERAL,
        DECIMAL_LITERAL,
        OPEN_SCOPE,
        CLOSE_SCOPE,
        OPEN_PARENTHESIS,
        CLOSE_PARENTHESIS,
        OPEN_BRACKETS,
        CLOSE_BRACKETS,
        GRATHER_THAN,
        LESSER_THAN,
        IDENTIFIER,
        WHITE_SPACE,
        COMMA,
        DOT,
        EQUAL,
        SEMICOLUN,
        PLUS,
        MINUN,
        ASTERISK,
        SLASH,
        BACKSLAH,
        COMMENT,
        FUNCTION,
        FUNCITON_PARAMETER
    };
    std::ostream& operator<< (std::ostream& o, const TokenType t)
    {
        switch (t) {
            case TokenType::STRING_LITERAL:o<<" STRING_LITERAL ";break;
            case TokenType::INT_LITERAL:o<<" INT_LITERAL ";break;
            case TokenType::DECIMAL_LITERAL:o<<" DECIMAL_LITERAL ";break;
            case TokenType::OPEN_SCOPE:o<<" OPEN_SCOPE ";break;
            case TokenType::CLOSE_SCOPE:o<<" CLOSE_SCOPE ";break;
            case TokenType::OPEN_PARENTHESIS:o<<" OPEN_PARENTHESIS ";break;
            case TokenType::CLOSE_PARENTHESIS:o<<" CLOSE_PARENTHESIS ";break;
            case TokenType::OPEN_BRACKETS:o<<" OPEN_BRACKETS ";break;
            case TokenType::CLOSE_BRACKETS:o<<" CLOSE_BRACKETS ";break;
            case TokenType::IDENTIFIER:o<<" IDENTIFIER ";break;
            case TokenType::WHITE_SPACE:o<<" WHITE_SPACE ";break;
            case TokenType::COMMA:o<<" COMMA ";break;
            case TokenType::DOT:o<<" DOT ";break;
            case TokenType::EQUAL:o<<" EQUAL ";break;
            case TokenType::SEMICOLUN:o<<" SEMICOLUN ";break;
            case TokenType::PLUS:o<<" PLUS ";break;
            case TokenType::MINUN:o<<" MINUN ";break;
            case TokenType::ASTERISK:o<<" ASTERISK ";break;
            case TokenType::SLASH:o<<" SLASH ";break;
            case TokenType::BACKSLAH:o<<" BACKSLAH ";break;
            case TokenType::BACKSLAH:o<<" BACKSLAH ";break;
            case TokenType::BACKSLAH:o<<" BACKSLAH ";break;
            case TokenType::BACKSLAH:o<<" BACKSLAH ";break;
            default:o <<"UNKNOWN TOKEN TYPE";
        }
        return o;
    }
    struct Token
    {
        const TokenType token_type;
        const char * start;
        const size_t size;
        const int line_number;
        const int column_number;
        Token(  const TokenType token_type,
                const char * start,
                const size_t size,
                const int line_number,
                const int column_number) noexcept:
            token_type(token_type),
            start(start),
            size(size),
            line_number(line_number),
            column_number(column_number) 
        {
        }

    };
    
    std::vector<Token> tokenize(const file_content& f_c)
    {

        size_t line_number = 0;
        size_t char_pos = 0;
        
        std::vector<Token> tokens;
        tokens.reserve(1000);
        const char * file_end=f_c.f_content+f_c.f_size;


        const char * iterator=f_c.f_content;//move character by character

        TokenType actual_type= TokenType::WHITE_SPACE;//define the type of the token being parse
        const char * token_start;//defines wehere the token start 
        bool token_is_open = false;


        auto open_token=[&](TokenType t)
        {
            token_start = iterator;
            actual_type = t;
            token_is_open = true;
        };

        auto close_token=[&](const int ignore_start=0, const int ignore_end=0)
        {
            const char * start=token_start+ignore_start;
            const char * end= iterator-ignore_end;

            tokens.emplace_back(actual_type,start,end-start +1,line_number,char_pos);
            actual_type=TokenType::WHITE_SPACE;
            token_is_open=false;
        };

        auto close_token_ignore_last=[&](){close_token(0,1);};

        auto token_is=[&](const TokenType t){return actual_type==t;};
        auto update_token_type=[&](const TokenType t){actual_type=t;};

        auto add_monochar_token=[&](const  TokenType t)
        {
            if(actual_type==TokenType::STRING_LITERAL) return;
            if(token_is_open)close_token(0,1);
            open_token(t);
            close_token(0,0);
        };


        for (;iterator!=file_end;iterator++)
        {
            const char character= *iterator;
            
            
            switch (character) {
                case'\n':
                    line_number++;
                    char_pos=0;
                    break;
                case ALL_NUMERIC_CHARS:
                    if(!token_is_open)open_token(TokenType::INT_LITERAL);
                    break;
                case '"':
                    if(!token_is_open)open_token(TokenType::STRING_LITERAL);
                    else if(token_is(TokenType::STRING_LITERAL))close_token();
                    break;
                case ALL_UPPER_LETTERS :case ALL_LOWER_LETTERS:case '_':
                    if(!token_is_open)open_token(TokenType::IDENTIFIER);
                    break;
                    
                //monocharaceter tokens, if actual token is not string, then acutal token is close
                //if a monoCharacterOperator closes an open token it must do it up to the previus character, so it doesnt include itself
                case ' '://close any token, but ignores spaces
                    if(token_is(TokenType::WHITE_SPACE))break;;
                    if(actual_type != TokenType::STRING_LITERAL) close_token_ignore_last();
                    break;

                case ',':add_monochar_token(TokenType::COMMA);break;
                case '.' :
                    if (actual_type==TokenType::INT_LITERAL){
                        update_token_type(TokenType::DECIMAL_LITERAL);
                        break;
                    }
                    add_monochar_token(TokenType::DOT);
                    break;
                case '(':add_monochar_token(TokenType::OPEN_PARENTHESIS);break;
                case ')':add_monochar_token(TokenType::CLOSE_PARENTHESIS);break;
                case '[':add_monochar_token(TokenType::OPEN_BRACKETS);break;
                case ']':add_monochar_token(TokenType::CLOSE_BRACKETS);break;
                case '{':add_monochar_token(TokenType::OPEN_SCOPE);break;
                case '}':add_monochar_token(TokenType::CLOSE_SCOPE);break;
                case '=':add_monochar_token(TokenType::EQUAL);break;
                case ';':add_monochar_token(TokenType::SEMICOLUN);break;
                case '+':add_monochar_token(TokenType::PLUS);break;
                case '-':add_monochar_token(TokenType::MINUN);break;
                case '/':add_monochar_token(TokenType::SLASH);break;
                case '*':add_monochar_token(TokenType::ASTERISK);break;
                case '\\':add_monochar_token(TokenType::BACKSLAH);break;
                


            }
            char_pos++;//advance the column nnumber;
        }

       return tokens;

    }
}

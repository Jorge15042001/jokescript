
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
        IDENTIFIER,
        WHITE_SPACE,
        COMMA,
        DOT,
        EQUAL,
        SEMICOLUN
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
        }
        return o;
    }
    struct Token
    {
        const TokenType token_type;
        const char * start;
        size_t size;
        Token(TokenType token_type,const char * start, const size_t size):
            token_type(token_type),
            start(start),
            size(size)
        {
        }
    };
    
    std::vector<Token> tokenize(const file_content& f_c)
    {
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
        auto close_token=[&](const int ignore_start=0, const int ignore_end=1)
        {
            const char * start=token_start+ignore_start;
            const char * end= iterator-ignore_end;


            if (actual_type==TokenType::STRING_LITERAL)start++;

            tokens.emplace_back(actual_type,start,end-start );
            //reset values 
            actual_type=TokenType::WHITE_SPACE;
            token_is_open=false;

        };
        auto update_token_type=[&](const TokenType t)
        {
            actual_type=t;
        };
        
        auto add_monochar_token=[&](const  TokenType t)
        {
            open_token(t);
            close_token(0,0);
        };


        for (;iterator!=file_end;iterator++)
        {
            const char character= *iterator;
            switch (character) {
                case '0' ... '9':
                    if(!token_is_open)open_token(TokenType::INT_LITERAL);
                    break;
                case '"':
                    if(!token_is_open)open_token(TokenType::STRING_LITERAL);
                    else if(actual_type==TokenType::STRING_LITERAL)close_token();
                    break;
                case ' '://close any token, but ignores spaces
                    if(actual_type == TokenType::WHITE_SPACE)break;;
                    if(actual_type != TokenType::STRING_LITERAL) close_token();
                    break;

                case 'a' ... 'z':
                case 'A' ... 'Z':
                case '_':
                    if(!token_is_open)open_token(TokenType::IDENTIFIER);
                    break;
                    
                //monocharaceter tokens, if actual token is not string, then acutal token is close
                case ',':
                    if (actual_type == TokenType::STRING_LITERAL) break;
                    if (token_is_open)close_token();
                    add_monochar_token(TokenType::COMMA);
                    break;
                case '.' :
                    if (actual_type==TokenType::INT_LITERAL){
                        update_token_type(TokenType::DECIMAL_LITERAL);
                        break;
                    }
                    if(token_is_open)close_token();
                    add_monochar_token(TokenType::DOT);
                    break;
                case '(':
                    if(actual_type == TokenType::STRING_LITERAL)break;
                    if (token_is_open)close_token();// 
                    add_monochar_token(TokenType::OPEN_PARENTHESIS);
                    break;
                case ')':
                    if(actual_type == TokenType::STRING_LITERAL)break;
                    if (token_is_open)close_token();// 
                    add_monochar_token(TokenType::CLOSE_PARENTHESIS);
                    break;
                case '[':
                    if(actual_type == TokenType::STRING_LITERAL)break;
                    if (token_is_open)close_token();// 
                    add_monochar_token(TokenType::OPEN_BRACKETS);
                    break;
                case ']':
                    if(actual_type == TokenType::STRING_LITERAL)break;
                    if (token_is_open)close_token();// 
                    add_monochar_token(TokenType::CLOSE_BRACKETS);
                    break;
                case '{':
                    if(actual_type == TokenType::STRING_LITERAL)break;
                    if (token_is_open)close_token();// 
                    add_monochar_token(TokenType::OPEN_SCOPE);
                    break;
                case '}':
                    if(actual_type == TokenType::STRING_LITERAL)break;
                    if (token_is_open)close_token();// 
                    add_monochar_token(TokenType::CLOSE_SCOPE);
                    break;
                case '=':
                    if(actual_type == TokenType::STRING_LITERAL)break;
                    if (token_is_open)close_token();// 
                    add_monochar_token(TokenType::EQUAL);
                    break;
                case ';':
                    if(actual_type == TokenType::STRING_LITERAL)break;
                    if (token_is_open)close_token();// 
                    add_monochar_token(TokenType::SEMICOLUN);
                    break;


            }
            //std::cout<<"Tokenizing: "<<character<<" actual_type:"<<actual_type<<" open:"<<token_is_open<<'\n';
        }

       return tokens;

    }
}

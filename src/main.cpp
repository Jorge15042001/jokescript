#include <iostream>
#include "tokenizer.h"
#include <vector>
int main(const int argc, const char ** argv)
{
    const char * filename= argv[1];
    const file_content fc=get_file_content(filename);
    std::cout<<"File Content:\n"<<fc.f_content<<'\n';
    std::vector<tokenizer::Token> tokens=tokenizer::tokenize(fc);
    for (const auto & t:tokens)
    {
        std::cout<<"\nToken: "<<t.token_type;
        std::cout<<"\tsize:"<<t.size<<"\n\tValues:";
        for (int i =0; i<=t.size;i++){
            std::cout<<*(t.start+i);
        }
        std::cout<<'\n';
    }
    
    
    


}

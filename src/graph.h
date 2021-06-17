#ifndef GRAPH_INCLUDE
#define GRAPH_INCLUDE

#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <utility>

template <typename T> class vertex_t;
template <typename T> class edge_t;
template <typename T> class graph_t;

template <typename T>
class vertex_t
{
    const T m_content;
    std::list<edge_t<T>> connections;
    constexpr edge_t<T>& add_connection(const int weight, const vertex_t<T>& connect_to) noexcept
    {
        connections.emplace_back(connect_to,weight);
        return connections.back();
    }
public:
    template<typename ...Args>
    vertex_t(Args&&... args):
        m_content(std::forward<Args>(args)...)
    {
    }
    vertex_t(const T& content):
        m_content(content)
    {
    }

    constexpr const T& get_content()const noexcept
    {
        return m_content;
    }
    constexpr bool already_conected(const T& connect_to) const  noexcept
    {
        for (const auto & edge:connections){
            if(edge.target==connect_to)return true;
        }
        return false;
    }
    constexpr const edge_t<T>& get_edge(const T& to_find)const 
    {
        for (const auto& edge:connections)
        {
            if(edge.target.get_content()==to_find)return edge;
        }
        //not found
        throw "not connected";
    }

    constexpr const edge_t<T>& get_edge(const vertex_t<T>& to_find)const  
    {
        for (const auto& edge:connections)
        {
            if(edge.target.get_content()==to_find.get_content())return edge;
        }
        //not found
        throw "not connected";
    }
    const edge_t<T>& connect( const vertex_t<T>& connect_to, const int weight=1){ 
        try 
        {
            return get_edge(connect_to);
        }
        catch(const char * error_message)
        {
            return add_connection(weight,connect_to);
        }
    }
    constexpr friend std::ostream& operator <<( std::ostream&  o, const vertex_t& value) noexcept
    {
        o<<"vertex ->"<<value.m_content;
        o<<"\tedges";
        for (const auto & edge:value.connections)
        {
            o<<"\t\t"<<edge;
        }
        o<<"\n";
        return o;
    }



};

/*
template<typename U>
std::ostream& operator <<( std::ostream& o, const vertex_t<U>& value){
    o<<"vertex ->"<<value.m_content;
    o<<"\tedges";
    for (const auto & edge:value.connections)
    {
        o<<"\t\t"<<edge;
    }
    return o;
    
}
*/
template <typename T> 
class edge_t
{
public:
    vertex_t<T>  target;
    const int weight=1;
    edge_t(const vertex_t<T>& targert):
        target(target)
    {
    }
    edge_t(const vertex_t<T>& target, const int weight):
        target(target),
        weight(weight)
    {
    }
    friend std::ostream& operator<<( std::ostream& o, const edge_t value)
    {
        o<<"edge{weight: "<<value.weight<<" target:"<<value.target.get_content()<<"}\t";
        return o;
    }
};

/*
template<typename U>
std::ostream& operator<<( std::ostream& o, const edge_t<U> value)
{
    o<<"edge{weight: "<<value.weight<<" target:"<<value.target.get_content()<<"}\n";
    return o;
}
*/

template <typename T> 
class graph_t
{
    std::list<vertex_t<T>> vertexes;
    const bool directed;
    vertex_t<T>& get_vertex(const T& to_find)
    {
        for (  auto & v:vertexes)if(v.get_content()==to_find)return v;
        throw ("not a vertex");
    }
public:
    graph_t(const bool directed):
        directed(directed)
    {
    }

    template<typename ...Args>
    vertex_t<T>& emplace_vertex(Args&&... args)
    {
        vertexes.emplace_back(std::forward<Args>(args)...);
        //check if have been added already
        const auto second_last=--vertexes.end();
        
        for(auto it=vertexes.begin();it!=second_last;++it){
            if((*it).get_content()==vertexes.back().get_content())//if inserted vertex was already in the list
            {
                auto& response= *it;
                vertexes.pop_back();
                return response;

            }
        }
        //if vertex was not already inserted 
        return vertexes.back();
    }
    vertex_t<T>& push_vertex(const T& vertex_content)
    {
        try 
        {
            return get_vertex(vertex_content);
        }
        catch(const char * error_message)
        {
            std::cout<<error_message<<std::endl;
        }
        vertexes.push_back(vertex_content);
        return vertexes.back();
        
        /*
        vertexes.remove_if([&](const auto& v) {return v.get_content()==vertex_content;});//remove if exist
        vertexes.emplace_back(vertex_content);//add it 
        
        return vertexes.back();
        */
    }
    void print_vertexes() const
    {
        for (const auto & v:vertexes)std::cout<<v.get_content().to_string()<<",";
    }
    const int vertexes_count()const noexcept
    {
        return vertexes.size();
    }
    void add_edge(const T& start, const T& end){
        try
        {
            auto & vertex_start= get_vertex(start);
            auto & vertex_end= get_vertex(end);
            add_edge(vertex_start,vertex_end);
        
        }
        catch(const char * message)
        {
            return;
        }
    }
    void add_edge( vertex_t<T>& vertex_start,  vertex_t<T>& vertex_end){
            vertex_start.connect(vertex_end);
            if(!directed)vertex_end.connect(vertex_start);
    }
    /*
    template<typename ...Args>
    void emplace_and_connect(Args&& ... args, Args&& ...args2)
    {

        auto & vertex_start = emplace_vertex(std::forward<Args>(args)...);
        auto & vertex_end = emplace_vertex(std::forward<Args>(args)...);
        vertex_start.connect(vertex_end,1);
        if(!directed)vertex_end.connect(vertex_start,1);

    }*/


    friend std::ostream& operator<< (std::ostream& o, const graph_t & value)
    {
        o<<"-------------------------------------------\n";
        o<<"graph_t -> size:"<<value.vertexes_count()<<'\n';
        for (auto & v:value.vertexes){
            o<<v;
        }
        o<<"___________________________________________\n";
        return o;

}
};

template<typename U>
std::ostream& operator<< (std::ostream& o, const graph_t<U> & value){
    o<<"graph_t -> size:"<<value.vertexes_count()<<'\n';
    o<<"-------------------------------------------\n";
    for (auto & v:value.vertexes){
        o<<v;
    }
    return o;

}


struct ran
{
    const int a;
    const int b;
    ran(const int a, const int b):
        a(a),b(b)
    {}
    std::string to_string()const 
    {

        std::string response("[");
        response+=std::to_string(a)+",";
        response+=std::to_string(b)+"]";
        return response;

    }
    bool operator==(const ran & other)
    {
        return a==other.a && b==other.b;
    }
    friend std::ostream& operator <<(std::ostream& o, const ran& value);
};
std::ostream& operator <<(std::ostream& o, const ran& value){
    o<<"["<<value.a<<","<<value.b<<"]";
    return o;
}
bool operator==(const ran& lhs, const ran& rhs)
{
    return lhs.a==rhs.a && lhs.b==rhs.b;
}
int main(){
    graph_t<ran> r(true);
    const int a=0;
    const ran x(0,0);
    const ran y(1,0);
    auto & v1=r.push_vertex(y);
    auto & v2=r.push_vertex(x);
    auto & v3=r.push_vertex(x);
    auto & v4=r.push_vertex(y);
    auto & v5=r.push_vertex(x);
    auto & v6=r.push_vertex(y);
    auto & v7=r.emplace_vertex(2,1);
    auto & v8=r.emplace_vertex(0,1);
    auto & v9=r.emplace_vertex(1,1);
    //const auto& v1=r.emplace_vertex(3,2);

    auto & v10=r.emplace_vertex(a,1);
    auto & v11=r.emplace_vertex(a,2);
    
    //r.add_edge(v1.get_content(), v2.get_content());
    //r.add_edge(v2.get_content(),x);
    r.print_vertexes();
    r.add_edge(v1,v2);
    r.add_edge(v3,v9);
    r.add_edge(v3,v9);
    
    std::cout<<r<<"\n";
}
#endif


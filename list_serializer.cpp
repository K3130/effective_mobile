#include "list_serializer.h"

#include <vector>
#include <cstring>
#include <fstream>
#include <unordered_map>

namespace
{
    static const size_t MAX_NODES = 1'000'000;
}

namespace list_serializer
{

ListSerializer::ListSerializer()
{

}

ListSerializer::~ListSerializer()
{

}

ListNode* ListSerializer::create_list_from_file(const std::string& file)
{
    std::ifstream f( file );
    if ( !f.is_open() )
    {
        return nullptr;
    }

    std::vector<ListNode*> nodes;
    std::vector<int> rand_indices;
    std::string line;

    while ( std::getline(f, line) )
    {
        if ( line.empty() )
        {
            continue;
        }
        if ( nodes.size() >= MAX_NODES )
        {
            for (auto node : nodes)
            {
                delete node;
            }
            return nullptr;
        }

        size_t semicolon_pos = line.find(';');
        if ( semicolon_pos == std::string::npos )
        {
            continue;
        }

        ListNode* node = new ListNode();
        node->data = line.substr(0, semicolon_pos);
        nodes.push_back(node);
        rand_indices.push_back(std::stoi(line.substr(semicolon_pos + 1)));
    }
    f.close();

    if ( nodes.empty() )
    {
        return nullptr;
    }

    for (size_t i = 0; i < nodes.size(); i++)
    {
        if ( i > 0 )
        {
            nodes[i]->prev = nodes[i - 1];
        }
        if ( i < nodes.size() - 1 )
        {
            nodes[i]->next = nodes[i + 1];
        }
    }

    for (size_t i = 0; i < nodes.size(); i++)
    {
        if ( rand_indices[i] >= 0 && rand_indices[i] < static_cast<int>(nodes.size()) )
        {
            nodes[i]->rand = nodes[rand_indices[i]];
        }
        else
        {
            nodes[i]->rand = nullptr;
        }
    }

    return nodes[0];
}

void ListSerializer::serialize_to_file(ListNode* head, const std::string& file)
{
    if ( !head )
    {
        std::ofstream f(file, std::ios::binary);
        int count = 0;
        f.write(reinterpret_cast<const char*>(&count), sizeof(count));
        f.close();
        return;
    }

    std::vector<ListNode*> nodes;
    ListNode* current = head;
    while ( current )
    {
        nodes.push_back(current);
        current = current->next;
    }

    std::unordered_map<ListNode*, int> ptr_to_index;
    for ( size_t i = 0; i < nodes.size(); i++ )
    {
        ptr_to_index[nodes[i]] = static_cast<int>(i);
    }

    std::vector<SerializedNode> serialized_nodes(nodes.size());

    for ( size_t i = 0; i < nodes.size(); i++ )
    {
        strncpy(serialized_nodes[i].data, nodes[i]->data.c_str(), 1000);
        serialized_nodes[i].data[1000] = '\0';

        if ( nodes[i]->rand )
        {
            auto it = ptr_to_index.find(nodes[i]->rand);
            if ( it != ptr_to_index.end() )
            {
                serialized_nodes[i].rand_index = it->second;
            }
            else
            {
                serialized_nodes[i].rand_index = -1;
            }
        } else
        {
            serialized_nodes[i].rand_index = -1;
        }

        if (nodes[i]->prev)
        {
            serialized_nodes[i].prev_index = ptr_to_index[nodes[i]->prev];
        }
        else
        {
            serialized_nodes[i].prev_index = -1;
        }

        if ( nodes[i]->next )
        {
            serialized_nodes[i].next_index = ptr_to_index[nodes[i]->next];
        }
        else
        {
            serialized_nodes[i].next_index = -1;
        }
    }

    std::ofstream f( file, std::ios::binary );
    if ( !f.is_open() )
    {
        return;
    }

    int count = static_cast<int>( nodes.size() );
    f.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for ( const auto& node : serialized_nodes )
    {
        f.write(reinterpret_cast<const char*>(&node), sizeof(SerializedNode));
    }
    f.close();
}

bool ListSerializer::deserialize_bin_to_text(const std::string &file_in, const std::string &file_out)
{
    std::ifstream f_in( file_in, std::ios::binary );
    if ( !f_in.is_open() )
    {
        return false;
    }

    int count;
    f_in.read(reinterpret_cast<char*>(&count), sizeof(count));

    if ( count == 0 || f_in.eof() )
    {
        f_in.close();
        std::ofstream f_out(file_out);
        f_out.close();
        return true;
    }

    std::vector<SerializedNode> serialized_nodes(count);
    for (size_t i = 0; i < count; i++)
    {
        f_in.read(reinterpret_cast<char*>(&serialized_nodes[i]), sizeof(SerializedNode));
        if ( f_in.eof() )
        {
            f_in.close();
            return false;
        }
    }
    f_in.close();

    std::ofstream f_out(file_out);
    if ( !f_out.is_open() )
    {
        return false;
    }

    for (size_t i = 0; i < count; i++)
    {
        std::string data(serialized_nodes[i].data);
        f_out << data << ";" << serialized_nodes[i].rand_index;
        if (i < count - 1)
        {
            f_out << "\n";
        }
    }
    f_out.close();

    return true;
}

} //list_serializer

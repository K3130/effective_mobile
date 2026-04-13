#pragma once

#include <string>

namespace
{
    static const size_t DATA_LEN = 1001;
}

namespace list_serializer
{

struct ListNode
{
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;
    std::string data;
};

struct SerializedNode
{
    int prev_index;
    int next_index;
    int rand_index;
    char data[DATA_LEN];
};

class ListSerializer
{
public:
    ListSerializer();
    ~ListSerializer();

public:
    ListNode* create_list_from_file(const std::string& file);
    void serialize_to_file(ListNode* head, const std::string& file);
    bool deserialize_bin_to_text(const std::string& file_in, const std::string& file_out);

};

} //list_serializer



#include "list_serializer.h"

#include <iostream>
#include <fstream>

int main()
{
    list_serializer::ListSerializer serializer;
    list_serializer::ListNode* list = serializer.create_list_from_file( "inlet.in" );

    if ( list )
    {
        serializer.serialize_to_file( list, "outlet.out" );
        std::cout << "Successfully serialized list to outlet.out" << std::endl;
    }
    else
    {
        std::cerr << "Failed to parse input file" << std::endl;
        return 1;
    }

    if ( serializer.deserialize_bin_to_text( "outlet.out", "inlet_copy.in" ) )
    {
        std::cout << "Successfully deserialized binary to inlet_copy.in" << std::endl;
        std::cout << "Content of inlet_copy.in:" << std::endl;
        std::ifstream check_file( "inlet_copy.in" );
        std::string line;
        while ( std::getline( check_file, line ) )
        {
            std::cout << line << std::endl;
        }
        check_file.close();
    }
    else
    {
        std::cerr << "Failed to deserialize binary to text" << std::endl;
        return 1;
    }

    return 0;
}

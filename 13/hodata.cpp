#include <memory>
#include <string>

struct File
{
    struct HotData
    {
        std::string path;
        unsigned _id;
    };

    struct ColdData
    {
        using Date = unsigned; // imagine a more complex date type
        Date created;
        Date last_modified;
        bool favourite;
        unsigned owner_id;
    };

    HotData hot_Data;
    ColdData cold_data;
};

int main()
{
    return 0;
}
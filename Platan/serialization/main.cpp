#include <iostream>
#include <string>
#include <vector>

using namespace std;

string serialize(vector<string> data)
{
    string result;
    for (auto str : data)
    {
        int length = str.length();
        char* c = (char*)&length;
        for (int i = 0; i < sizeof(int); ++i)
            result += c[i];
        result += str;
    }
    return result;
}

int getInt(string input, int start)
{
    int length = 0;
    for (int i = start; i < start + sizeof(int); ++i)
    {
        length += input[i] << (sizeof(char) * 8 * (start - i));
    }
    return length;
}

vector<string> deserialize(string ser_data)
{
    vector<string> result;
    int start = 0;

    while(start != ser_data.length())
    {
        int length = getInt(ser_data, start);
        result.push_back(ser_data.substr(start + sizeof(int), length));
          start +=  sizeof(int) + length;
    }

    return result;
}

int main()
{
    vector<string> data;
    data.push_back("hello");
    data.push_back("google");
    data.push_back("world");
    string ser = serialize(data);
    cout << ser << endl;
    deserialize(ser);
    return 0;
}


#ifndef string_h
#define string_h
#include <iostream>

class String
{
    private:

    public:

    char* text;

    String()
    {
        this->text = "";
    }

    String (char* text)
    {
        this->text = text;
    }

    int length()
    {
        int length = 0;

        while (text[length] != '\0')
        {
            length++;
        }

        return length;
    }

    int countChar(char c)
    {
        int countChar = 0;
        int i = 0;

        while (text[i] != '\0')
        {
            if (text[i] == c)
            {
                countChar++;
            }

            i++;
        }

        return countChar;
    }

    int findFirstOf(char c)
    {
        int i = 0;

        while (text[i] != '\0')
        {
            if (text[i] == c)
            {
                return i;
            }

            i++;
        }

        return 0;
    }

    String* split(int index)
    {
        if (index > length())
        {
            index = length();
        }

        char* str1 = new char[index];

        str1[index - 1] = '\0';

        char* str2 = new char[length() - index];

        str2[length() - index - 1] = '\0';

        int i = 0;

        while (i != index)
        {
            str1[i] = text[i];

            i++;
        }

        while (text[i] != '\0')
        {
            str2[i - index] = text[i];

            i++;
        }

        String resultS1;
        resultS1.text = str1;

        String resultS2;
        resultS2.text = str2;

        String* result = new String[2];
        result[0] = resultS1;
        result[1] = resultS2;
        return result;
    }

    String operator+(String s)
    {
        int len = length() + s.length() + 1;

        char* str = new char[len];

        for (int i = 0; i < length(); i++)
        {
            str[i] = text[i];
        }

        for (int i = 0; i < s.length(); i++)
        {
            str[length() + i] = s.text[i];
        }

        str[len - 1] = '\0';

        String resultS;
        resultS.text = str;
        return resultS;
    }

    char operator[](int index)
    {
        return text[index];
    }

    bool findstr(char* str)
    {
        int len = 0;

        while (str[len] != '\0')
        {
            len++;
        }

        int textI = 0;
        int strI = 0;

        while (text[textI] != '\0')
        {
            if (text[textI] == str[strI])
            {
                strI++;
            } 
            else
            {
                strI = 0;
            }

            if (strI == len)
            {
                return true;
            }

            textI++;
        }

        return false;
    }
};

/*int main()
{
    String str1("zxc123zxc");

    std::cout << str1.text << std::endl;

    char c = 'z';
    
    std::cout << str1.length() << std::endl;

    std::cout << str1.countChar(c) << std::endl;

    std::cout << str1.findFirstOf(c) << std::endl;

    String* strSplit = str1.split(4);
    String str2 = strSplit[0];
    String str3 = strSplit[1];
    std::cout << str2.text << " " << str3.text << std::endl;

    String str4 = str1 + str2;
    std::cout << str4.text << std::endl;

    std::cout << str1[3] << std::endl;

    char* testChar1 = "zxc";
    char* testChar2 = "cxz"; 

    if (str1.findstr(testChar1))
    {
        std::cout << "YES" << std::endl;
    }

    if (not str1.findstr(testChar2))
    {
        std::cout << "NO" << std::endl;
    }

    return 0;
}*/

#endif

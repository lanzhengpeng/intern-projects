### 【1880】replace characters in a string 替换字符串中的字符

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static logical replace_characters_in_a_string(char *from, char *replace,  
        char *with, char *into, logical verbose)  
    {  
        char  
            *loc,  
            rest[133],  
            *start;  
        strcpy(into, from);  
        if (strstr(from, replace) == NULL) return FALSE;  
        if (verbose) WRITE("Replacing \"");  
        if (verbose) WRITE(from);  
        start = into;  
        while ((loc = strstr(start, replace)) != NULL)  
        {  
            strcpy(rest, loc + strlen(replace));  
            strcpy(loc, with);  
            strcat(into, rest);  
            start = loc + strlen(with);  
        }  
        if (verbose) WRITE("\" with \"");  
        if (verbose) WRITE(into);  
        if (verbose) WRITE("\"\n");  
        return TRUE;  
    }

```

#### 代码解析

> 这段代码定义了一个函数replace_characters_in_a_string，用于在字符串中查找并替换指定的子串。
>
> 函数的主要功能包括：
>
> 1. 初始化目标字符串into为源字符串from的副本。
> 2. 如果源字符串from中不包含待替换的子串replace，则返回FALSE。
> 3. 通过一个循环，在目标字符串into中查找待替换的子串replace，每次找到后将其替换为with，并将剩余的字符串拼接在替换后的字符串后面。
> 4. 在循环结束后，如果verbose参数为真，则通过WRITE宏输出替换前后的字符串。
> 5. 函数最终返回TRUE表示替换成功。
>
> 该函数的参数包括：
>
> 这个函数主要用于字符串处理，实现字符串中的子串查找和替换功能。
>

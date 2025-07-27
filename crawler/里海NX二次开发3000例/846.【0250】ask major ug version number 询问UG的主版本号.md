### 【0250】ask major ug version number 询问UG的主版本号

#### 代码

```cpp
    static int ask_major_ug_version_number(void)  
    {  
        int  
            rel_no;  
        char  
            *major;  
        UF_CALL(UF_get_release(&major));  
        sscanf(major, "NX V%d", &rel_no);  
        if (rel_no == 0) sscanf(major, "V%d", &rel_no);  
        UF_free(major);  
        return rel_no;  
    }

```

#### 代码解析

> 这是段用于获取NX软件版本号的二次开发代码。具体来说，代码首先通过UF_get_release函数获取NX软件的版本字符串，然后通过sscanf函数解析字符串以获取版本号。在解析时，代码首先尝试匹配NX V%d的格式，如果失败则尝试匹配V%d的格式。最后，释放版本字符串占用的内存，并返回解析得到的版本号。
>
> 这段代码的主要功能是获取NX软件的版本号，为后续的版本判断和处理提供依据。
>

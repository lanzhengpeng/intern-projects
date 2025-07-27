### 【1824】read string value from text file 从文本文件中读取字符串值

#### 代码

```cpp
    static void build_unique_temp_name(char *fspec, int ftype)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, ftype, unique, fspec));  
    }  
    //  This assumes that the desired value is on the first line in the text file which has the given value_name string.  
    static logical read_string_value_from_text_file(char *fspec, char *value_name,  
        char *value)  
    {  
        logical  
            got_it = FALSE;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            *found;  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL)  
            {  
                found =  strstr(a_line, value_name);  
                if (found == NULL) continue;  
                found = found + strlen(value_name) + 1;  
                while (found[0] == ' ') found = found + 1; //strip leading spaces  
                found[strlen(found)-1] = '\0';  // strip the newline character  
                strcpy(value, found);  
                got_it = TRUE;  
                break;  
            }  
            fclose(the_file);  
        }  
        return got_it;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于NX CAD系统的二次开发。主要包含两个函数：
>
> 1. build_unique_temp_name：此函数用于生成一个唯一的临时文件名。它会从UGII_TMP_DIR环境变量中获取临时目录的路径，然后调用一个未公开的API函数uc4577生成一个唯一的文件名前缀，最后调用另一个未公开的API函数uc4575将文件名前缀、文件类型和临时目录拼接成完整的临时文件路径，并存储在fspec参数指向的字符串中。
> 2. read_string_value_from_text_file：此函数用于从文本文件中读取特定键对应的字符串值。它会打开指定的文件，逐行读取，如果某行包含指定的键value_name，则提取该行中键后面的字符串作为值，去除字符串前后的空格和换行符，并将值存储在value参数指向的字符串中。如果找到匹配的键，函数返回TRUE，否则返回FALSE。
>
> 总体来说，这段代码的目的是为了从NX环境变量中获取临时目录，生成唯一的临时文件名，以及从文本文件中提取特定键对应的字符串值。这些功能在NX二次开发中是非常实用的。
>

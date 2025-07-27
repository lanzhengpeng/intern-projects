### 【1817】read coordinates from text file 从文本文件中读取坐标

#### 代码

```cpp
    //  This assumes that the desired coordinates are found at the end of three  
    //  lines starting with the first line in the text file which has the given  
    //  value_name string.  
    static logical read_coordinates_from_text_file(char *fspec, char *value_name,   
        double value[3])  
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
                value[0] = atof(strrchr(found, '=')+1);  
                fgets(a_line, MAX_LINE_SIZE, the_file);  
                value[1] = atof(strrchr(a_line, '=')+1);  
                fgets(a_line, MAX_LINE_SIZE, the_file);  
                value[2] = atof(strrchr(a_line, '=')+1);  
                got_it = TRUE;  
                break;  
            }  
            fclose(the_file);  
        }  
        return got_it;  
    }

```

#### 代码解析

> 这段代码是用于从文本文件中读取特定坐标值的NX二次开发代码。其基本思路是：
>
> 1. 以只读方式打开指定的文本文件。
> 2. 逐行读取文件内容，直到找到包含特定value_name字符串的行。
> 3. 在找到该行后，分别从该行及其后两行中提取坐标值，并将其存储在value数组中。
> 4. 如果找到并成功读取坐标值，则返回true，否则返回false。
>
> 具体来说，代码使用了strstr函数来查找包含value_name的行，然后使用strrchr和atof函数提取坐标值。需要注意的是，代码假设value_name所在行的后两行分别包含了y坐标和z坐标。
>
> 这段代码简洁高效，实现了从文本文件中读取特定坐标值的功能。
>

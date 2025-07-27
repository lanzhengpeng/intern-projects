### 【1539】list parts in session 列出会话中的零件

#### 代码

```cpp
    static void list_parts_in_session(void)  
    {  
        int  
            ii,  
            n,  
            status;  
        tag_t  
            part;  
        char  
            part_fspec[MAX_FSPEC_SIZE+1],  
            *part_name;  
        WRITE_D(n = UF_PART_ask_num_parts());  
        for (ii = 0; ii < n; ii++)  
        {  
            part = UF_PART_ask_nth_part(ii);  
            UF_CALL(UF_PART_ask_part_name(part, part_fspec));  
            UF_CALL(UF_PART_file_name_for_display(part_fspec, &part_name));  
            ECHO(part_name);  
            status = UF_PART_is_loaded(part_fspec);  
            switch (status)  
            {  
                case 0:  
                    ECHO(" - not loaded\n");  
                    break;  
                case 1:  
                    ECHO(" - fully loaded\n");  
                    break;  
                case 2:  
                    ECHO(" - partially loaded\n");  
                    break;  
                default:  
                    ECHO(" - unknown load ");  
                    WRITE_D(status);  
                    break;  
            }  
            UF_free(part_name);  
        }  
    }

```

#### 代码解析

> 这段代码用于列出NX会话中当前加载的所有零件的信息。具体步骤如下：
>
> 1. 调用UF_PART_ask_num_parts()获取当前会话中零件的数量n。
> 2. 通过循环遍历所有零件，使用UF_PART_ask_nth_part(ii)获取每个零件的tag。
> 3. 调用UF_PART_ask_part_name()获取零件的文件名，并将其存储在part_fspec数组中。
> 4. 使用UF_PART_file_name_for_display()获取零件的显示名称part_name，并将其输出。
> 5. 调用UF_PART_is_loaded()判断零件的加载状态，根据返回值输出“- not loaded”、“- fully loaded”、“- partially loaded”或“- unknown load”。
> 6. 每次循环结束后释放part_name的内存。
> 7. 循环结束后，程序结束。
>
> 总的来说，这段代码通过遍历所有零件，输出其显示名称和加载状态，用于显示当前会话中所有零件的列表。
>

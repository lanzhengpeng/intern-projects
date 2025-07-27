### 【1177】ensure part fully loaded 确保零件完全加载

#### 代码

```cpp
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
        }  
    }  
    static void ensure_part_fully_loaded(tag_t part)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        if (!UF_CALL(UF_PART_ask_part_name(part, partname))  
            && ((UF_PART_is_loaded(partname)) != 1))  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要实现以下功能：
>
> 1. 报告加载状态：函数report_load_status用于报告NX部件的加载状态，通过遍历部件的加载状态数组，获取每个部件的加载失败消息，并输出到控制台。如果加载状态数组不为空，则释放加载状态内存。
> 2. 确保部件完全加载：函数ensure_part_fully_loaded用于确保指定的NX部件被完全加载。首先，通过调用UF_PART_ask_part_name获取部件名称。然后，通过UF_PART_is_loaded判断部件是否已加载。如果未加载，则调用UF_PART_open_quiet尝试静默加载，并获取加载状态。最后，如果存在加载失败的部件，则调用report_load_status报告加载状态。
> 3. 加载失败处理：代码使用UF_PART_load_status_p_t结构体来保存加载状态信息，包括加载失败的部件数量、文件名数组、状态数组等。通过UF_get_fail_message获取加载失败消息。
> 4. 内存管理：在释放加载状态内存时，代码使用了UF_CALL宏来简化错误处理。
>
> 总体来说，这段代码提供了检查NX部件加载状态并在必要时尝试加载的功能，同时在加载失败时能够输出提示信息。
>

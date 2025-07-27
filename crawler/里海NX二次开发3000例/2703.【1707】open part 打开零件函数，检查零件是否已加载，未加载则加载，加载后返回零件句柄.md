### 【1707】open part 打开零件函数，检查零件是否已加载，未加载则加载，加载后返回零件句柄

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
    static logical open_part(char *fspec, tag_t *part)  
    {  
        UF_PART_load_status_t  
            status;  
        if (!UF_PART_is_loaded(fspec))  
        {  
            UF_CALL(UF_PART_open(fspec, part, &status));  
            if (status.n_parts > 0) report_load_status(&status);  
            return (!status.failed);  
        }  
        else  
        {  
            *part = UF_PART_ask_part_tag(fspec);  
            if (!UF_CALL(UF_PART_set_display_part(*part)))  
                return TRUE;  
            else  
                return FALSE;  
        }  
    }

```

#### 代码解析

> 这段代码是用于NX Open的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个名为report_load_status的静态函数，用于报告零件加载状态。该函数接收零件加载状态结构体作为参数，并输出加载失败的文件名和失败原因。
> 2. 定义了一个名为open_part的静态函数，用于打开NX零件文件。该函数接收零件文件路径和零件句柄两个参数。
> 3. 在open_part函数中，首先调用UF_PART_is_loaded函数判断零件是否已加载。
> 4. 如果零件未加载，调用UF_PART_open函数加载零件，并获取加载状态。
> 5. 如果加载状态中有失败信息，调用report_load_status函数报告加载失败的文件名和原因。
> 6. 如果零件已加载，则直接获取零件句柄，并设置当前显示的零件。
> 7. open_part函数返回加载是否成功的逻辑值。
> 8. 通过调用open_part函数，可以方便地打开NX零件文件，并获取零件句柄。如果加载失败，会输出加载失败的文件名和原因。
>
> 总的来说，这段代码提供了加载NX零件文件并获取零件句柄的功能，同时提供了加载失败的错误报告，方便进行二次开发。
>

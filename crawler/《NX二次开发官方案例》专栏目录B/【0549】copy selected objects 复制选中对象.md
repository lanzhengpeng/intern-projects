### 【0549】copy selected objects 复制选中对象

#### 代码

```cpp
    /*HEAD COPY_SELECTED_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog 是 V18 中的新增功能，请只提供翻译，不要添加无关内容。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    static void copy_objects(int n_objects, tag_t *objects)  
    {  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_export_options_t  
            options = { FALSE, UF_PART_remove_params, UF_PART_copy_exp_shallowly };  
        UF_CALL(UF_PART_ask_part_name(part, part_name));  
        UF_CALL(UF_PART_export_with_options(part_name, n_objects, objects, &options));  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *objects;  
        while ((n = select_objects("Copy in place", &objects)) != 0)  
        {  
            copy_objects(n, objects);  
            UF_free(objects);  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发示例，用于实现复制选定的对象功能。
>
> 主要功能和步骤包括：
>
> 1. 包含必要的NX API头文件。
> 2. 定义错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 3. 定义选择对象函数select_objects，使用NX选择对话框选择对象，并返回对象数组。
> 4. 定义复制对象函数copy_objects，将选定的对象导出并复制到当前部件。
> 5. 定义主函数do_it，循环调用select_objects选择对象，然后调用copy_objects复制对象。
> 6. 定义ufusr入口函数，初始化NX API，调用do_it函数，然后终止NX API。
> 7. 定义卸载函数，返回立即卸载标志。
>
> 通过以上步骤，用户可以重复选择对象并复制到当前部件，实现了对象的复制功能。代码结构清晰，注释详细，可作为NX二次开发的参考示例。
>

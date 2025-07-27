### 【1039】delete selected objects 删除选定的对象

#### 代码

```cpp
    /*HEAD DELETE_SELECTED_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_objs,  
            *statuses;  
        tag_t  
            *objs;  
        char  
            err[133],  
            msg[1000];  
        while ((n_objs = select_objects("Delete", &objs)) > 0)  
        {  
            UF_CALL(UF_OBJ_delete_array_of_objects(n_objs, objs, &statuses));  
            for (ii = 0; ii < n_objs; ii++)  
            {  
                if (statuses[ii] == 0) continue;  
                UF_get_fail_message(statuses[ii], err);  
                UF_UI_open_listing_window();  
                sprintf(msg, "object tag %d: delete returned %d - %s\n", objs[ii],  
                    statuses[ii], err);  
                UF_UI_write_listing_window(msg);  
            }  
            UF_free(objs);  
            UF_free(statuses);  
        }  
    }  
    /* qq3123197280 */  
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

> 这段NX二次开发代码的主要功能是实现了删除选中的NX对象的功能。代码的主要逻辑包括：
>
> 1. 包含了必要的NX Open头文件，以使用NX Open提供的API。
> 2. 定义了report_error函数，用于在出错时报告错误信息。
> 3. 定义了select_objects函数，用于弹出选择对话框，让用户选择要删除的对象，并返回选中的对象数组。
> 4. 定义了do_it函数，用于循环调用select_objects获取要删除的对象，然后调用UF_OBJ_delete_array_of_objects API批量删除这些对象。在删除过程中，如果出现错误，会报告错误信息。
> 5. 定义了ufusr函数，这是NX Open二次开发程序的入口函数。在这个函数中，首先调用UF_initialize初始化环境，然后调用do_it执行删除操作，最后调用UF_terminate终止环境。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发应用程序可以立即卸载。
>
> 总体来说，这段代码实现了简单的NX对象批量删除功能，采用面向对象的方式组织代码，结构清晰。
>

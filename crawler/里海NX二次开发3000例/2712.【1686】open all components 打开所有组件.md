### 【1686】open all components 打开所有组件

#### 代码

```cpp
    /*HEAD OPEN_ALL_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数。 */  
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
    static logical add_to_cset(tag_t comp, void *user_data)  
    {  
        tag_t  
            *cset = (tag_t *)user_data;  
        UF_CALL(UF_ASSEM_add_to_cset(*cset, comp, FALSE));  
        return TRUE;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            all_cset,  
            part = UF_PART_ask_display_part(),  
            tmp_cset;  
        UF_PART_load_status_t  
            status;  
    /*  
        If you pass UF_PART_open_cset the "all comp" cset, the components will not  
        unblank properly, so create and use a new cset instead - see PRs 4529272  
        and 5372470.  
    */  
        UF_CALL(UF_ASSEM_create_cset(part, "TempCSet", &tmp_cset));  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &all_cset));  
        UF_CALL(UF_ASSEM_apply_to_cset(all_cset, add_to_cset, &tmp_cset));  
        UF_CALL(UF_PART_open_cset(tmp_cset, &status));  
        report_load_status(&status);  
        UF_CALL(UF_OBJ_delete_object(tmp_cset));  
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

> 这段NX二次开发代码主要用于加载和显示NX部件中的所有组件。具体来说：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了组件集合添加函数add_to_cset，用于将组件添加到组件集合中。
> 3. 定义了加载状态报告函数report_load_status，用于输出部件的加载状态信息。
> 4. 定义了主函数do_it，用于执行以下操作：获取当前显示的部件part。创建一个临时的组件集合tmp_cset。获取当前部件中的所有组件集合all_cset。将所有组件添加到临时组件集合tmp_cset中。以临时组件集合为参数加载部件，并输出加载状态。删除临时组件集合。
> 5. 获取当前显示的部件part。
> 6. 创建一个临时的组件集合tmp_cset。
> 7. 获取当前部件中的所有组件集合all_cset。
> 8. 将所有组件添加到临时组件集合tmp_cset中。
> 9. 以临时组件集合为参数加载部件，并输出加载状态。
> 10. 删除临时组件集合。
> 11. 定义了ufusr主函数，用于初始化NX并调用do_it函数，然后终止NX。
> 12. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示在NX关闭时立即卸载该程序。
>
> 总的来说，这段代码通过创建临时组件集合，加载并显示NX部件中的所有组件，并在加载后删除临时组件集合。
>

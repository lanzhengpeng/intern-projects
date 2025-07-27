### 【3011】unblank all components 取消所有组件的隐藏

#### 代码

```cpp
    /*HEAD UNBLANK_ALL_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能，用于打印系统日志信息。 */  
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
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
            UF_CALL(UF_OBJ_set_blank_status(comp, FALSE));  
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

> 根据代码内容，这是一段NX Open C++的二次开发代码，其功能是遍历当前显示的部件中的所有组件，并取消它们的隐藏状态。
>
> 主要代码部分包括：
>
> 1. 头文件包含：包含了NX Open C++的常用头文件。
> 2. 错误报告函数：定义了report_error函数，用于在出错时打印错误信息。
> 3. 组件遍历函数：定义了ask_next_component函数，用于遍历当前部件中的组件，并返回下一个组件的tag。
> 4. 主函数：定义了do_it函数，该函数遍历当前显示的部件中的所有组件，并取消它们的隐藏状态。
> 5. 用户函数：定义了ufusr函数，这是NX二次开发的主入口函数，用于初始化环境、调用主函数和终止环境。
> 6. 卸载函数：定义了ufusr_ask_unload函数，用于设置二次开发的卸载方式。
>
> 总的来说，这段代码实现了在NX中取消当前显示部件中所有组件的隐藏状态的功能。
>

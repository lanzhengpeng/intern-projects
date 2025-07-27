### 【2507】report tag of specified category name 报告指定类别名称的标签

#### 代码

```cpp
    /*HEAD REPORT_TAG_OF_SPECIFIED_CATEGORY_NAME CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog 是 V18 版本新增的函数，主要用于打印系统日志信息。 */  
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
    static tag_t ask_category_tag(char *name)  
    {  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, UF_layer_category_type,  
            FALSE, &object));  
        return object;  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            category;  
        char  
            cat_name[MAX_ENTITY_NAME_SIZE+1] = { "ALL" };  
        while (prompt_for_text("Enter category name", cat_name))  
            WRITE_D(category = ask_category_tag(cat_name));  
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

> 这段代码是NX二次开发代码，主要用于查询指定的图层类别名称对应的图层类别标签。代码的主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时报告错误信息。
> 2. 定义了一个函数ask_category_tag，用于根据给定的图层类别名称查询对应的图层类别标签。
> 3. 定义了一个函数prompt_for_text，用于在交互式窗口中提示用户输入字符串。
> 4. 定义了一个宏WRITE_D，用于在列表窗口中输出整数值。
> 5. 定义了一个函数write_integer_to_listing_window，用于在列表窗口中输出整数值。
> 6. 定义了一个函数do_it，用于循环提示用户输入图层类别名称，并输出对应的图层类别标签。
> 7. 定义了ufusr函数，作为二次开发的入口函数，在其中初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 通过这段代码，用户可以交互式地输入图层类别名称，并查询到对应的图层类别标签，为后续的图层操作提供了便利。代码结构清晰，易于理解和扩展。
>

### 【2509】report tag of specified named group 报告指定名称组的标签

#### 代码

```cpp
    /*HEAD REPORT_TAG_OF_SPECIFIED_NAMED_GROUP CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是在V18版本中新增的。 */  
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
    static tag_t ask_named_group_tag(char *name)  
    {  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, UF_group_type,  
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
            group;  
        char  
            grp_name[MAX_ENTITY_NAME_SIZE+1] = { "TEST" };  
        while (prompt_for_text("Enter group name", grp_name))  
            WRITE_D(group = ask_named_group_tag(grp_name));  
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

> 这是段NX二次开发代码，主要功能是查询指定名称的组并显示其tag值。以下是代码的主要功能：
>
> 1. 定义了一个宏UF_CALL，用于调用NX API函数，并在出错时报告错误。
> 2. 定义了一个错误报告函数report_error，用于在出错时打印错误信息。
> 3. 定义了一个函数ask_named_group_tag，用于根据组名称查询组的tag。
> 4. 定义了一个函数prompt_for_text，用于提示用户输入组名称。
> 5. 定义了一个宏WRITE_D，用于将整数写入listing窗口。
> 6. 定义了一个函数do_it，用于循环提示用户输入组名称，查询组的tag并显示。
> 7. 定义了ufusr函数，用于初始化NX，执行do_it函数，然后终止NX。
> 8. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 总体来说，这段代码实现了查询指定名称的NX组并显示其tag值的功能，通过NX API调用实现，并具有良好的错误处理。
>

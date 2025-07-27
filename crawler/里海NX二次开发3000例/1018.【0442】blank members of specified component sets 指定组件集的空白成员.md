### 【0442】blank members of specified component sets 指定组件集的空白成员

#### 代码

```cpp
    /*HEAD BLANK_MEMBERS_OF_SPECIFIED_COMPONENT_SETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是 V18 版本中的新功能。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_component_set(tag_t part, tag_t cset)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_set_type, &cset));  
        return (cset);  
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
    static tag_t ask_cset_tag(char *cset_name)  
    {  
        tag_t  
            cset = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, cset_name,  
            UF_component_set_type, FALSE, &cset));  
        return cset;  
    }  
    /* 里海 */  
    static logical blank_component(tag_t comp, void *user_data)  
    {  
        UF_CALL(UF_OBJ_set_blank_status(comp, UF_OBJ_BLANKED));  
        return TRUE;  
    }  
    static void list_all_csets(void)  
    {  
        tag_t  
            cset = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            cset_name[UF_OBJ_NAME_LEN + 1];  
        WRITE("component sets in part\n");  
        while ((cset = ask_next_component_set(part, cset)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(cset, cset_name));  
            WRITE_S(cset_name);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            cset;  
        char  
            cset_name[UF_OBJ_NAME_LEN + 1] = { "" };  
        list_all_csets();  
        while (prompt_for_text("Blank components in cset", cset_name))  
            if ((cset = ask_cset_tag(cset_name)) != NULL_TAG)  
                UF_CALL(UF_ASSEM_apply_to_cset(cset, blank_component, NULL));  
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

> 这段代码是一个NX Open C++应用程序，用于在NX部件中标记指定组件集中的所有组件。
>
> 主要功能包括：
>
> 1. 列出部件中的所有组件集。
> 2. 提示用户输入组件集名称，并获取该组件集的标签。
> 3. 遍历指定组件集中的所有组件，并将它们标记为隐藏。
> 4. 使用UFASSEM库的UF_ASSEM_apply_to_cset函数，对组件集执行自定义函数，用于隐藏组件。
> 5. 提供错误报告函数，用于打印出函数调用失败的错误信息。
> 6. 提供日志函数，用于将信息写入列表窗口。
> 7. 提供UF_UNLOAD_IMMEDIATELY卸载类型，以立即卸载应用程序。
>
> 总体而言，这段代码实现了隐藏指定组件集的功能，并具有完善的错误处理和日志记录功能。
>

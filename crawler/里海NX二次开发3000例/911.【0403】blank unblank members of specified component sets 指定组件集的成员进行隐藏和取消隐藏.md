### 【0403】blank unblank members of specified component sets 指定组件集的成员进行隐藏和取消隐藏

#### 代码

```cpp
    /*HEAD BLANK_UNBLANK_MEMBERS_OF_SPECIFIED_COMPONENT_SETS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中新增的，请只回答翻译内容，不要添加其他内容。 */  
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
    static logical blank_unblank_component(tag_t comp, void *user_data)  
    {  
        int  
            *option = (int *)user_data;  
        UF_CALL(UF_OBJ_set_blank_status(comp, 2 - *option));  
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
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 译:returns 1 or 2的翻译是：返回1或2。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            option = 1;  
        tag_t  
            cset;  
        char  
            cset_name[UF_OBJ_NAME_LEN + 1] = { "" };  
        list_all_csets();  
        while (pick_one_of_two_choices("Process cset members", "Blank", "Unblank",  
            &option) && prompt_for_text("Enter cset name to process", cset_name))  
        {  
            if ((cset = ask_cset_tag(cset_name)) != NULL_TAG)  
            {  
                UF_CALL(UF_ASSEM_apply_to_cset(cset, blank_unblank_component,  
                    &option));  
                option = 3 - option;  
            }  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误处理：定义了错误处理函数report_error，用于在调用NX API函数出错时打印错误信息。
> 2. 组件集遍历：定义了组件集遍历函数ask_next_component_set，用于遍历当前部件的所有组件集。
> 3. 组件集名称获取：定义了组件集名称获取函数ask_cset_tag，根据名称获取组件集的tag。
> 4. 组件显隐设置：定义了组件显隐设置函数blank_unblank_component，用于设置组件的显隐状态。
> 5. 组件集遍历打印：定义了组件集遍历打印函数list_all_csets，用于打印当前部件的所有组件集名称。
> 6. 选择显隐操作：定义了选择显隐操作函数pick_one_of_two_choices，用于让用户选择是要显隐还是隐藏组件集。
> 7. 组件集显隐处理：定义了组件集显隐处理函数do_it，是主函数，用于遍历组件集，根据用户选择执行显隐操作。
> 8. 用户函数：定义了用户函数ufusr，用于初始化NX系统，调用do_it函数，然后终止NX系统。
> 9. 卸载提示：定义了卸载提示函数ufusr_ask_unload，用于提示用户立即卸载该程序。
>
> 整体而言，这段代码通过组件集的遍历，实现了对组件显隐状态的控制，使用户可以方便地隐藏或显示指定的组件集。
>

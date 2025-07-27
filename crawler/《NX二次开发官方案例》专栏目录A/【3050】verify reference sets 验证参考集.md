### 【3050】verify reference sets 验证参考集

#### 代码

```cpp
    /*HEAD VERIFY_REFERENCE_SETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，UF_print_syslog是V18版本中新增的函数。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static tag_t ask_tag_of_reference_set(char *refset_name)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            refset = NULL_TAG;  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, refset_name,  
            UF_reference_set_type, FALSE, &refset));  
        return refset;  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *members,  
            ref_set;  
        char  
            msg[MAX_LINE_SIZE+1],  
            ref_set_name[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while (prompt_for_text("Verify reference set", ref_set_name))  
        {  
            ref_set = ask_tag_of_reference_set(ref_set_name);  
            if (ref_set == NULL_TAG)  
            {  
                sprintf(msg, "Reference set %s does not exist", ref_set_name);  
                uc1601(msg, TRUE);  
                continue;  
            }  
            UF_CALL(UF_ASSEM_ask_ref_set_members(ref_set, &n, &members));  
            set_highlight_object_array(n, members, TRUE);  
            sprintf(msg, "%s has %d members", ref_set_name, n);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n, members, FALSE);  
            if (n > 0) UF_free(members);  
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

> 这段代码是一个NX Open C++应用程序，主要功能是验证NX中的参考集。
>
> 主要流程如下：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了一个提示输入文本的函数prompt_for_text，用于提示用户输入参考集的名称。
> 3. 定义了一个获取参考集tag的函数ask_tag_of_reference_set，通过参考集名称查找并返回其tag。
> 4. 定义了一个设置高亮显示的函数set_highlight_object_array，用于设置参考集成员的高亮显示。
> 5. 定义了一个主函数do_it，用于实现参考集验证的主要逻辑：不断提示用户输入参考集名称获取参考集的tag查询参考集的成员，并设置高亮显示打印参考集的成员数量移除高亮显示
> 6. 不断提示用户输入参考集名称
> 7. 获取参考集的tag
> 8. 查询参考集的成员，并设置高亮显示
> 9. 打印参考集的成员数量
> 10. 移除高亮显示
> 11. 定义了ufusr函数作为程序的入口，初始化NX Open环境，调用do_it函数，然后终止NX Open环境。
> 12. 定义了ufusr_ask_unload函数，用于卸载应用程序。
>
> 整个应用程序通过交互方式让用户输入参考集名称，然后高亮显示参考集的成员，以验证参考集是否正确。如果参考集不存在，会给出提示。
>

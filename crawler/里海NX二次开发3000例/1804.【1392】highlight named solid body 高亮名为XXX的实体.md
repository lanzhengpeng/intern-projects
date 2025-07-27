### 【1392】highlight named solid body 高亮名为XXX的实体

#### 代码

```cpp
    /*HEAD HIGHLIGHT_NAMED_SOLID_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18，请提供相关上下文以便我为您提供准确的翻译。 */  
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
    static tag_t ask_next_solid_body_of_name(tag_t part, char *name, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, UF_solid_type,  
            TRUE, &body)) && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SOLID_BODY) return body;  
            }  
        }  
        return NULL_TAG;  
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
    static void do_it(void)  
    {  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            name[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while (prompt_for_text("Highlight solids named?", name))  
        {  
            while ((body = ask_next_solid_body_of_name(part,name,body))!=NULL_TAG)  
            {  
                UF_CALL(UF_DISP_set_highlight(body, TRUE));  
                uc1601(name, TRUE);  
                UF_CALL(UF_DISP_set_highlight(body, FALSE));  
            }  
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

> 这段代码是用于NX的二次开发，其主要功能是高亮显示指定名称的实体。
>
> 1. 代码结构：包含头文件和函数定义，以及主函数 ufusr。
> 2. 功能介绍：ufusr()：初始化NX，调用 do_it() 函数，并在结束时终止NX。do_it()：询问用户输入实体名称，并高亮显示名称匹配的实体。ask_next_solid_body_of_name()：根据名称和类型循环遍历实体，返回下一个符合条件的实体。prompt_for_text()：提示用户输入文本，并返回输入结果。report_error()：报告错误信息。
> 3. ufusr()：初始化NX，调用 do_it() 函数，并在结束时终止NX。
> 4. do_it()：询问用户输入实体名称，并高亮显示名称匹配的实体。
> 5. ask_next_solid_body_of_name()：根据名称和类型循环遍历实体，返回下一个符合条件的实体。
> 6. prompt_for_text()：提示用户输入文本，并返回输入结果。
> 7. report_error()：报告错误信息。
> 8. 关键点：使用 UF_CALL 宏来检查函数调用是否成功。通过 UF_DISP_set_highlight 来设置高亮显示。通过 UF_OBJ_cycle_by_name_and_type 来根据名称和类型遍历实体。通过 UF_MODL_ask_body_type 来查询实体的类型。
> 9. 使用 UF_CALL 宏来检查函数调用是否成功。
> 10. 通过 UF_DISP_set_highlight 来设置高亮显示。
> 11. 通过 UF_OBJ_cycle_by_name_and_type 来根据名称和类型遍历实体。
> 12. 通过 UF_MODL_ask_body_type 来查询实体的类型。
> 13. 其他：该代码实现了高亮显示指定名称实体的功能，并且通过错误检查和用户交互设计，具有较好的鲁棒性。
>

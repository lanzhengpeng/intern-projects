### 【1369】highlight all notes on specified drawing 在指定绘图上高亮所有注释

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ALL_NOTES_ON_SPECIFIED_DRAWING CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 中的新功能，只提供翻译，不要添加无关内容。 */  
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
    static tag_t ask_next_note_on_drawing(tag_t part, char *dwg_name, tag_t a_note)  
    {  
        int  
            subtype,  
            status,  
            type;  
        char  
            drawing_view[MAX_ENTITY_NAME_SIZE+3],  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        sprintf(drawing_view, "%s@0", dwg_name);  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &a_note)) && (a_note != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(a_note, &type, &subtype));  
            if (subtype == UF_draft_note_subtype)  
            {  
                uc6409(a_note, &status, view_name);  
                if (!strcmp(drawing_view, view_name)) return a_note;  
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
        int  
            cnt;  
        tag_t  
            a_note = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            drawing[MAX_ENTITY_NAME_SIZE+1] = { "SH1" },  
            msg[133];  
        while (prompt_for_text("Enter drawing name", drawing))  
        {  
            cnt = 0;  
            while ((a_note=ask_next_note_on_drawing(part,drawing,a_note)) != NULL_TAG)  
            {  
                UF_CALL(UF_DISP_set_highlight(a_note, TRUE));  
                cnt++;  
            }  
            sprintf(msg, "%d notes on drawing %s highlighted", cnt, drawing);  
            uc1601(msg, TRUE);  
            UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
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

> 这段代码是NX的二次开发示例，其主要功能是高亮指定绘图上的所有注释。
>
> 具体来说：
>
> 1. 包含了必要的NX头文件，定义了错误报告函数report_error，用于打印错误信息。
> 2. 定义了ask_next_note_on_drawing函数，用于在指定绘图上获取下一个注释，直到没有注释为止。
> 3. 定义了prompt_for_text函数，用于提示用户输入绘图名称。
> 4. do_it函数实现了主要逻辑，循环提示用户输入绘图名称，然后高亮该绘图上的所有注释。
> 5. ufusr函数是主入口函数，初始化NX环境，调用do_it函数，然后清理环境。
> 6. ufusr_ask_unload函数用于卸载用户函数，返回立即卸载。
>
> 通过以上函数定义，实现了在NX中高亮指定绘图上所有注释的功能。
>

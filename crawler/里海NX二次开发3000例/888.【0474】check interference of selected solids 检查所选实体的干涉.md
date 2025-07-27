### 【0474】check interference of selected solids 检查所选实体的干涉

#### 代码

```cpp
    /*  
        NOTE:  Prior to NX6.0.2, UF_MODL_check_interference may not work correctly  
                on assembly occurrences.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* 里海 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int select_bodies(char *prompt, tag_t **bodies)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select solids or sheets", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp, &cnt,  
            bodies));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*bodies)[ii], FALSE));  
        return cnt;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void do_it(void)  
    {  
        int  
            *codes,  
            cnt,  
            ii,  
            n;  
        tag_t  
            target,  
            *tools;  
        char  
            msg[133];  
        while (((target = select_a_body("Select target body")) != NULL_TAG) &&  
            ((n = select_bodies("Select tool bodies", &tools)) > 0))  
        {  
            UF_CALL(allocate_memory(n * sizeof(int), (void **)&codes));  
            UF_CALL(UF_MODL_check_interference(target, n, tools, codes));  
            UF_CALL(UF_DISP_set_highlight(target, TRUE));  
            cnt = 0;  
            for (ii = 0; ii < n; ii++)  
            {  
                if (codes[ii] == 1)  
                {  
                    UF_CALL(UF_DISP_set_highlight(tools[ii], TRUE));  
                    cnt++;  
                }  
            }  
            sprintf(msg, "%d interferences found", cnt);  
            uc1601(msg, TRUE);  
            for (ii = 0; ii < n; ii++)  
                if (codes[ii] == 1)  
                    UF_CALL(UF_DISP_set_highlight(tools[ii], FALSE));  
            UF_CALL(UF_DISP_set_highlight(target, FALSE));  
            UF_free(codes);  
            UF_free(tools);  
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

> 这段代码是一个用于检测NX装配体中两个实体之间干涉的NX Open API程序。
>
> 主要功能包括：
>
> 1. 提供了一个简单的错误处理函数report_error，用于输出函数调用失败时的错误信息。
> 2. 定义了一个mask_for_bodies函数，用于在对话框中选择实体时只显示实体特征。
> 3. select_a_body函数用于选择目标实体，并高亮显示。
> 4. select_bodies函数用于选择工具实体集，并高亮显示。
> 5. allocate_memory函数用于申请内存。
> 6. do_it函数是主函数，用于循环选择目标实体和工具实体集，调用UF_MODL_check_interference函数检测干涉，并高亮显示干涉的实体。
> 7. ufusr是主函数，用于初始化和执行do_it。
> 8. ufusr_ask_unload函数用于卸载NX Open程序。
>
> 这个程序实现了在NX装配体中检测实体间干涉的功能，并高亮显示干涉的实体，对NX装配体的干涉检查非常有用。
>

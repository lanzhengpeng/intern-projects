### 【2449】reposition selected component 移动选定组件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_mtx.h>  
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
    /* qq3123197280 */  
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp;  
        double  
            trans[3] = { 1,0,0 },  
            rot[9] = { 1,0,0, 0,1,0, 0,0,1 },  
            xform[4][4];  
        UF_UNDO_mark_id_t  
            mark_id;  
    // Move delta X=1  
        UF_MTX4_initialize(1, trans, rot, (double *)xform);  
        while ((comp = select_a_component("Repostion")) != NULL_TAG)  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Reposition Component",   
                &mark_id));  
            UF_CALL(UF_ASSEM_reposition_part_occurrence(comp, xform,  
                UF_ASSEM_establish_override));  
        }  
    }  
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

> 这段代码是一段用于NX软件的二次开发代码，主要功能是选择一个组件并沿X轴移动1单位。下面是该代码的主要部分和功能：
>
> 1. 函数ECHO：用于输出信息到日志窗口和系统日志。
> 2. 宏UF_CALL：用于调用NX的UF函数，并报告错误。
> 3. 函数mask_for_components：用于设置选择组件的类型掩码。
> 4. 函数select_a_component：使用NX选择对话框让用户选择一个组件，并返回组件的tag。
> 5. 函数do_it：是主函数，用于不断让用户选择组件，然后沿X轴移动1单位。
> 6. 函数ufusr：NX的二次开发入口函数，用于初始化NX环境，执行do_it函数，然后终止NX环境。
> 7. 函数ufusr_ask_unload：用于卸载二次开发程序。
>
> 这段代码的主要功能是让用户选择组件，然后沿X轴移动1单位，实现了简单的组件移动功能。
>

### 【1162】edit selected points location 编辑选定点的位置

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <stdarg.h>  
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
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_three_numbers(char *prompt, char *item1, char *item2,  
        char * item3, double *number1, double *number2, double *number3)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[3][16];  
        double  
            da[3];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        strncpy(&menu[2][0], item3, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        resp = uc1609(prompt, menu, 3, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            point;  
        double  
            loc[3];  
        while ((point = select_a_point("Edit location")) != NULL_TAG)  
        {  
            UF_CALL(UF_CURVE_ask_point_data(point, loc));  
            if (prompt_for_three_numbers("Edit point data", "X Abs", "Y Abs",  
                "Z Abs", &loc[0], &loc[1], &loc[2]))  
            {  
                set_undo_mark("Move point");  
                UF_CALL(UF_CURVE_edit_point_data(point, loc));  
                UF_CALL(UF_MODL_update()); // update any associated features  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 定义了ECHO宏用于打印信息和错误，以及UF_CALL宏用于调用NX API，并在出错时报告错误信息。
> 2. 定义了mask_for_points函数，用于设置选择过滤器，仅选择点类型。
> 3. 定义了select_a_point函数，用于提示用户选择一个点，并返回点的tag。
> 4. 定义了prompt_for_three_numbers函数，用于提示用户输入三个数字，并返回输入结果。
> 5. 定义了set_undo_mark函数，用于设置撤销标记。
> 6. 定义了do_it函数，用于循环选择点，获取点的位置，提示用户编辑位置，并更新点数据。
> 7. 在ufusr函数中，初始化NX环境，调用do_it函数实现功能，并在结束时关闭NX环境。
> 8. 在ufusr_ask_unload函数中，设置在卸载用户程序时立即卸载。
>
> 总的来说，这段代码实现了一个简单的NX点编辑功能，用户可以循环选择点并编辑其位置。通过使用选择过滤器和撤销标记，提高了交互的友好性和可靠性。
>

### 【1661】move selected view by specified values 移动选定视图到指定值

#### 代码

```cpp
    /*HEAD MOVE_ALL_VIEWS_TO_SPECIFIED_VALUES CCC UFUN */  
    /* Moves selected drawing view by specified values 里海译:移动选定绘图视图，按指定值。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_BUFSIZE];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char *file, int line, char *call, int irc)  
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
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strcpy(&menu[0][0], item1);  
        strcpy(&menu[1][0], item2);  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static int mask_for_views(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_view_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_view_by_mask(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_views, NULL, &resp,  
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
        char  
            view_name[33] = "";  
        double  
            ref_pt[2],  
            values[2] = { 0.0, 0.0 },  
            view_bounds[4];  
        tag_t  
            view_tag = NULL_TAG;  
        view_tag = select_a_view_by_mask("Select view to move");  
        if (view_tag == NULL_TAG) return;  
        UF_CALL(UF_OBJ_ask_name(view_tag, view_name));  
        ECHO("View Tag: %d\nView Name: %s\n", view_tag, view_name);  
        UF_CALL(uc6485(view_name, view_bounds));  
        UF_CALL(UF_DRAW_ask_drawing_ref_pt(view_tag, ref_pt));  
        ECHO(" Left bound is %f\n", view_bounds[0]);  
        ECHO(" Bottom bound is %f\n", view_bounds[1]);  
        ECHO(" Right bound is %f\n", view_bounds[2]);  
        ECHO(" Top bound is %f\n", view_bounds[3]);  
        ECHO(" Reference Point X: %f\n", ref_pt[0]);  
        ECHO(" Reference Point Y: %f\n", ref_pt[1]);  
        if (prompt_for_two_numbers("Enter movement values", "DX", "DY",  
            &values[0], &values[1]))  
        {  
            ECHO(" Specified Values: dx:%f, dy:%f\n", values[0], values[1]);  
            ref_pt[0] = ref_pt[0] + values[0];  
            ref_pt[1] = ref_pt[1] + values[1];  
            ECHO(" New Reference Point X: %f\n", ref_pt[0]);  
            ECHO(" New Reference Point Y: %f\n", ref_pt[1]);  
            UF_CALL(UF_DRAW_set_drawing_ref_pt(NULL_TAG, view_tag, ref_pt));  
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

> 根据提供的NX二次开发代码，可以总结如下：
>
> 1. 函数和头文件：代码包含多个函数，使用UF、UF_DISP、UF_DRAW、UF_OBJ、UF_VIEW、UF_UI等头文件，这些都是NX Open提供的API，用于与NX进行交互。
> 2. 错误处理：代码定义了report_error函数，用于处理UF函数调用错误，并输出错误信息。同时，使用了宏UF_CALL来调用UF函数，并捕获可能的错误。
> 3. 视图选择：代码实现了select_a_view_by_mask函数，用于提示用户选择一个视图，并返回视图的tag。函数使用了UF_UI提供的对话框来实现视图选择。
> 4. 视图移动：代码实现了do_it函数，用于移动用户选择的视图。首先获取视图的参考点，然后提示用户输入移动值，并更新视图的参考点，从而实现视图的移动。
> 5. 主函数：代码定义了ufusr主函数，用于初始化NX Open，调用do_it函数执行视图移动操作，并在结束时关闭NX Open。
> 6. 卸载函数：代码实现了ufusr_ask_unload函数，返回立即卸载，表示该程序不需要保留在NX内存中。
>
> 总的来说，这段代码实现了在NX中移动视图的功能，通过提示用户选择视图并输入移动值，实现视图的平移。代码结构清晰，利用了NX Open提供的API进行二次开发。
>

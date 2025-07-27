### 【1330】get arc center of full bolt circle 获取完整螺栓圆弧的中心

#### 代码

```cpp
    /*HEAD GET_ARC_CENTER_OF_FULL_BOLT_CIRCLE CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static int mask_for_draft_aids(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_draft_aid(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_draft_aids, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                ECHO(msg);  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    ECHO(msg);  
                }  
            }  
            if (cols > 0) ECHO("\n");  
        }  
    }  
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        tag_t                       centerline;  
        UF_DRF_valid_cline_form_t   centerline_type;  
        double                      centerline_origin[3];  
        UF_DRF_centerline_info_p_t  centerline_info;  
        while ((centerline = select_a_draft_aid("Select centerline")) != NULL_TAG)  
        {  
            if (UF_CALL( UF_DRF_ask_centerline_info( centerline,  
                     &centerline_type, centerline_origin, &centerline_info)))  
                     continue;  
            WRITE_D(centerline_type);  
            WRITE3F(centerline_origin);  
            WRITE3F(centerline_info->arc_info->arc_center);  
        }  
    }  
    /* qq3123197280 */  
    extern void ufusr(char *param, int *retcode, int paramLen)  
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

> 这段代码是一个NX二次开发的UF函数，主要功能是从NX图形数据库中获取中心线的相关信息。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于处理UF函数调用错误。
> 2. 定义mask_for_draft_aids函数，用于设置选择过滤条件，只选择绘图辅助类型的实体。
> 3. 定义select_a_draft_aid函数，用于弹出选择对话框，让用户选择中心线。
> 4. 定义write_integer_to_listing_window和write_double_array_to_listing_window函数，用于将信息输出到日志窗口。
> 5. do_it函数是主函数，循环调用select_a_draft_aid获取用户选择的中心线，然后调用UF_DRF_ask_centerline_info获取中心线的类型、原点、圆心等信息，并输出。
> 6. ufusr函数是UF函数的入口，初始化和结束NX API，并调用do_it函数。
> 7. ufusr_ask_unload函数用于设置UF函数的卸载方式。
>
> 总体来说，这段代码实现了从NX图形数据库中获取中心线信息的功能，采用了面向对象的设计，具有较好的可读性和可维护性。
>

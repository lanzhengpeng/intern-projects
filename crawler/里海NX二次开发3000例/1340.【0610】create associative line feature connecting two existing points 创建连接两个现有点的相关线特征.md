### 【0610】create associative line feature connecting two existing points 创建连接两个现有点的相关线特征

#### 代码

```cpp
    /*HEAD CREATE_ASSOCIATIVE_LINE_FEATURE_CONNECTING_TWO_EXISTING_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
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
    /* 里海 */  
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
    static void do_it(void)  
    {  
        tag_t  
            line_feature_id,  
            point1,  
            point2;  
        while (((point1 = select_a_point("Start point")) != NULL_TAG) &&  
               ((point2 = select_a_point("End point")) != NULL_TAG))  
        {  
            UF_CALL(UF_CURVE_create_line_point_point(point1, point2,  
                NULL, NULL_TAG, TRUE, &line_feature_id));  
        /*  This should not be required - see PR 6154789 译:这不应该被要求 - 见 PR 6154789 */  
            UF_CALL(UF_DISP_regenerate_display());  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 包含了必要的NX Open头文件，用于调用NX的API。
> 2. 定义了宏ECHO和UF_CALL，分别用于在列表窗口打印信息和调用NX API，并报告错误。
> 3. 定义了report_error函数，用于在API调用出错时报告错误信息。
> 4. 定义了mask_for_points函数，用于设置选择点时的过滤条件。
> 5. 定义了select_a_point函数，用于提示用户选择一个点，并返回点的标签。
> 6. 定义了do_it函数，用于循环提示用户选择两个点，并调用NX API创建连接这两个点的直线特征。
> 7. 定义了ufusr函数，用于初始化NX，调用do_it函数，然后终止NX。
> 8. 定义了ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 总的来说，这段代码通过交互式选择两个点，在NX中创建了一条连接这两个点的直线特征。
>

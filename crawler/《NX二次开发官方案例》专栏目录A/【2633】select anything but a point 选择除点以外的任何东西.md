### 【2633】select anything but a point 选择除点以外的任何东西

#### 代码

```cpp
    /*HEAD SELECT_ANYTHING_BUT_A_POINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
            UF_UI_write_listing_window(X); \  
            UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc) {  
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
    static int mask_for_no_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ALL_AND_DISABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_non_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_no_points, NULL, &resp,  
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
            item;  
        char  
            msg[133];  
        while ((item = select_a_non_point("Select something")) != NULL_TAG)  
        {  
            sprintf(msg, "Item Tag: %d", item);  
            ECHO(msg);  
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

> 这段NX二次开发代码的主要功能是让用户选择除点以外的任何NX对象，并在选择后打印出对象的标签。
>
> 代码的主要步骤如下：
>
> 1. 包含必要的NX开发库头文件。
> 2. 定义错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义一个选择过滤函数mask_for_no_points，该函数在UF_UI_set_sel_mask中设置选择屏蔽，屏蔽点类型，允许选择除点以外的任何对象。
> 4. 定义选择函数select_a_non_point，使用UF_UI_select_with_single_dialog进行选择，并调用mask_for_no_points作为选择过滤。选择成功后，打印对象标签并高亮显示。
> 5. 主函数do_it循环调用select_a_non_point，直到用户取消选择为止。
> 6. 定义ufusr函数作为NX的入口函数，在NX启动时调用初始化、执行主函数、终止。
> 7. 定义ufusr_ask_unload函数，用于卸载时立即终止。
>
> 综上所述，这段代码实现了在NX中让用户选择除点以外的任何对象，并在选择后打印对象标签的功能。通过自定义选择过滤函数，实现了选择范围的控制。
>

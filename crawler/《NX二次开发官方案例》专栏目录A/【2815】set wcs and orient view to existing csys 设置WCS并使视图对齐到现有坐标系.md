### 【2815】set wcs and orient view to existing csys 设置WCS并使视图对齐到现有坐标系

#### 代码

```cpp
    /*HEAD SET_WCS_AND_ORIENT_VIEW_TO_EXISTING_CSYS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_csys.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本中新增的功能，专门用于打印系统日志。 */  
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
    /* qq3123197280 */  
    static int mask_for_csys(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_coordinate_system_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_csys(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "Orient WCS",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
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
            csys;  
        if ((csys = select_a_csys("Select CSYS")) != NULL_TAG)  
        {  
            UF_CALL(UF_CSYS_set_wcs(csys));  
            UF_CALL(uc6434("", 2, NULL_TAG, NULL));  
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

> 这段NX二次开发代码的主要功能是设置工作坐标系(WCS)并定向视图到已存在的坐标系。以下是代码的主要功能和流程：
>
> 1. 包含必要的NX Open API头文件，用于进行NX二次开发。
> 2. 定义了一个错误报告函数report_error，用于在API调用出错时报告错误信息。
> 3. 定义了一个用于选择坐标系的函数mask_for_csys，它设置了选择过滤条件，只允许选择坐标系类型。
> 4. 定义了一个选择坐标系的函数select_a_csys，它使用对话框让用户选择一个坐标系。
> 5. 定义了主要功能函数do_it，首先调用select_a_csys选择坐标系，然后调用UF_CSYS_set_wcs设置WCS，最后调用uc6434函数定向视图到所选坐标系。
> 6. 定义了ufusr函数，它是NX二次开发程序的入口函数。在该函数中，首先调用UF_initialize初始化环境，然后调用do_it执行主要功能，最后调用UF_terminate终止环境。
> 7. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示程序可以立即卸载。
>
> 总的来说，这段代码通过选择一个坐标系，然后设置WCS和视图方向到该坐标系，实现了坐标系和视图的快速定位功能。
>

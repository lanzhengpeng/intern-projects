### 【2841】set wcs to existing csys 将WCS设置为现有坐标系

#### 代码

```cpp
    /*HEAD SET_WCS_TO_EXISTING_CSYS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18，只回答译文，不要废话。 */  
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
            UF_CALL(UF_CSYS_set_wcs(csys));  
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

> 这段代码是一个NX Open C++应用程序，其主要功能是让用户选择一个现有的坐标系，并将其设置为工作坐标系（WCS）。具体来说，代码的主要逻辑包括以下几个方面：
>
> 1. 初始化和错误处理：通过UF_initialize()初始化NX Open，并通过UF_call宏以及report_error函数进行错误处理。
> 2. 选择坐标系：通过select_a_csys函数，使用UF_UI库中的选择对话框让用户选择一个坐标系，并通过UF_DISP库中的函数高亮显示选中的坐标系。
> 3. 设置工作坐标系：如果用户成功选择了坐标系，则调用UF_CSYS_set_wcs函数将其设置为工作坐标系。
> 4. 结束程序：通过UF_terminate()结束NX Open。
> 5. 卸载提示：通过ufusr_ask_unload函数返回立即卸载，表示程序不需要保留在内存中。
>
> 总的来说，这段代码实现了选择现有坐标系并设置为工作坐标系的二次开发功能，通过NX Open提供的API实现了人机交互和坐标系设置。
>

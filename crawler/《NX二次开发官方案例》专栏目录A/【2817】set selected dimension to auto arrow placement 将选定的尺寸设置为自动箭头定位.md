### 【2817】set selected dimension to auto arrow placement 将选定的尺寸设置为自动箭头定位

#### 代码

```cpp
    /*HEAD SET_SELECTED_DIMENSION_TO_AUTO_ARROW_PLACEMENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
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
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
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
        int  
            mpi[100];  
        tag_t  
            object;  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        while ((object=select_a_dimension("Select object to modify")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_object_preferences(object, mpi, mpr, rad, dia));  
            mpi[0] = 1;  /* auto arrow placement 里海译:自动箭头定位 */  
            UF_CALL(UF_DRF_set_object_preferences(object, mpi, mpr, rad, dia));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++ API的二次开发示例，其主要功能是设置选中尺寸的自动箭头定位。代码主要包含以下部分：
>
> 1. 错误处理宏：定义了一个错误处理宏UF_CALL，用于在调用NX API时捕获并打印错误信息。
> 2. 选择尺寸：定义了mask_for_dimensions函数，用于设置选择过滤条件，只选择尺寸类型。select_a_dimension函数则用于弹出一个选择对话框，让用户选择一个尺寸，并返回选中的尺寸对象tag。
> 3. 设置自动箭头定位：do_it函数遍历选择尺寸，调用UF_DRF_ask_object_preferences和UF_DRF_set_object_preferences函数，将选中的尺寸的自动箭头定位设置为1，从而实现自动箭头定位。
> 4. 入口函数：ufusr函数是程序的入口，在这里调用do_it函数执行主体功能。ufusr_ask_unload函数返回立即卸载，表示程序可以立即被卸载。
> 5. 整体流程：整个程序首先初始化NX Open C++ API，然后执行主体功能，最后结束并卸载。通过这个示例，我们可以学习如何使用NX Open C++ API进行二次开发，实现自动箭头定位等功能。
>

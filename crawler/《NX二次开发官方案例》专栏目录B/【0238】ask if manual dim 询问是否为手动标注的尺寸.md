### 【0238】ask if manual dim 询问是否为手动标注的尺寸

#### 代码

```cpp
    /*HEAD ASK_IF_MANUAL_DIM CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define MANUAL_TEXT            3  
    #define MANUAL_TEXT_AND_APP    4  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的内容，翻译如下：

UF_print_syslog 是 V18 中新增的函数。 */  
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
            mpi_array[100];  
        double  
            mpr_array[70];  
        char  
            rad_val[27],  
            dia_val[27];  
        tag_t  
            dim_tag;  
        while((dim_tag = select_a_dimension("select dimension")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_object_preferences( dim_tag, mpi_array,  
                                           mpr_array, rad_val, dia_val));  
            if(mpi_array[7] == MANUAL_TEXT || mpi_array[7] == MANUAL_TEXT_AND_APP)  
                uc1601("Dimension has manual text", 1);  
            else  
                uc1601("NOT!!", 1);  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 包含了必要的NX二次开发头文件，如uf.h、uf_drf.h等。
> 2. 定义了宏MANUAL_TEXT和MANUAL_TEXT_AND_APP，表示手动文本和手动文本加公差。
> 3. 定义了UF_CALL宏，用于调用NX API函数并处理错误。
> 4. 定义了report_error函数，用于在出错时输出错误信息。
> 5. 定义了mask_for_dimensions函数，用于设置选择过滤条件，只选择尺寸类型。
> 6. 定义了select_a_dimension函数，用于弹出一个对话框，让用户选择一个尺寸。
> 7. 定义了do_it函数，循环调用select_a_dimension选择尺寸，并判断尺寸是否为手动文本。
> 8. 定义了ufusr函数，这是NX二次开发的入口函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 9. 定义了ufusr_ask_unload函数，用于在卸载用户函数时立即卸载。
> 10. 整体上，这段代码实现了选择尺寸并判断是否为手动文本的功能，具有典型的NX二次开发结构。
>

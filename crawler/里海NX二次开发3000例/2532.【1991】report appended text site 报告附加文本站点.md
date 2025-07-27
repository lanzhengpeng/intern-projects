### 【1991】report appended text site 报告附加文本站点

#### 代码

```cpp
    /*HEAD REPORT_APPENDED_TEXT_SITE CCC UFUN*/  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我理解到UF_print_syslog是V18版本中新增的一个函数。根据文档的要求，我仅提供翻译，不添加任何其他内容。

UF_print_syslog在V18版本中是新增的。 */  
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
    static int mask_for_dim_or_draft(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_drafting_entity_type, UF_all_subtype, 0 },  
                        { UF_dimension_type, UF_all_subtype, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dim_or_draft(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dim_or_draft, NULL, &resp,  
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
            dia[27],  
            msg[300] = {""};  
        while ((object = select_a_dim_or_draft("Select object")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_object_preferences(object, mpi, mpr, rad, dia));  
            UF_CALL(UF_UI_open_listing_window());  
            sprintf(msg, "appended text site MPI8: %d\n", mpi[8]);  
            UF_CALL(UF_UI_write_listing_window (msg));  
            sprintf(msg, "---------------------------------------------\n");  
            UF_CALL(UF_UI_write_listing_window (msg));  
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

> 这段代码是NX的二次开发代码，主要功能是选择图纸中的尺寸标注或草图对象，并获取它们的显示参数。以下是代码的主要功能：
>
> 1. 错误处理宏：定义了UF_CALL宏，用于调用NX API函数，并在出错时报告错误信息。
> 2. 选择过滤函数：定义了mask_for_dim_or_draft函数，用于过滤选择对话框，仅显示尺寸标注或草图对象。
> 3. 选择对象函数：定义了select_a_dim_or_draft函数，用于弹出选择对话框，让用户选择尺寸标注或草图对象，并返回所选对象的tag。
> 4. 获取显示参数并显示：在do_it函数中，循环调用select_a_dim_or_draft获取用户选择的每个对象，然后调用UF_DRF_ask_object_preferences函数获取对象的显示参数，并将参数MPI8输出到列表窗口。
> 5. 主函数：定义了ufusr函数，用于初始化和终止NX环境，并调用do_it函数实现主要功能。
> 6. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载标志，表示该二次开发程序可以立即被卸载。
>
> 总体而言，这段代码实现了选择图纸中的尺寸标注或草图对象，获取它们的显示参数，并将参数输出到列表窗口的功能。
>

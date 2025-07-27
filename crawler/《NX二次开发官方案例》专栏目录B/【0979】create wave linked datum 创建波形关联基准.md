### 【0979】create wave linked datum 创建波形关联基准

#### 代码

```cpp
    /*HEAD CREATE_WAVE_LINKED_DATUM CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_wave.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
注意：UF_print_syslog 是 V18 中新增的，请只回答翻译，不要添加任何评论。 */  
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
    static int mask_for_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_datum_plane_type, 0, 0 },  
                        { UF_datum_axis_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datums, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void create_wave_linked_datum(tag_t datum)  
    {  
        tag_t  
            feat,  
            proto,  
            xform;  
        if (UF_ASSEM_is_occurrence(datum))  
        {  
            proto = UF_ASSEM_ask_prototype_of_occ(datum);  
            UF_CALL(UF_SO_create_xform_assy_ctxt(datum,  
                UF_ASSEM_ask_part_occurrence(datum), NULL_TAG, &xform));  
            UF_CALL(UF_WAVE_create_linked_datum(proto, xform, datum, &feat));  
        }  
        else  
            UF_CALL(UF_WAVE_create_linked_datum(datum, NULL_TAG, datum, &feat));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            datum;  
        while ((datum = select_a_datum("Select datum to link")) != NULL_TAG)  
            create_wave_linked_datum(datum);  
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

> 这段代码是一个NX Open C++的UF程序，主要用于创建链接到装配体的基准。代码的主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于输出UF函数调用的错误信息。
> 2. 定义了一个函数mask_for_datums，用于设置选择过滤器，只允许选择基准面和基准轴。
> 3. 定义了一个函数select_a_datum，用于让用户选择一个基准，并返回基准的tag。
> 4. 定义了一个函数create_wave_linked_datum，用于创建一个链接基准特征，该基准特征链接到用户选择的基准。
> 5. 主函数do_it中，循环调用select_a_datum获取用户选择的基准，然后调用create_wave_linked_datum创建链接基准特征。
> 6. ufusr是程序的入口函数，初始化后调用do_it，最后终止。
> 7. ufusr_ask_unload函数用于卸载程序，返回立即卸载。
>
> 总的来说，这段代码实现了选择装配体中的基准，并创建一个链接到该基准的特征的功能。
>

### 【1242】extract bsurf and report area 提取B曲面并报告面积

#### 代码

```cpp
    /*HEAD EXTRACT_BSURF_AND_REPORT_AREA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的信息，UF_print_syslog是V18版本中新增的函数。因此，翻译为：

翻译：UF_print_syslog是在V18版本中新增的函数。 */  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face = NULL_TAG,  
            sheet_body = NULL_TAG;      
        char  
            msg[132] = { "" };  
        double  
            /* this will run VERY slowly with an acc_val[0] of 1.0 里海译:这句话的翻译是：当 acc_val[0] 为 1.0 时，这将运行得非常慢。 */  
            acc_val[11] = {.99,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.},  
            statistics[13],  
            mass_props[47];  
        UF_CALL( UF_UI_open_listing_window());  
        while( ( face = select_a_face( "Pick a FACE:" )) != NULL_TAG )  
        {  
            UF_CALL( UF_MODL_extract_face( face, 2, /* make a b-surface 里海译:生成B曲面 */  
                                           &sheet_body ));  
            UF_CALL( UF_MODL_ask_mass_props_3d ( &sheet_body,   
                                    1, /* Number of objects 里海译:Number of objects的翻译为：对象数量。 */  
                                    2, /* Thin Shell - Sheet Bodies 里海译:薄壳 - 板体 */  
                                    4, /* kilograms & meters 里海译:千克和米 */  
                                    1.0, /*  Mass per unit area for thin shell 里海译:薄壳单位面积的质量 */  
                                    1, /* Use Accuracy 里海译:Accuracy的中文意思是“准确率”。 */  
                                    acc_val, /* Accuracy values 里海译:Accuracy values的译文是“准确度值”。 */  
                                    mass_props, /*  Mass properties 里海译:质量属性 */  
                                    statistics 里海译:统计 )); /* statistics */  
            sprintf( msg, "Area of extracted bsurf: %f sqare meters\n",   
                                                          mass_props[0] );  
            UF_CALL( UF_UI_write_listing_window( msg ));  
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

> 这段代码实现了以下功能：
>
> 1. 提取选择的面的B曲面，并计算B曲面的面积。
> 2. 用户可以反复选择一个面，直到输入NULL_TAG结束。
> 3. 提取B曲面时，指定精度参数，其中acc_val[0]为0.99表示使用高精度。
> 4. 使用UF_MODL_ask_mass_props_3d计算B曲面的质量属性，其中mass_props[0]返回的是面积。
> 5. 将B曲面的面积输出到日志窗口。
> 6. 使用UF_UI_set_sel_mask设置了选择面时的选择掩码。
> 7. 使用UF_DISP_set_highlight高亮显示选择的面对象。
> 8. 使用UF_UI_open_listing_window打开日志窗口。
> 9. 使用UF_UI_write_listing_window在日志窗口输出信息。
> 10. 使用UF_print_syslog输出错误信息。
> 11. 使用UF_call宏来捕获UF函数调用时的错误码，并输出错误信息。
> 12. 定义了UF_UNLOAD_IMMEDIATELY表示卸载函数立即返回。
> 13. 整体实现了选择面、提取B曲面、计算面积并显示的完整流程。
>

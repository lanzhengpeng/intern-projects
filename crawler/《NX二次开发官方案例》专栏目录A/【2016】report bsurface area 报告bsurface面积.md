### 【2016】report bsurface area 报告bsurface面积

#### 代码

```cpp
    /*HEAD REPORT_BSURFACE_AREA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的。 */  
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
    static int mask_for_bsurfs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PARAMETRIC_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_bsurf(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a bsurf", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bsurfs, NULL, &resp,  
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
            bsurf = NULL_TAG,  
            solid = NULL_TAG;  
        char  
            msg[132] = { "" };  
        double  
            /* this will run VERY slowly with an acc_val[0] of 1.0 里海译:这行代码中的acc_val[0]的值为1.0时，将会运行得非常慢。 */  
            acc_val[11] = {.99,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.},  
            statistics[13],  
            mass_props[47];  
        UF_CALL( UF_UI_open_listing_window());  
        while( ( bsurf = select_a_bsurf( "Pick a bsurf:" )) != NULL_TAG )  
        {   
            UF_CALL( UF_MODL_ask_face_body( bsurf, &solid ));  
            UF_CALL( UF_MODL_ask_mass_props_3d ( &solid,   
                                    1, /* Number of objects 里海译:Number of objects 的翻译是：对象数量。 */  
                                    2, /* Thin Shell - Sheet Bodies 里海译:薄壳 - 板体 */  
                                    1, /* pounds & inches 里海译:磅和英寸 */  
                                    1.0, /*  Mass per unit area for thin shell 里海译:单位面积质量(薄壳) */  
                                    1, /* Use Accuracy 里海译:Use Accuracy的中文翻译是使用准确度。 */  
                                    acc_val, /* Accuracy values 里海译:Accuracy values 的中文翻译为“准确率”。 */  
                                    mass_props, /*  Mass properties 里海译:质量特性 */  
                                    statistics 里海译:统计数据 )); /* statistics */  
            sprintf( msg, "Area of bsurf: %f\n", mass_props[0] );  
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

> 这段代码是用于在NX中测量选择曲面的面积。以下是代码的主要功能和步骤：
>
> 1. 头文件和错误报告函数：包含所需的NX Open头文件，并定义了一个错误报告函数report_error，用于在发生错误时输出错误信息和调用信息。
> 2. 选择曲面掩码：定义了一个选择曲面掩码mask_for_bsurfs，仅允许用户选择参数化曲面。
> 3. 选择曲面：定义了一个select_a_bsurf函数，使用对话框提示用户选择一个曲面，并返回选择的曲面标签。
> 4. 测量面积：在do_it函数中，使用一个循环让用户选择曲面，然后调用UF_MODL_ask_mass_props_3d函数计算所选曲面的质量属性，其中包含了面积信息。最后，将面积信息格式化输出到列表窗口。
> 5. 主函数：ufusr函数初始化NX Open，调用do_it函数，然后终止NX Open。
> 6. 卸载函数：定义了一个卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总体来说，这段代码实现了在NX中选择参数曲面并测量其面积的功能。
>

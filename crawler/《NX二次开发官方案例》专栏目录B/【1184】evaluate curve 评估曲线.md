### 【1184】evaluate curve 评估曲线

#### 代码

```cpp
    /*HEAD EVALUATE_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。文档要求只提供译文，不要添加任何其他内容。因此，我提供的翻译如下：

UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
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
            deriv_flag = UF_MODL_LOC,  
            ii;  
        tag_t  
            crv_tag,  
            pt_tag;  
        double  
            parm = 0.0,  
            pos_deriv[6];  
        crv_tag = select_a_curve("select curve to evaluate");  
        for(ii =0 ; ii < 11 ; ii ++)  
        {  
            UF_CALL(UF_MODL_evaluate_curve(crv_tag, &parm ,  
                                     &deriv_flag, pos_deriv));  
            UF_CALL(UF_CURVE_create_point(pos_deriv, &pt_tag));  
            parm += 0.1;  
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

> 这段代码是一个NX的二次开发示例，其主要功能是选择一个曲线，然后在曲线的参数空间上均匀地取11个点，在每个点处创建一个点对象。以下是代码的主要组成部分：
>
> 1. 头文件包含：引入了NX的UI、模型、曲线等相关的头文件。
> 2. 宏定义：ECHO和UF_CALL用于打印变量值和封装UF函数调用。
> 3. 错误报告函数：report_error用于在UF函数调用出错时打印错误信息。
> 4. 曲线选择屏蔽函数：mask_for_curves用于设置曲线选择对话框的屏蔽。
> 5. 选择曲线函数：select_a_curve用于弹出对话框选择一个曲线，并返回其tag。
> 6. 主函数：do_it函数选择一个曲线，然后在参数空间上均匀取11个点，在每个点处创建一个点对象。
> 7. UF初始化和终止：ufusr函数在NX启动时调用，用于初始化和终止NX环境。
> 8. 卸载询问函数：ufusr_ask_unload用于在卸载时询问是否立即卸载。
>
> 总体来说，这段代码通过二次开发的方式，实现了在NX中交互式选择曲线并创建均匀分布的点对象的功能。
>

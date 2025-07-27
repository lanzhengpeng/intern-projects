### 【0840】create points along curve 在曲线上创建点

#### 代码

```cpp
    /*HEAD create_points_along_a_curve CCC UFUN */  
    /*  
    This program creates points along a curve either   
    by distance or chordal tolerance and prints the   
    point coordinate WCS dependent to info window  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_csys.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                sprintf(err, "error %d at line %d in %s\n", irc, line, file);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n\n");  
            }  
            else  
            {  
                fprintf(stderr, "%s\n", err);  
                fprintf(stderr, "error %d at line %d in %s\n", irc, line, file);  
                fprintf(stderr, "%s;\n\n", call);  
    #ifdef _USRDLL  
                uc1601(err, TRUE);  
                sprintf(err, "error %d at line %d in %s\n", irc, line, file);  
                uc1601(err, TRUE);  
    #endif  
            }  
        }  
        return(irc);  
    }  
    static void print_message(char *text)  
    {  
        if (!UF_UI_open_listing_window())  
            UF_UI_write_listing_window(text);  
    }  
    /* 里海 */  
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
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
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
        tag_t point, curve, part;  
        char text[133];  
        char prompt[][16] = { "Chord", "Angle", "Point distance" };  
        int junk, resp, np;  
        int j, k;  
        int input_csys = UF_CSYS_ROOT_COORDS;  
        int output_csys = UF_CSYS_ROOT_WCS_COORDS;  
        double tols[] = { 0.1, 0.5, 1.0 };  
        double *pts;  
        double input_point[3], output_point[3];  
        if( (part=UF_ASSEM_ask_work_part()) == NULL_TAG )  
        {  
            print_message("No work part. Exiting...\n");  
            return;  
        }  
        // Ask for parameters  
        resp = uc1609("Parameter for point creation", prompt, 3, tols, &junk);  
        if (resp != 3 && resp != 4) return;  
        while( curve = select_a_curve("Select a curve") )  
        {  
            UF_CALL(UF_MODL_ask_curve_points(curve, tols[0], tols[1], tols[2],&np,&pts));  
            sprintf(text, "\nCount of Points:%d\n", np);  
            print_message(text);  
            print_message("\nWCS coordinates of Points:\n");  
            for (j=0; j<np; j++)   
            {  
                k = j * 3; /* 3 Koordinaten 译:3个坐标 */  
                UF_CALL(UF_CURVE_create_point(&pts[k], &point));  
                input_point[0] = pts[k];  
                input_point[1] = pts[k+1];  
                input_point[2] = pts[k+2];  
                UF_CSYS_map_point( input_csys, input_point, output_csys, output_point);  
                sprintf(text, "  %+09.3f %+09.3f %+09.3f\n", output_point[0], output_point[1], output_point[2]);  
                print_message(text);  
            } // for  
            UF_free(pts);  
        } // while  
    } // do_it  
    /* 里海 */  
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

> 这段NX二次开发代码的主要功能是沿着曲线生成点，并根据距离或角度公差设置点之间的间距，最后在信息窗口中输出点的WCS坐标。代码的主要结构如下：
>
> 1. 包含必要的头文件，如uf.h、uf_ui.h等，用于调用NX的API函数。
> 2. 定义了一个错误报告函数report_error，用于在发生错误时打印错误信息。
> 3. 定义了一个打印信息函数print_message，用于在信息窗口中输出文本信息。
> 4. 定义了一个曲线选择掩码函数mask_for_curves，用于在曲线选择对话框中设置可选择的曲线类型。
> 5. 定义了一个选择曲线函数select_a_curve，用于让用户选择一个曲线实体，并返回其tag。
> 6. 定义了一个主函数do_it，用于实现创建点的主要逻辑：
>
> 1. 定义了一个ufusr函数，作为程序的入口点，用于初始化NX环境，调用主函数，然后终止NX环境。
> 2. 定义了一个卸载函数ufusr_ask_unload，用于设置立即卸载二次开发程序。
>
> 总的来说，这段代码实现了选择曲线、根据参数生成点、转换坐标系并打印坐标的功能，为NX的曲线处理提供了便利。
>

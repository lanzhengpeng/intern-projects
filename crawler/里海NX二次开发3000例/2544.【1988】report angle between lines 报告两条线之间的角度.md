### 【1988】report angle between lines 报告两条线之间的角度

#### 代码

```cpp
    /*HEAD REPORT_ANGLE_BETWEEN_LINES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
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

注意：UF_print_syslog是V18版本新增的功能，请只回答翻译，不要添加其他内容。 */  
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
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
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
            dim,  
            ii;  
        tag_t  
            lines[2];  
        double  
            angle,  
            ccw[3],  
            data[2][6];  
        while (((lines[0] = select_a_line("Select first line")) != NULL_TAG) &&  
               ((lines[1] = select_a_line("Select second line")) != NULL_TAG))  
        {  
            for (ii = 0; ii < 2; ii++)  
              UF_CALL(UF_MODL_ask_obj_dimensionality(lines[ii],&dim,&data[ii][0]));  
            UF_VEC3_cross(&data[0][3], &data[1][3], ccw);  
            UF_VEC3_angle_between(&data[0][3], &data[1][3], ccw, &angle);  
            printf("The angle between the lines is %f\n", angle * RADEG);  
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

> 这是段NX二次开发代码，其主要功能是计算两条线之间的夹角。代码流程如下：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息。
> 2. 定义了一个选择线的函数select_a_line，使用NX的UI选择功能，让用户选择两条线。
> 3. 定义了主要执行函数do_it，在该函数中：调用select_a_line选择两条线，得到两条线的tag。获取两条线的方向向量，保存在data数组中。使用UF_VEC3_cross函数计算两条线向量的叉乘，得到垂直于两条线的向量ccw。使用UF_VEC3_angle_between函数计算两条线之间的夹角。输出两条线之间的夹角。
> 4. 调用select_a_line选择两条线，得到两条线的tag。
> 5. 获取两条线的方向向量，保存在data数组中。
> 6. 使用UF_VEC3_cross函数计算两条线向量的叉乘，得到垂直于两条线的向量ccw。
> 7. 使用UF_VEC3_angle_between函数计算两条线之间的夹角。
> 8. 输出两条线之间的夹角。
> 9. 在ufusr函数中，初始化NX环境，调用do_it函数执行，然后终止NX环境。
> 10. 定义了一个卸载函数ufusr_ask_unload，返回立即卸载。
>
> 该代码的主要功能是让用户选择两条线，然后计算并输出这两条线之间的夹角。代码结构清晰，通过封装错误报告、选择线和计算夹角等函数，使主流程简洁易懂。
>

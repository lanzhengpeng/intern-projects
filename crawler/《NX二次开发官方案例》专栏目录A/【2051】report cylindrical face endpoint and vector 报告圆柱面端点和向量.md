### 【2051】report cylindrical face endpoint and vector 报告圆柱面端点和向量

#### 代码

```cpp
    /*HEAD REPORT_CYLINDRICAL_FACE_ENDPOINT_AND_VECTOR CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新增的功能。

只回答译文，不要废话。 */  
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
    static int mask_for_cylindrical_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CYLINDRICAL_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_cylindrical_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select cylindrical face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_cylindrical_faces, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            cyl_face;  
        double  
            endpoint[3],  
            vector[3];  
        while ((cyl_face = select_a_cylindrical_face("Report endpoint and vector"))  
            != NULL_TAG)  
        {  
            FTN(uf5303)(&cyl_face, endpoint, vector);  
            display_conehead(endpoint, vector);  
            WRITE3F(endpoint);  
            WRITE3F(vector);  
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

> 这是段NX Open C++ API的二次开发代码，主要功能是选择圆柱面并报告其端点和向量。以下是该代码的主要内容和功能：
>
> 1. 包含必要的头文件，如UF、UF_UI等。
> 2. 定义了宏UF_CALL，用于调用UF函数并检查返回码，如果出错则报告错误信息。
> 3. 定义了函数report_error，用于输出错误信息。
> 4. 定义了函数mask_for_cylindrical_faces，用于设置选择掩码，仅选择圆柱面特征。
> 5. 定义了函数select_a_cylindrical_face，用于弹出选择对话框，让用户选择圆柱面，并返回其tag。
> 6. 定义了宏WRITE3F，用于输出3个双精度浮点数到列表窗口。
> 7. 定义了函数write_double_array_to_listing_window，用于输出double数组到列表窗口。
> 8. 定义了函数display_conehead，用于在视图中显示圆锥头。
> 9. 定义了函数do_it，循环选择圆柱面，并报告其端点和向量，同时显示圆锥头。
> 10. 定义了ufusr函数，这是NX二次开发的入口函数，调用do_it函数。
> 11. 定义了ufusr_ask_unload函数，用于卸载二次开发应用程序。
>
> 综上所述，这段代码实现了选择圆柱面并报告其端点和向量的功能，并具有友好的用户交互界面。
>

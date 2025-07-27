### 【2464】reverse selected datum axis direction 反转选定的基准轴方向

#### 代码

```cpp
    /*HEAD REVERSE_SELECTED_DATUM_AXIS_DIRECTION CCC UFUN */  
    /*  
        Note the function UF_MODL_reverse_datum_axis does not seem to work  
        on ABSOLUTE_DATUM_AXIS feature types.  See PR 4598311.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数，用于打印系统日志。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
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
    /* qq3123197280 */  
    static int mask_for_datum_axes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_axis_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum_axis(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum axis", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_axes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_feature_type(tag_t feature)  
    {  
        char  
            *type;  
        WRITE_D(feature);  
        UF_CALL(UF_MODL_ask_feat_type(feature, &type));  
        WRITE_S(type);  
        UF_free(type);  
    }  
    static void report_datum_axis_parms(tag_t daxis)  
    {  
        tag_t  
            feat;  
        double  
            origin[3],  
            normal[3];  
        UF_CALL(UF_MODL_ask_object_feat(daxis, &feat));  
        UF_CALL(UF_MODL_ask_datum_axis_parms(feat, origin, normal));  
        WRITE3F(origin);  
        WRITE3F(normal);  
    }  
    static void do_it(void)  
    {  
        logical  
            reversed;  
        int  
            irc,  
            one = 1,  
            two = 2,  
            zero = 0;  
        tag_t  
            datum_axis,  
            datum_feat;  
        double  
            one_eighty = 180,  
            origin[3],  
            normal[3],  
            rotation_matrix[16],  
            rotation_vector[3];  
        char  
            *type;  
        while ((datum_axis = select_a_datum_axis("Reverse")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_object_feat(datum_axis, &datum_feat));  
            report_feature_type(datum_feat);  
            UF_CALL(UF_MODL_is_datum_axis_reversed(datum_feat, &reversed));  
            WRITE_L(reversed);  
            report_datum_axis_parms(datum_feat);  
            UF_CALL(UF_MODL_reverse_datum_axis(datum_feat));  
            UF_CALL(UF_MODL_update());  
            WRITE("After UF_MODL_reverse_datum_axis:\n");  
            UF_CALL(UF_MODL_is_datum_axis_reversed(datum_feat, &reversed));  
            WRITE_L(reversed);  
            report_datum_axis_parms(datum_feat);  
        /*  This is a work around for PR 4598311 里海译:翻译：这是针对PR 4598311的变通方法。 */  
            UF_CALL(UF_MODL_ask_feat_type(datum_feat, &type));  
            if (!strcmp(type, "ABSOLUTE_DATUM_AXIS"))  
            {  
                UF_CALL(UF_MODL_ask_datum_axis_parms(datum_feat, origin, normal));  
                UF_VEC3_ask_perpendicular(normal, rotation_vector);  
                FTN(uf5945)(origin, rotation_vector, &one_eighty, rotation_matrix,  
                    &irc);  
                FTN(uf5947)(rotation_matrix, &datum_axis, &one, &one, &zero, &two,  
                    NULL, NULL, &irc);  
                WRITE("After work around for PR 4598311:\n");  
                UF_CALL(UF_MODL_is_datum_axis_reversed(datum_feat, &reversed));  
                WRITE_L(reversed);  
                report_datum_axis_parms(datum_feat);  
            }  
            UF_free(type);  
            WRITE("\n");  
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

> 这段代码是一个NX Open API的UF程序，其主要功能是反转选取的基准轴。以下是代码的主要功能介绍：
>
> 1. 选择基准轴：使用UF_UI函数，通过对话框让用户选择一个基准轴，并返回基准轴的tag。
> 2. 报告基准轴参数：通过UF_MODL函数查询所选基准轴的类型、是否反转以及原点方向向量等参数，并打印到列表窗口。
> 3. 反转基准轴：使用UF_MODL_reverse_datum_axis函数反转所选基准轴，然后更新模型。
> 4. 再次报告基准轴参数：反转后，再次查询并打印基准轴参数到列表窗口。
> 5. 处理绝对基准轴：针对PR 4598311中的问题，如果基准轴是绝对基准轴，则使用矩阵变换的方式实现反转。
> 6. 错误处理：使用UF_CALL宏封装UF函数调用，并在出错时打印错误信息。
> 7. 辅助函数：定义了WRITE系列宏，用于将信息打印到列表窗口。
> 8. 初始化和卸载：在ufusr函数中初始化和终止UF系统，在ufusr_ask_unload函数中设置立即卸载。
>
> 总的来说，这段代码实现了基准轴反转的功能，并处理了一些特殊情况，如绝对基准轴的反转。代码结构清晰，注释详细，便于理解和使用。
>

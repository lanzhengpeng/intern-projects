### 【0239】ask if selected face is misaligned 询问选定面是否错位

#### 代码

```cpp
    /*HEAD ASK_IF_SELECTED_FACE_IS_MISALIGNED CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
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
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
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
    static void report_csys_info(tag_t csys)  
    {  
        tag_t  
            matrix;  
        double  
            origin[3],  
            vectors[9];  
        if (UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin))) return;  
        WRITE3F(origin);  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, vectors));  
        WRITE9F(vectors);  
    }  
    /* 里海 */  
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
            csys = NULL_TAG,  
            matrix = NULL_TAG;  
        double   
            matrix_values[ 9 ],  
            tolerance = 0.1;  
        logical   
            is_misaligned;  
        char   
            msg[50],  
            *aligned_or_not[2] = {"not misaligned", "misaligned"};  
        UF_CALL( UF_CSYS_ask_wcs( &csys ) );  
        UF_CALL( UF_CSYS_ask_matrix_of_object( csys, &matrix ) );  
        UF_CALL( UF_CSYS_ask_matrix_values (matrix, matrix_values) );  
        while( face = select_a_face("Select a face") )  
        {  
            UF_CALL( UF_MODL_ask_misalign_geometry (   
                    face, matrix_values, tolerance, &is_misaligned ) );  
            sprintf( msg, "Face %d is %s", face, aligned_or_not[is_misaligned]);  
            uc1601(msg, 1);  
        }  
    }  
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

> 这段代码是一个NX Open的二次开发示例，主要功能是检查选择的模型面是否与当前坐标系对齐。
>
> 主要步骤包括：
>
> 1. 初始化NX Open，并设置选择模式为选择模型面。
> 2. 获取当前坐标系的信息，包括原点和矩阵。
> 3. 循环让用户选择模型面。
> 4. 对于每个选择的模型面，调用UF_MODL_ask_misalign_geometry函数检查面是否与坐标系对齐，如果偏差大于容差，则认为面是错位的。
> 5. 输出检查结果。
> 6. 重复步骤3-5，直到用户不选择模型面为止。
> 7. 结束NX Open初始化。
>
> 代码中还包含了一些辅助函数，用于设置选择模式、获取坐标系信息、输出坐标系信息和错误处理。
>
> 总的来说，这段代码提供了一个简单的示例，演示了如何使用NX Open的API来检查模型面是否与坐标系对齐。
>

### 【2329】report selected face data 报告选定面数据

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_FACE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，具体翻译如下：

UF_print_syslog是V18版本新增的功能。

仅回答译文，不要添加其他内容。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    #define WRITE3F(X)  (write_double_array(#X, X, 1, 3))  
    #define WRITE6F(X)  (write_double_array(#X, X, 2, 3))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void write_double_array(char *name, double *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
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
            sprintf(msg, "\n");  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    static void report_face_data(tag_t face)  
    {  
        int  
            norm_dir,  
            type;  
        double  
            point[3],  
            dir[3],  
            box[6],  
            radius,  
            rad_data;  
        UF_CALL(UF_MODL_ask_face_data(face, &type, point, dir, box, &radius,  
            &rad_data, &norm_dir));  
        WRITE_D(face);  
        WRITE_D(type);  
        WRITE3F(point);  
        WRITE3F(dir);  
        WRITE6F(box);  
        WRITE_F(radius);  
        WRITE_F(rad_data);  
        WRITE_D(norm_dir);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        while ((face = select_a_face("Select a face")) != NULL_TAG)  
            report_face_data(face);  
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

> 这段代码是一个NX二次开发示例，主要用于选择并报告所选面的数据。
>
> 代码的主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印出调用UF函数失败的错误信息。
> 2. 定义了一个面选择掩码函数mask_for_faces，用于在对话框中只允许选择实体类型的面。
> 3. 定义了一个选择面的函数select_a_face，用于打开对话框，让用户选择一个面，并返回所选面的tag。
> 4. 定义了一些写入窗口的函数，用于将面的各种数据写入列表窗口。
> 5. 定义了一个报告面数据的函数report_face_data，用于查询一个面的各种数据，并调用写入窗口的函数将数据打印出来。
> 6. 定义了一个主函数do_it，用于循环调用select_a_face选择面，然后调用report_face_data报告数据。
> 7. 定义了ufusr函数，用于初始化NX，调用do_it函数，然后终止NX。
> 8. 定义了一个卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 整体来看，这段代码实现了选择面并获取面数据的功能。选择面使用了对话框，数据报告使用了列表窗口。代码结构清晰，注释详细，是一个不错的NX二次开发示例。
>

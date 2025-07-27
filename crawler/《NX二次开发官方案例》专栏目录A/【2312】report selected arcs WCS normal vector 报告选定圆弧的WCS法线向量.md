### 【2312】report selected arcs WCS normal vector 报告选定圆弧的WCS法线向量

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_object_types.h>  
    #include <uf_trns.h>  
    #include <uf_vec.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[1] = { { UF_circle_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_arc(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an arc", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_arcs, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
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
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void map_vec_abs2wcs(double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 },  
            origin[3],  
            mx[16];  
        int  
            irc;  
        tag_t  
            input_csys,  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&input_csys));  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            arc,  
            mx;  
        double  
            normal[3],  
            xyz[9];  
        while ((arc = select_an_arc("Report arc normal WCS")) != NULL_TAG)  
        {  
            UF_CALL(UF_CSYS_ask_matrix_of_object(arc, &mx));  
            UF_CALL(UF_CSYS_ask_matrix_values(mx, xyz));  
            UF_VEC3_copy(&(xyz[6]), normal);  
            map_vec_abs2wcs(normal);  
            WRITE3F(normal);  
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

> 这段NX二次开发代码的主要功能如下：
>
> 1. 定义了ECHO和report_error两个宏，用于输出日志信息和错误报告。
> 2. 定义了mask_for_arcs函数，用于设置选择掩码，仅允许选择圆形类型的对象。
> 3. 定义了select_an_arc函数，用于打开选择对话框，让用户选择一个圆形对象，并返回其tag。
> 4. 定义了WRITE3F宏，用于将一个3元素数组输出到日志窗口。
> 5. 定义了map_vec_abs2wcs函数，用于将一个向量从绝对坐标系转换到工作坐标系。
> 6. 定义了do_it函数，循环调用select_an_arc选择圆形对象，获取其法向量，并转换到工作坐标系后输出。
> 7. 定义了ufusr函数，这是用户入口函数，初始化后调用do_it，然后终止。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总的来说，这段代码实现了选择圆形对象，获取其法向量，转换到工作坐标系后输出的功能，并且包含日志输出和错误处理。
>

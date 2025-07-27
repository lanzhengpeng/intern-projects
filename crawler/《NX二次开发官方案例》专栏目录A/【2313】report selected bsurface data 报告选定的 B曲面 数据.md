### 【2313】report selected bsurface data 报告选定的 B曲面 数据

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_vec.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(n,X) (write_double_array_to_listing_window(#X, X, n, 4))  
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
    static void report_face_bsurf_data(tag_t face, logical terse)  
    {  
        logical  
            untrimmed;  
        UF_MODL_bsurface_t  
            bsurf;  
        if (UF_CALL(UF_MODL_ask_bsurf(face, &bsurf))) return;  
        WRITE_D(bsurf.num_poles_u);  
        WRITE_D(bsurf.num_poles_v);  
        WRITE_D(bsurf.order_u);  
        WRITE_D(bsurf.order_v);  
        WRITE_L(bsurf.is_rational);  
        if (!terse)  
        {  
            WRITEnF(bsurf.num_poles_u + bsurf.order_u, bsurf.knots_u);  
            WRITEnF(bsurf.num_poles_v + bsurf.order_v, bsurf.knots_v);  
            WRITEnF4(bsurf.num_poles_u * bsurf.num_poles_v, (double *)bsurf.poles);  
        }  
        UF_CALL(UF_MODL_free_bsurf_data(&bsurf));  
        untrimmed = uf5411(&face);  
        WRITE_L(untrimmed);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            bsurf;  
        while ((bsurf = select_a_bsurf("Report bsurface data")) != NULL_TAG)  
            report_face_bsurf_data(bsurf, FALSE);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是选择并报告NX中的bsurf（B样条曲面）数据。以下是代码的主要组成部分：
>
> 1. 头文件包含：代码包含了必要的NX二次开发头文件，如uf.h, uf_ui.h等。
> 2. 宏定义：定义了几个宏，用于简化错误报告、变量打印等操作。
> 3. 选择函数：mask_for_bsurfs函数用于设置选择掩码，确保只选择bsurf类型。select_a_bsurf函数使用单选对话框让用户选择一个bsurf。
> 4. 报告函数：report_face_bsurf_data函数用于获取并报告一个bsurf的数据，包括控制点数量、阶数、是否为有理bsurf等。
> 5. 主要逻辑：do_it函数循环调用select_a_bsurf和report_face_bsurf_data，直到用户没有选择任何bsurf。
> 6. 入口函数：ufusr是程序的入口函数，用于初始化和调用主要逻辑。
> 7. 卸载函数：ufusr_ask_unload用于处理程序的卸载。
>
> 综合来看，这段代码的主要功能是使用户能够选择NX中的bsurf，并获取其详细信息，为NX的bsurf操作提供了方便的二次开发接口。
>

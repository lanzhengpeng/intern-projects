### 【1449】report curve parm 报告曲线参数

#### 代码

```cpp
    /*HEAD REPORT_CURVE_PARM CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #define WRITE(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            WRITE(msg);  
            WRITE(err);  
            WRITE("\n");  
            WRITE(call);  
            WRITE(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve = NULL_TAG;  
        double  
            parm = 0.,  
            pnt[3] = { 0., 0., 0. },  
            pos[3] = { 0., 0., 0. };  
        while (((curve = select_a_curve("Select curve")) != NULL_TAG) &&  
            (specify_point("Indicate location", pos)))  
        {  
            if (!UF_CALL(UF_MODL_ask_curve_parm(curve, pos, &parm, pnt)))  
            {  
                WRITE_F(parm);  
                WRITE3F(pnt);  
            }  
            else  
            {  
                if (!UF_CALL(UF_MODL_ask_curve_parm_no_ext(curve, pos, &parm)))  
                    WRITE_F(parm);  
            }  
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

> 这段代码是一个NX二次开发示例，其主要功能是选择曲线并查询指定点的参数。代码的关键点包括：
>
> 1. 定义错误报告函数report_error，用于记录函数调用失败时的错误信息。
> 2. 定义数组写入函数write_double_array_to_listing_window，用于将数组写入列表窗口。
> 3. 定义点输入函数specify_point，用于提示用户指定点的坐标。
> 4. 定义曲线选择函数select_a_curve，用于选择曲线，并设置曲线高亮。
> 5. 主函数do_it中，循环调用select_a_curve和specify_point，选择曲线和指定点，然后调用UF_MODL_ask_curve_parm查询点的参数，并将结果写入列表窗口。
> 6. 用户函数ufusr中调用主函数do_it，并在函数开始和结束时进行NX的初始化和终止。
> 7. 定义卸载函数ufusr_ask_unload，用于卸载当前加载项。
> 8. 整个代码流程清晰，通过错误报告和数组写入函数，提高了代码的健壮性和可读性。
>
> 这段代码通过NX的二次开发接口，实现了曲线参数查询的功能，展示了NX二次开发的基本流程。
>

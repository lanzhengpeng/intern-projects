### 【2429】report wcs values of component with suppressed display 报告隐藏显示组件的WCS值

#### 代码

```cpp
    /*HEAD REPORT_WCS_VALUES_OF_COMPONENT_WITH_SUPPRESSED_DISPLAY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_csys.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
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
    /* qq3123197280 */  
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    /* qq3123197280 */  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(X,n)(write_double_array_to_listing_window(#X, X, 1, n))  
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
    static void do_it(void)  
    {  
        tag_t  
            disp_part = UF_PART_ask_display_part(),  
            tmptag = NULL_TAG,  
            comp = NULL_TAG,  
            wcs = NULL_TAG,  
            mtx = NULL_TAG;  
        double   
            mtxvals[ 9 ] = { 0., 0., 0.,  0., 0., 0.,  0., 0., 0. };   
        if( NULL_TAG == disp_part ) return;  
        if((comp = ask_next_component( disp_part, comp ))!=NULL_TAG)  
        {  
            UF_CALL( UF_DISP_set_display( UF_DISP_SUPPRESS_DISPLAY ));  
            tmptag = UF_ASSEM_ask_prototype_of_occ( comp );  
            UF_CALL( UF_PART_set_display_part( tmptag ));  
            UF_CALL( UF_CSYS_ask_wcs( &wcs ));  
            UF_CALL( UF_CSYS_ask_matrix_of_object ( wcs, &mtx ));  
            UF_CALL( UF_CSYS_ask_matrix_values ( mtx, mtxvals ));  
            WRITE9F( mtxvals );  
            UF_CALL( UF_PART_set_display_part( disp_part ));  
            UF_CALL( UF_DISP_set_display( UF_DISP_UNSUPPRESS_DISPLAY ));  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时报告错误信息。
> 2. 定义了一个函数ask_next_component，用于在组件中遍历下一个组件。
> 3. 定义了一个宏WRITE，用于将字符串写入列表窗口。
> 4. 定义了一系列宏WRITE2F、WRITE3F、WRITE9F等，用于将不同维度的浮点数组写入列表窗口。
> 5. 定义了函数write_double_array_to_listing_window，用于将浮点数组写入列表窗口。
> 6. 定义了函数do_it，用于执行以下操作：获取当前显示部件遍历当前显示部件的组件如果组件不为空，则设置组件为不显示获取组件的模型设置模型为当前显示部件获取模型的工作坐标系获取工作坐标系的变换矩阵将变换矩阵写入列表窗口恢复组件的显示恢复模型的显示
> 7. 获取当前显示部件
> 8. 遍历当前显示部件的组件
> 9. 如果组件不为空，则设置组件为不显示
> 10. 获取组件的模型
> 11. 设置模型为当前显示部件
> 12. 获取模型的工作坐标系
> 13. 获取工作坐标系的变换矩阵
> 14. 将变换矩阵写入列表窗口
> 15. 恢复组件的显示
> 16. 恢复模型的显示
> 17. 定义了ufusr函数，作为NX的入口函数，调用do_it执行功能。
> 18. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总体来说，这段代码实现了在NX中遍历组件，获取组件的工作坐标系变换矩阵，并将矩阵写入列表窗口的功能。
>

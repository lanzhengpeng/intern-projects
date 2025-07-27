### 【1034】delete retained dimensions 删除保留尺寸

#### 代码

```cpp
    /*HEAD DELETE_RETAINED_DIMENSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
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
    static tag_t ask_next_dim(tag_t part, tag_t dim)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &dim))  
            && (dim != NULL_TAG))  
        {  
            if (!UF_ASSEM_is_occurrence(dim)) return dim;  
        }  
        return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        logical  
            retflg;  
        int  
            inx = 0,  
            counter = 0;  
        tag_t  
            ret_dim = NULL_TAG,  
            ret_dims[100], /* you may require a larger array 里海译:您可能需要一个更大的数组。 */  
            part = UF_PART_ask_display_part();  
        char  
            messg[133];  
        for( inx = 0; inx < 100; inx++ )  
            ret_dims[inx] = NULL_TAG;  
        while ((ret_dim = ask_next_dim(part, ret_dim)) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_DRF_is_annotation_retained(ret_dim, &retflg)) &&  
               (retflg == TRUE))  
            {  
                ret_dims[counter] = ret_dim;  
                counter++;  
                WRITE_D(ret_dim);  
                UF_CALL(UF_DISP_set_highlight(ret_dim, TRUE));  
            }  
        }  
        sprintf(messg, "Found %d retained dimensions", counter);  
        uc1601(messg, TRUE);  
        if (counter > 0) UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
        for(inx = 0; inx < counter; inx++ )  
        {  
            UF_CALL( UF_OBJ_delete_object( ret_dims[inx] ));  
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

> 这段代码是用于在NX中删除保留尺寸的二次开发函数。
>
> 主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于在函数执行失败时输出错误信息。
> 2. 定义了一个获取下一个尺寸的函数ask_next_dim，用于遍历零件中的所有尺寸，并返回下一个非引用尺寸的tag。
> 3. 定义了一个写整数到列表窗口的函数write_integer_to_listing_window，用于将整数输出到列表窗口。
> 4. do_it函数用于遍历零件中的所有尺寸，找出保留的注释尺寸，并将这些尺寸的tag保存到数组ret_dims中，同时高亮显示这些尺寸。最后统计保留尺寸的数量并输出，然后删除这些保留尺寸。
> 5. ufusr函数是NX调用的主函数，首先初始化NX环境，然后调用do_it函数执行主要功能，最后终止NX环境。
> 6. ufusr_ask_unload函数返回立即卸载，表示NX在调用完ufusr函数后可以立即卸载这个二次开发函数。
>
> 综上所述，这段代码实现了在NX中查找并删除保留的注释尺寸的功能。
>

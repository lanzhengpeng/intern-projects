### 【0502】compare two bodies in different parts for equivalence 比较不同部件中的两个实体是否等价

#### 代码

```cpp
    /*HEAD COMPARE_TWO_BODIES_IN_DIFFERENT_PARTS_FOR_EQUIVALENCE CCC UFUN */  
    /*----------------------------------------------------------------------------  
    To test this, open two parts in your session, each of which contains one  
    and only one body.  
    This program assumes that both bodies are created in the same orientation.  
    If that is not the case, you can modify the transformation matrices to  
    correct for that situation.  
    ----------------------------------------------------------------------------*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_mdlcmp.h>  
    #include <uf_mtx.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* 里海 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    /* 里海 */  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    /* 里海 */  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            num_parts = 0;  
        tag_t  
            part1 = NULL_TAG,  
            part2 = NULL_TAG;  
        double  
            transform_1[ 16 ],  
            transform_2[ 16 ];  
        UF_MODL_compare_options_t  
            compare_options;  
        UF_MODL_compare_part_map_data_t  
            mapping_data;  
        num_parts = UF_PART_ask_num_parts();  
        WRITE_D( num_parts );  
        if (num_parts != 2) return;  
        part1 = UF_PART_ask_nth_part ( 0 );  
        part2 = UF_PART_ask_nth_part ( 1 );  
        UF_MTX4_identity( transform_1 );  
        UF_MTX4_identity( transform_2 );  
        UF_MODL_INIT_COMPARE_OPTIONS( &compare_options );  
        UF_CALL( UF_MODL_model_compare_2( part1, NULL_TAG, transform_1,  
                                          part2, NULL_TAG, transform_2,  
                                          &compare_options, &mapping_data));  
        WRITE_L( mapping_data.identical_parts );  
        UF_CALL( UF_MODL_free_compare_data ( &mapping_data ));  
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

> 这段代码是用于比较两个NX部件中实体是否完全相同的二次开发代码。其主要功能包括：
>
> 1. 获取当前会话中部件的数量，并确保只有两个部件。
> 2. 获取这两个部件的tag。
> 3. 初始化两个4x4的变换矩阵，默认为单位矩阵，表示两个部件没有进行任何旋转和平移。
> 4. 初始化比较选项，使用默认设置。
> 5. 调用UF_MODL_model_compare_2函数，比较两个部件，并返回映射数据。
> 6. 判断映射数据中的identical_parts标志，以确定两个部件中的实体是否完全相同。
> 7. 释放映射数据。
> 8. 通过宏定义，实现了错误报告和日志输出功能。
> 9. 主函数 ufusr 完成初始化、执行主体逻辑、终止会话。
> 10. 提供卸载函数 ufusr_ask_unload，以便立即卸载该用户函数。
>
> 总体来说，这段代码实现了在NX会话中比较两个部件中实体是否完全相同的功能。
>

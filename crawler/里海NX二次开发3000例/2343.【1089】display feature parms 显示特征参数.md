### 【1089】display feature parms 显示特征参数

#### 代码

```cpp
    /*HEAD DISPLAY_FEATURE_PARMS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的。 */  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void display_feature_expression( tag_t feat, tag_t expr )  
    {  
        double  
            pnt[3];  
        UF_MODL_dimension_data_t  
            dim_data;  
        if ( UF_MODL_get_dimension_data( expr, feat,  &dim_data )) return;  
        UF_free( dim_data.top_array );  
        if( 0 == dim_data.type )  
        {  
            UF_VEC3_midpt(dim_data.first_point, dim_data.second_point, pnt);  
        }  
        else  
        {  
            UF_VEC3_copy(dim_data.first_point, pnt);  
        }  
        display_temporary_text(dim_data.drf_txt, pnt);  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            num_exps = 0;  
        tag_t  
            my_feat = NULL_TAG,  
            * exps = NULL;  
        char  
            ** descriptions;  
        while( (my_feat = select_a_feature( "Select a Feature:" )) != NULL_TAG )  
        {  
            UF_DISP_refresh();  
            UF_CALL( UF_MODL_ask_exp_desc_of_feat( my_feat, &num_exps,  
                                                   &descriptions, &exps ));  
            for( inx = 0; inx < num_exps; inx++ )  
            {  
                display_feature_expression( my_feat, exps[inx] );  
            }  
            UF_free( exps );  
            UF_free_string_array( num_exps, descriptions );  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_set_retiring_flag( 2 ); /* notify about retiring functions 里海译:关于废弃功能的通告 */  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发程序，主要功能是选择特征并显示其尺寸表达式。
>
> 主要流程包括：
>
> 1. 使用UF_UI_select_feature函数让用户选择特征。
> 2. 使用UF_MODL_ask_exp_desc_of_feat函数获取特征的所有尺寸表达式描述和表达式tag。
> 3. 遍历所有尺寸表达式，使用UF_MODL_get_dimension_data获取表达式的详细信息。
> 4. 计算表达式的文本位置，并使用UF_DISP_display_temporary_text函数在图形区显示表达式的文本。
> 5. 重复上述过程，直到用户不再选择特征。
> 6. 使用UF_print_syslog函数打印错误信息。
> 7. 使用UF_DISP_refresh函数刷新图形区。
> 8. 定义了一个错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 9. 定义了选择特征和显示文本的辅助函数。
> 10. 在ufusr函数中初始化和终止NX Open，并调用主函数do_it。
> 11. 定义了卸载函数ufusr_ask_unload。
>
> 整体来看，这段代码实现了选择特征并显示其尺寸表达式的功能，利用了NX Open C++ API提供的几何建模和显示相关的函数。
>

### 【2020】report chamfer dimension data 报告倒角尺寸数据

#### 代码

```cpp
    /*HEAD REPORT_CHAMFER_DIMENSION_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能，主要用于在用户空间打印内核日志。它允许用户空间应用程序读取内核日志缓冲区，并将其输出到标准输出。与传统的syslog相比，UF_print_syslog提供了更高效的方式在用户空间中查看内核日志，从而提高了日志查看的效率。 */  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    /* qq3123197280 */  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a chamfer dimension", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static void report_chamfer_dimension_data( tag_t chamfer_dim )  
    {  
        int  
            type = 0,  
            subtype = 0;  
        UF_DRF_chamfer_dimension_data_t   
            cham_dim_data;  
        char  
            symbol_name[132] = { "" },  
            msg[300] = { "" };  
        /*   
        The following line is a workaround for PR 5130871   
        Once that is fixed, this line should be removed,   
        and a UF_free will need to be added  
        */  
        cham_dim_data.symbol_name = symbol_name;   
        if( NULL_TAG == chamfer_dim ) return;  
        UF_CALL( UF_OBJ_ask_type_and_subtype( chamfer_dim, &type, &subtype ));  
        if( (type != UF_dimension_type) ||   
            (subtype != UF_dim_chamfer_subtype ) ) return;  
        UF_CALL(UF_DRF_ask_chamfer_dimension_data(chamfer_dim, &cham_dim_data));  
        sprintf( msg, "cham_dim_data.symbol_name: %s\n",   
                    cham_dim_data.symbol_name );  
        WRITE( msg );  
        WRITE_F( cham_dim_data.space_factor );  
        switch ( cham_dim_data.form )  
        {  
            case UF_DRF_CHAMFER_FORM_SYMBOL :  
                strcpy(msg, "cham_dim_data.form: UF_DRF_CHAMFER_FORM_SYMBOL\n");  
                break;  
            case UF_DRF_CHAMFER_FORM_SIZE :  
                strcpy(msg, "cham_dim_data.form: UF_DRF_CHAMFER_FORM_SIZE\n");  
                break;  
            case UF_DRF_CHAMFER_FORM_SIZEANGLE :  
                strcpy(msg, "cham_dim_data.form: UF_DRF_CHAMFER_FORM_SIZEANGLE\n");  
                break;  
            case UF_DRF_CHAMFER_FORM_ANGLESIZE :  
                strcpy(msg, "cham_dim_data.form: UF_DRF_CHAMFER_FORM_ANGLESIZE\n");  
                break;  
        }  
        WRITE( msg );  
        switch ( cham_dim_data.stub_type )  
        {  
            case UF_DRF_CHAMFER_TEXTABOVE_LEADER_NOSTUB :  
                strcpy(msg,"cham_dim_data.stub_type: ");  
                strcat( msg, "UF_DRF_CHAMFER_TEXTABOVE_LEADER_NOSTUB\n");  
                break;  
                        case UF_DRF_CHAMFER_TEXTAFTER_LEADER_NOSTUB :  
                strcpy(msg,"cham_dim_data.stub_type: ");  
                strcat( msg, "UF_DRF_CHAMFER_TEXTAFTER_LEADER_NOSTUB\n");  
                break;  
                        case UF_DRF_CHAMFER_TEXTABOVE_STUB :  
                strcpy(msg,"cham_dim_data.stub_type: ");  
                strcat( msg, "UF_DRF_CHAMFER_TEXTABOVE_STUB\n");  
                break;  
                        case UF_DRF_CHAMFER_TEXTAFTER_STUB  :  
                strcpy(msg,"cham_dim_data.stub_type: ");  
                strcat( msg, "UF_DRF_CHAMFER_TEXTAFTER_STUB\n");  
                break;  
        }  
        WRITE( msg );  
        switch ( cham_dim_data.leader_type )  
        {  
            case UF_DRF_CHAMFER_PERPENDICULAR_LEADER :  
                strcpy( msg,"cham_dim_data.leader_type: ");  
                strcat( msg, "UF_DRF_CHAMFER_PERPENDICULAR_LEADER\n" );  
                break;  
            case UF_DRF_CHAMFER_PARALLEL_LEADER :  
                strcpy( msg,"cham_dim_data.leader_type: ");  
                strcat( msg, "UF_DRF_CHAMFER_PARALLEL_LEADER\n" );  
                break;  
            case UF_DRF_CHAMFER_LINEAR_CHAMFER_DIM :  
                strcpy( msg,"cham_dim_data.leader_type: ");  
                strcat( msg, "UF_DRF_CHAMFER_LINEAR_CHAMFER_DIM\n" );  
                break;  
        }  
        WRITE( msg );  
        switch ( cham_dim_data.symbol_type )  
        {  
            case UF_DRF_CHAMFER_NONE_SYMBOL :  
                strcpy( msg,"cham_dim_data.symbol_type: ");  
                strcat( msg, "UF_DRF_CHAMFER_NONE_SYMBOL\n" );  
                break;  
            case UF_DRF_CHAMFER_PREFIX_SYMBOL :  
                strcpy( msg,"cham_dim_data.symbol_type: ");  
                strcat( msg, "UF_DRF_CHAMFER_PREFIX_SYMBOL\n" );  
                break;  
            case UF_DRF_CHAMFER_SUFFIX_SYMBOL :  
                strcpy( msg,"cham_dim_data.symbol_type: ");  
                strcat( msg, "UF_DRF_CHAMFER_SUFFIX_SYMBOL\n" );  
                break;  
                    }  
        WRITE( msg );  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        tag_t  
            chamfer_dim = NULL_TAG;  
        while((chamfer_dim = select_a_dimension("Select Chamfer Dimension:"))   
                                                                != NULL_TAG )  
        {  
            report_chamfer_dimension_data( chamfer_dim );  
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

> 这是一段用于获取NX中倒角标注信息的二次开发代码，主要功能如下：
>
> 1. 包含头文件：代码包含了必要的NX Open API头文件，如uf.h、uf_drf.h等。
> 2. 错误处理函数：定义了report_error函数，用于在出错时打印错误信息，包括文件名、行号、函数名、错误码和错误描述。
> 3. 选择标注：定义了select_a_dimension函数，用于通过对话框让用户选择一个倒角标注。
> 4. 获取标注信息：定义了report_chamfer_dimension_data函数，用于获取倒角标注的详细信息，包括符号名、空间因子、形式、文字位置、引线类型等，并打印到列表窗口。
> 5. 主函数：定义了do_it函数，用于循环选择倒角标注并打印其信息。
> 6. ufusr函数：是二次开发的入口函数，在这里调用do_it函数以执行主要功能。
> 7. ufusr_ask_unload函数：用于设置二次开发模块卸载方式。
>
> 总体来说，这段代码实现了选择并获取NX中倒角标注信息的功能，使用了NX Open API进行二次开发，涉及标注选择、信息获取等操作。
>

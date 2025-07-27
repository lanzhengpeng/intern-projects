### 【1481】is object weld 对象焊接

#### 代码

```cpp
    /*HEAD IS_OBJECT_WELD CCC UFUN */  
    /*--------------------------------------------------------------------  
    This can be linked to run internally or externally.  
    If it is linked to run externally, you can run it against a given part  
    by specifying the part name on the command line after the program name.  
    For example:  
        is_object_weld.exe my_weld_part.prt  
    --------------------------------------------------------------------*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_weld.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。

只回答译文，不要废话 */  
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
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body = NULL_TAG,  
            feature_set_tag = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        logical  
            is_weld = FALSE;  
        char  
            msg[133] = { "           Object is a weld body.\n" };  
        while ((body = ask_next_body( part, body )) != NULL_TAG)  
        {  
            WRITE_D( body );  
            UF_CALL( UF_WELD_is_object_weld( body, &is_weld, &feature_set_tag ));  
            if( is_weld == TRUE ) WRITE( msg );  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }  
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    void main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            if (argc > 1)  
            {  
                int  
                    cnt = 0,  
                    ii;  
                logical  
                    new_flag  = FALSE,  
                    save_flag = FALSE;  
                tag_t  
                    part;  
                UF_PART_load_status_t  
                    status;  
                for (ii=1; ii<argc; ii++)  
                {  
                    if (!strcmp(argv[ii], "-save")) save_flag = TRUE;  
                    else if (!strcmp(argv[ii], "-new")) new_flag = TRUE;  
                    else  
                    {  
                        printf("%d.  %s\n", ++cnt, argv[ii]);  
                        if (new_flag)  
                        {  
                            if (!UF_CALL(UF_PART_new(argv[ii], UF_PART_ENGLISH,  
                                &part)))  
                            {  
                                do_it();  
                                if (save_flag)  UF_CALL(UF_PART_save());  
                                UF_PART_close_all();  
                            }  
                        }  
                        else  
                        {  
                            if (UF_CALL(UF_PART_open(argv[ii], &part, &status))  
                                || status.n_parts > 0)  
                                check_load_status(&status);  
                            else  
                            {  
                                do_it();  
                                if (save_flag)  UF_CALL(UF_PART_save());  
                                UF_PART_close_all();  
                            }  
                        }  
                    }  
                }  
                printf("\nProcessed %d parts.\n", cnt);  
            }  
            else do_it();  
            UF_CALL(UF_terminate());  
        }  
    }

```

#### 代码解析

> 这是NX的二次开发代码，其主要功能是检查当前打开的零件中是否包含焊接体，并输出检测结果。
>
> 主要功能点包括：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了写入整数到列表窗口的函数write_integer_to_listing_window。
> 3. 定义了遍历零件中实体的函数ask_next_body。
> 4. 主函数do_it遍历零件中的所有实体，并使用UF_WELD_is_object_weld函数检查每个实体是否为焊接体，如果是，则输出提示信息。
> 5. ufusr函数是NX二次开发的标准入口函数，在这里调用do_it函数。
> 6. main函数根据命令行参数，打开指定的NX零件文件，然后调用do_it函数进行检查，最后保存并关闭零件文件。
> 7. check_load_status函数用于检查零件加载状态，并输出错误信息。
> 8. 该代码既可以在NX内部运行，也可以作为外部程序通过命令行参数指定零件文件进行检测。
>
> 总体来说，该代码利用NX的二次开发接口，实现了对零件中焊接体的检测功能。
>

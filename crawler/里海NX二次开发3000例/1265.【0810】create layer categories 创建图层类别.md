### 【0810】create layer categories 创建图层类别

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_layer.h>  
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
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            category;  
        UF_LAYER_category_info_t  
            category_info;  
        for (ii=0; ii<256; ++ii) category_info.layer_mask[ii] = FALSE;  
        for (ii=0; ii<256; ++ii)  
        {  
            sprintf(category_info.name, "cat%d", ii+1);  
            sprintf(category_info.descr, "category for layer %d", ii+1);  
            category_info.layer_mask[ii] = TRUE;  
            if (ii > 0) category_info.layer_mask[ii-1] = FALSE;  
            UF_CALL(UF_LAYER_create_category(&category_info, &category));  
        }  
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
    int main( int argc, char *argv[] )  
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
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现以下功能：
>
> 1. 定义了ECHO和report_error两个宏，用于输出信息到日志窗口和系统日志，并在函数调用失败时输出错误信息。
> 2. 定义了do_it函数，用于创建256个图层类别，每个类别对应一个图层，并设置图层掩码。
> 3. 定义了check_load_status函数，用于检查零件加载状态，并输出加载失败的信息。
> 4. 在main函数中，首先初始化NX系统，然后根据命令行参数打开或新建零件，调用do_it函数，然后保存和关闭零件。如果没有参数，则直接调用do_it函数。最后终止NX系统。
> 5. 使用宏UF_CALL包装NX API调用，在调用失败时输出错误信息。
> 6. 使用宏#define UF_CALL(X) (report_error(FILE,LINE, #X, (X)))，简化了错误处理流程。
> 7. 使用va_list实现可变参数的ECHO宏，用于输出格式化信息。
> 8. 代码中包含了典型的NX二次开发流程，包括系统初始化、零件操作、错误处理等，具有一定的参考价值。
>

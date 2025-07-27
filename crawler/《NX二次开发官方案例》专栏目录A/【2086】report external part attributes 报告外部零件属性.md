### 【2086】report external part attributes 报告外部零件属性

#### 代码

```cpp
    /*HEAD REPORT_EXTERNAL_PART_ATTRIBUTES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_cfi.h>  
    #include <uf_attr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0,  
            ii,  
            n_attrs;  
        UF_ATTR_part_attr_p_t  
            attrs;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_ATTR_ask_part_attrs_in_file(part_name,&n_attrs,&attrs));  
                for (ii = 0; ii < n_attrs; ii++)  
                    printf("\t%s = %s\n", attrs[ii].title, attrs[ii].string_value);  
                if (n_attrs > 0) UF_free(attrs);  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要功能是查询并打印出NX部件文件中的所有属性信息。
>
> 代码的关键点包括：
>
> 1. 定义了一个错误报告函数report_error，用于在调用NX API时捕获并打印错误信息。
> 2. 在main函数中，首先调用UF_initialize初始化NX环境，然后遍历命令行传入的部件文件路径。
> 3. 对于每个部件文件，调用UF_ATTR_ask_part_attrs_in_file获取所有属性信息，并打印出来。
> 4. 使用UF_terminate终止NX环境。
> 5. 使用了宏UF_CALL封装所有NX API调用，以简化错误处理。
> 6. 提供了两种运行方式，直接运行和通过NX/Manager或IMAN模式运行。
> 7. 使用了NX提供的UI函数UF_UI_open_listing_window和UF_UI_write_listing_window，可以在NX窗口中显示结果。
> 8. 使用了NX提供的日志打印函数UF_print_syslog，可以在NX系统日志中打印信息。
> 9. 考虑了内存释放，在获取属性后释放了内存。
>
> 总的来说，这段代码是一个NX二次开发的简单示例，实现了查询部件属性并打印的功能。
>

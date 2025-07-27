### 【2264】report part attributes without opening part external 报告部件属性，无需打开部件外部的报告

#### 代码

```cpp
    /*HEAD REPORT_PART_ATTRIBUTES_WITHOUT_OPENING_PART_EXTERNAL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18版本中是新增的。 */  
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
    static void report_part_atts(char *part_name)  
    {  
        int  
            ii,  
            n_attrs;  
        UF_ATTR_part_attr_p_t  
            attrs;  
        char  
            msg[UF_UI_MAX_STRING_LEN];  
            UF_CALL(UF_ATTR_ask_part_attrs_in_file(part_name,&n_attrs,&attrs));  
            sprintf(msg, "%s has %d attributes\n", part_name, n_attrs);  
            WRITE(msg);  
            for (ii = 0; ii < n_attrs; ii++)  
            {  
                sprintf(msg, "  %d.  %s = %s\n", ii+1, attrs[ii].title,  
                    attrs[ii].string_value);  
                WRITE(msg);  
            }  
            if (n_attrs > 0) UF_free(attrs);  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                report_part_atts(part_name);  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这是段用于在NX中查询和报告部件属性的二次开发代码，主要包括以下功能：
>
> 1. 错误报告函数：定义了report_error函数，用于在出错时打印错误信息。
> 2. 部件属性查询和报告：定义了report_part_atts函数，用于查询指定部件的属性，并打印属性名称和值。
> 3. 主函数：在主函数中，初始化NX环境，遍历所有打开的部件，并调用report_part_atts函数打印每个部件的属性信息。
> 4. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载。
> 5. 代码使用说明：首先需要编译这段代码生成ufu文件，然后在NX中加载该ufu文件，运行后会在Listing窗口中打印出所有打开部件的属性信息。
> 6. 注意事项：该代码在NX中查询部件属性，因此需要在NX环境下编译和运行。同时，需要注意NX的二次开发接口和函数的使用方法。
>
> 综上所述，这段代码主要用于在NX中查询和报告部件属性信息，方便进行二次开发时获取部件的属性数据。
>

### 【1560】load specified color definition file 加载指定颜色定义文件

#### 代码

```cpp
    /*HEAD LOAD_SPECIFIED_COLOR_DEFINITION_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    static void strip_trailing_spaces(char *s)  
    {  
        int ii;  
        for (ii = strlen(s) - 1; s[ii] == ' '; ii--) s[ii] = '\0';  
    }  
    static void load_color_definition_file(char *cdf_spec)  
    {  
        int  
            ii;  
        double  
            rgb[3];  
        char  
            a_line[MAX_LINE_SIZE+1],  
            color_name[UF_DISP_MAX_NAME_SIZE+1],  
            *ptr;  
        FILE  
            *cdf;  
        if ((cdf = fopen(cdf_spec, "r")) != NULL)  
        {  
        /*  Read 4 header lines 里海译:这里是4行标题的翻译：

1. 机器学习简介
2. 线性回归
3. 逻辑回归
4. 决策树 */  
            for (ii = 0; ii < 4; ii++)  
                fgets(a_line, MAX_LINE_SIZE, cdf);  
            ii = 0;  
            while ((fgets(a_line, MAX_LINE_SIZE, cdf)) != NULL)  
            {  
                strncpy(color_name, a_line, UF_DISP_MAX_NAME_SIZE);  
                color_name[UF_DISP_MAX_NAME_SIZE] = '\0';  
                strip_trailing_spaces(color_name);  
                ptr = a_line + 32;  
                rgb[0] = atof(ptr);  
                ptr = ptr + 11;  
                rgb[1] = atof(ptr);  
                ptr = ptr + 11;  
                rgb[2] = atof(ptr);  
                UF_CALL(UF_DISP_set_color(ii, UF_DISP_rgb_model, color_name, rgb));  
                ii++;  
            }  
            fclose(cdf);  
            UF_CALL(UF_DISP_load_color_table());  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        char  
            cdf_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            filter[UF_CFI_MAX_PATH_NAME_SIZE],  
            *root_dir;  
        UF_CALL(UF_translate_variable("UGII_ROOT_DIR", &root_dir));  
        UF_CALL(uc4575(root_dir, 0, "xxx.cdf", filter));  
        strcpy(strstr(filter, "xxx.cdf"), "*.cdf");  
        UF_CALL(UF_UI_create_filebox("Enter a Color Definition File Name",  
            "Color Definition File Names", filter, "", cdf_spec, &resp));  
        if (resp == UF_UI_OK)  load_color_definition_file(cdf_spec);  
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

> 根据代码内容，这是NX的二次开发代码，主要功能是加载指定的颜色定义文件，并将文件中的颜色信息添加到NX的颜色表中。
>
> 主要步骤包括：
>
> 1. 包含必要的头文件，如stdio.h、stdlib.h等。
> 2. 定义了UF_CALL宏，用于调用UF函数，并报告错误。
> 3. 定义了report_error函数，用于输出错误信息。
> 4. 定义了strip_trailing_spaces函数，用于去除字符串尾部的空格。
> 5. 定义了load_color_definition_file函数，用于加载颜色定义文件，读取文件中的颜色信息，并调用UF_DISP_set_color函数将颜色添加到颜色表。
> 6. 定义了do_it函数，用于获取颜色定义文件的路径，并调用load_color_definition_file函数。
> 7. 定义了ufusr函数，作为NX二次开发的入口函数，初始化NX，调用do_it函数，然后终止NX。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总的来说，该代码实现了加载颜色定义文件，并在NX中显示文件中定义的颜色信息的功能。
>

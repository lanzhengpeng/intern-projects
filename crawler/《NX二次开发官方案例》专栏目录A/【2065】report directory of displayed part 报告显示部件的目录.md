### 【2065】report directory of displayed part 报告显示部件的目录

#### 代码

```cpp
    /*HEAD REPORT_DIRECTORY_OF_DISPLAYED_PART CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        logical  
            is_active;  
        char  
            partname[MAX_FSPEC_SIZE+1],  
            dir_of_disp_part[MAX_FSPEC_SIZE+1],  
            shortname[MAX_FSPEC_SIZE+1];  
        UF_is_ugmanager_active(&is_active);  
        if (is_active == TRUE) return;  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(uc4576(partname, 2, dir_of_disp_part, shortname));  
        WRITE_S( dir_of_disp_part );  
        WRITE_S( shortname );  
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

> 这段NX二次开发代码的主要功能是查询当前显示的部件的相关信息，包括部件名称、所在目录和简称。
>
> 主要步骤包括：
>
> 1. 判断当前是否为UG管理模式，如果是则不执行后续代码。
> 2. 获取当前显示的部件的Tag。
> 3. 查询当前显示部件的名称。
> 4. 通过自定义函数uc4576查询部件所在目录和简称。
> 5. 打开日志窗口，并将部件所在目录和简称输出到日志窗口。
>
> 该代码通过NX提供的UF函数进行二次开发，以查询和输出当前显示部件的相关信息。
>

### 【3155】report solid edge par file 报告Solid Edge的par文件信息，包括创建时间、修改时间和访问时间，描述区域以及所有者

#### 代码

```cpp
    /*HEAD REPORT_SOLID_EDGE_PAR_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，我了解到UF_print_syslog是V18版本新增的函数。 */  
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
    #define WRITE(X)   (UF_UI_write_listing_window(X))  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static logical prompt_for_solid_edge_par_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[UF_CFI_MAX_PATH_NAME_SIZE] = { "*.par" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Solid Edge File", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            cdate[2],  
            ldate[2],  
            mdate[2];  
        char  
            msg[133],  
            darea[132] = { "" },  
            owner[17] = { "" } ,  
            datestg[21],  
            fspec[MAX_FSPEC_SIZE + 1],  
            timestg[21];  
        if (prompt_for_solid_edge_par_file_name("From Point File", fspec))  
        {  
            if (!UF_CALL(uc4564(fspec, 0, (1 << 4))))  
            {  
                UF_CALL(uc4606(cdate, mdate, ldate));  
                UF_CALL(UF_UI_open_listing_window());  
                UF_CALL(uc4582(cdate, 3, datestg, timestg));  
                sprintf(msg, "Creation date & time     - %s %s\n", datestg, timestg);  
                WRITE(msg);  
                UF_CALL(uc4582(mdate, 3, datestg, timestg));  
                sprintf(msg, "Modification date & time     - %s %s\n", datestg, timestg);  
                WRITE(msg);  
                UF_CALL(uc4582(ldate, 3, datestg, timestg));  
                sprintf(msg, "Last Access date & time     - %s %s\n", datestg, timestg);  
                WRITE(msg);  
            }  
            UF_CALL(uc4564(fspec, 0, (1 << 5)));  
            uc4607(darea);  
            sprintf(msg, "Description area:\n %s \n",  darea);  
            WRITE(msg);  
            WRITENZ(uc4603(owner));  
            WRITE_S(owner);  
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

> 根据代码注释，这是一个NX二次开发代码，用于读取Solid Edge的参数文件(.par)并打印文件的相关信息，包括创建日期、修改日期、最后访问日期、描述区域和所有者。
>
> 主要函数包括：
>
> 1. 报告错误函数：用于在函数调用失败时报告错误。
> 2. 写字符串到列表窗口：用于将字符串写入NX的列表窗口。
> 3. 写整数到列表窗口：用于将整数写入NX的列表窗口。
> 4. 提示用户输入Solid Edge参数文件名：用于提示用户输入参数文件名。
> 5. 执行函数：用于读取参数文件信息并打印。
> 6. ufusr函数：这是NX二次开发的入口函数，用于初始化、执行功能、结束。
> 7. 卸载函数：用于返回立即卸载标志。
>
> 通过以上函数，用户可以输入参数文件，然后程序将读取文件信息并打印到NX的列表窗口中。这有助于了解Solid Edge参数文件的相关信息。
>

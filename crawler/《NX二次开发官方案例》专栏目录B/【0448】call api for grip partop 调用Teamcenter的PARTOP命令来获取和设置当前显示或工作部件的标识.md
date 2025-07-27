### 【0448】call api for grip partop 调用Teamcenter的PARTOP命令来获取和设置当前显示或工作部件的标识

#### 代码

```cpp
    /*HEAD CALL_API_FOR_GRIP_PARTOP CCC UFUN */  
    /*  
        This code can be used to work around the fact that the PARTOP command  
        returns a "string too long" error when used to ASK the WORK or DSPLAY  
        part name in TeamCenter (IMAN).  
        It uses the displayed name format rather than the longer encoded  
        name format.  
        Here is some sample GRIP code which demonstrates how to call it:  
            STRING/ disprt(132), wrkprt(132), p_path(132)  
            NUMBER/ ask_flag, set_flat, disp_flag, work_flag  
            DATA/ ask_flag, 1, set_flag, 2, disp_flag, 1, work_flag, 2  
            DATA/ p_path, '/full/path/to/partop.sl_so_dll'  
        $$  Ask the displayed part  
            GRARGS/ ask_flag, disp_flag, disprt  
            XSPAWN/UFUN, p_path  
            PRINT/ 'DISPRT = ' + disprt  
        $$  Ask the work part  
            GRARGS/ ask_flag, work_flag, wrkprt  
            XSPAWN/UFUN, p_path  
            PRINT/ 'WRKPRT = ' + wrkprt  
        $$  make sure the work part matches the displayed part  
            IFTHEN/ disprt <> wrkprt  
                GRARGS/ set_flag, work_flag, disprt  
                XSPAWN/UFUN, p_path  
            ENDIF  
            HALT  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            part;  
        double  
            which_op,   /* 1 = ASK,    2 = SET 译:好的，以下是翻译结果：

ASK(询问)  
SET(设置) */  
            which_part; /* 1 = DSPLAY, 2 = WORK 译:好的，以下是翻译结果：

1. DSPLAY - 显示
2. WORK - 工作 */  
        char  
            encoded[MAX_FSPEC_SIZE+1],  
            display_name[133];  
        UF_args_t  
            args[3] = { { UF_TYPE_DOUBLE, 0, NULL },  
                        { UF_TYPE_DOUBLE, 0, NULL },  
                        { UF_TYPE_CHAR, 0, NULL }};  
        args[0].address = &which_op;  
        args[1].address = &which_part;  
        args[2].address = display_name;  
        UF_CALL(UF_ask_grip_args(3, args));  
        if (which_op == 1)  
        {  
            if (which_part == 1)  
                part = UF_PART_ask_display_part();  
            else  
                part = UF_ASSEM_ask_work_part();  
            UF_CALL(UF_PART_ask_part_name(part, encoded));  
            UF_CALL(UF_PART_name_for_display(encoded, display_name));  
            UF_CALL(UF_set_grip_args(3, args));  
        }  
        else  
        {  
            UF_CALL(UF_PART_ask_tag_of_disp_name(display_name, &part));  
            if (which_part == 1)  
                UF_CALL(UF_PART_set_display_part(part));  
            else  
                UF_CALL(UF_ASSEM_set_work_part(part));  
        }  
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

> 这段代码是NX的二次开发代码，主要实现了在TeamCenter中通过PARTOP命令获取和设置零件的显示名和工作名的功能。
>
> 代码主要包括以下几个部分：
>
> 1. 错误报告函数：用于在函数调用失败时打印错误信息。
> 2. 实际功能函数：根据传入的参数，获取或设置零件的显示名和工作名。获取时，先获取零件的标签，然后获取零件的编码名称，再转换为显示名称；设置时，先根据显示名称获取零件标签，然后设置零件为显示零件或工作零件。
> 3. 主函数：首先初始化UF函数库，然后调用功能函数，最后终止UF函数库。
> 4. 卸载函数：返回立即卸载标志。
>
> 这段代码通过封装NX的API，实现了在TeamCenter中通过PARTOP命令获取和设置零件的显示名和工作名，避免了直接使用PARTOP命令可能出现的错误。
>
